import sys
import pprint

SIZE = 20
cache = [[0x00, 0] for i in range(SIZE)]
cache_idx = 0

hit = 0
miss = 0

ratio_sum = 0

for addr_num, addr in enumerate(open(sys.argv[1])):
  
    #pprint.pprint(cache)
    got_hit = 0
    for i in range(SIZE):
        if cache[i][0] == addr:
            cache[i][1] = 0    # reset age
            hit += 1
            got_hit = 1
        else:
            cache[i][1] += 1

    oldest = 0
    oldest_idx = 0
    if (got_hit == 0):
        miss += 1
        for i in range(SIZE):
            if cache[i][1] > oldest:
                oldest = cache[i][1]
                oldest_idx = i
        cache[oldest_idx][0] = addr
        cache[oldest_idx][1] = 0
        
    ratio = float(hit)/float(hit+miss)*100.0
    #print "%d | Hit ratio: %f | hits: %d | miss: %d" % (addr_num, ratio, hit, miss)
    print ratio
