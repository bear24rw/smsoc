import sys

SIZE = 8
cache = [0 for i in range(SIZE)]
cache_idx = 0

hit = 0
miss = 0

ratio_sum = 0

for addr_num, addr in enumerate(open(sys.argv[1])):

    if addr in cache:
        hit += 1
    else:
        miss += 1
        cache[cache_idx] = addr
        cache_idx = (cache_idx + 1) % SIZE
        
    ratio = float(hit)/float(hit+miss)*100.0
    #print "%d | Hit ratio: %f | hits: %d | miss: %d | cache_idx: %d]" % (addr_num, ratio, hit, miss, cache_idx)
    print ratio
