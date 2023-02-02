#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"

#ifdef CACHE_ENABLED

typedef struct{
    bool valid;
    uint32_t sign;
    uint8_t block[64];
}CacheLine;
extern CacheLine Cache[];
void init_cache();
uint32_t cache_read(paddr_t paddr,size_t len,CacheLine *cache);
void cache_write(paddr_t paddr,size_t len,uint32_t data,CacheLine *cache);
// init the cache
//void init_cache();

// write data to cache
//void cache_write(paddr_t paddr, size_t len, uint32_t data);

// read data from cache
//uint32_t cache_read(paddr_t paddr, size_t len);

#endif

#endif
