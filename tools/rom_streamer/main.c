#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct {
    uint16_t checksum;
    char region_code;
    char rom_size;
    char version;
} rom_header;

int main(int argc, char** argv)
{

    FILE *fp;
    fp = fopen(argv[1], "r");

    fseek(fp, 0L, SEEK_END);
    int rom_size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    
    if ((rom_size % 1024) == 512)
    {
        printf("512 byte ROM header skipped...\n");
        fseek(fp, 512, SEEK_SET);
        rom_size -= 512;
    }

    printf("ROM size: %d\n", rom_size);

    int num_8k_banks;
    int num_16k_banks;

    num_8k_banks = (rom_size / 8192);

    // need at least 4 16k banks to handle all z80 address space
    if (num_8k_banks < 8) num_8k_banks = 8;
    num_16k_banks = num_8k_banks / 2;

    printf("Cartridge contains %d 16KB banks\n", num_16k_banks);

    char *rom;

    // allocate ram for the whole cartridge
    if (rom_size < 65536)
    {
        rom = malloc(65536);
        memset(rom, 0, 65536);
    }
    else
    {
        rom = malloc(rom_size);
        memset(rom, 0, rom_size);
    }

    fread(rom, 1, rom_size, fp);

    rom_header.checksum = rom[0x7FFA];
    rom_header.checksum |= rom[0x7FFA+1] << 8;
    rom_header.region_code = (rom[0x7FFF] & 0xF0) >> 4;
    rom_header.rom_size = rom[0x7FFF] & 0x0F;
    rom_header.version = rom[0x7FFE] & 0x0F;

    printf("Region code: %d\n", rom_header.region_code);
    switch (rom_header.region_code)
    {
        case 3:  printf ("SMS Japan");        break;
        case 4:  printf ("SMS Export");       break;
        case 5:  printf ("GG Japan");         break;
        case 6:  printf ("GG International"); break;
        case 7:  printf ("GG Export");        break;
        default: printf ("Unknown");          break;
    }
   
    int checksum_len;
    printf("Rom size: %x\n", rom_header.rom_size);
    switch (rom_header.rom_size)
    {
        case 0x0A: checksum_len = 0x02000;  printf ("8k");      break;
        case 0x0B: checksum_len = 0x04000;  printf ("16k");     break;
        case 0x0C: checksum_len = 0x08000;  printf ("32k");     break;
        case 0x0D: checksum_len = 0x0C000;  printf ("48k");     break;
        case 0x0E: checksum_len = 0x10000;  printf ("64k");     break;
        case 0x0F: checksum_len = 0x20000;  printf ("128k");    break;
        case 0x00: checksum_len = 0x40000;  printf ("256k+");   break;
        default:   checksum_len = rom_size; printf ("Unknown"); break;
    }
    printf (" \n");


    printf("Version: %d\n", rom_header.version);

    printf("Checksum: %x\n", rom_header.checksum);
    int checksum = 0;
    int i;
    for (i=0; i<0x7FEF+1; i++)
        checksum += rom[i];
    printf("Checksummed: %x   %d\n", checksum, checksum);
}

