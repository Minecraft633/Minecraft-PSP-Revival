#include <Source/Aurora/Graphics/vfram.h>
#include <stdio.h>

#define __MEM_SIZE 0x00200000
#define __MEM_START 0x04000000
#define __BLOCK_SIZE 512
#define __MEM_BLOCKS (__MEM_SIZE/__BLOCK_SIZE)
#define __BLOCKS(x) ((x+__BLOCK_SIZE-1)/__BLOCK_SIZE)
#define __BLOCKSIZE(x) ((x+__BLOCK_SIZE-1)&~(__BLOCK_SIZE-1))
#define __BLOCK_GET_SIZE(x)    ((x & 0x7FFF))
#define __BLOCK_GET_PREV(x)    ((x >> 15) & 0x7FFF)
#define __BLOCK_GET_FREE(x)    ((x >> 31))
#define __BLOCK_GET_BLOCK(x)   ((x >> 30) & 0x1)
#define __BLOCK_SET_SIZE(x,y)  x=((x & ~0x7FFF) | ((y) & 0x7FFF))
#define __BLOCK_ADD_SIZE(x,y)  x=((x & ~0x7FFF) | (((x & 0x7FFF)+((y) & 0x7FFF)) & 0x7FFF))
#define __BLOCK_SET_PREV(x,y)  x=((x & ~0x3FFF8000) | (((y) & 0x7FFF)<<15))
#define __BLOCK_SET_FREE(x,y)  x=((x & 0x7FFFFFFF) | (((y) & 0x1)<<31))
#define __BLOCK_SET_BLOCK(x,y) x=((x & 0xBFFFFFFF) | (((y) & 0x1)<<30))
#define __BLOCK_MAKE(s,p,f,n)   (((f & 0x1)<<31) | ((n & 0x1)<<30) | (((p) & 0x7FFF)<<15) | ((s) & 0x7FFF))
#define __BLOCK_GET_FREEBLOCK(x) ((x>>30) & 0x3)//returns 11b if block is a starting block and free, 10b if block is a starting block and allocated, 0xb if it is a non-starting block (don't change)
#define __BLOCK0 ((__MEM_BLOCKS) | (1<<31) | (1<<30))

unsigned int	__mem_blocks[__MEM_BLOCKS] = { 0 };

static int __largest_update = 0;
static int __largest_block = __MEM_BLOCKS;
static int __mem_free = __MEM_BLOCKS;

inline void* vrelptr(void* ptr)
{
	return (void*)((unsigned int)ptr & ~__MEM_START);
}

inline void* vabsptr(void* ptr)
{
	return (void*)((unsigned int)ptr | __MEM_START);
}

static void __find_largest_block()
{
	int i = 0;
	__largest_block = 0;
	while (i < __MEM_BLOCKS)
	{
		int csize = __BLOCK_GET_SIZE(__mem_blocks[i]);
		if (__BLOCK_GET_FREEBLOCK(__mem_blocks[i]) == 3 && csize > __largest_block)
			__largest_block = csize;
		i += csize;
	}
	__largest_update = 0;
}

#ifdef _DEBUG
void __memwalk()
{
	int i = 0;
	if (__mem_blocks[0] == 0) __mem_blocks[0] = __BLOCK0;
	while (i < __MEM_BLOCKS)
	{
		printf("BLOCK %i:\n", i);
		printf("  free: %i\n", __BLOCK_GET_FREEBLOCK(__mem_blocks[i]));
		printf("  size: %i\n", __BLOCK_GET_SIZE(__mem_blocks[i]));
		printf("  prev: %i\n", __BLOCK_GET_PREV(__mem_blocks[i]));
		i += __BLOCK_GET_SIZE(__mem_blocks[i]);
	}
}
#endif

void* valloc(size_t size)
{
	// Initialize memory block, if not yet done
	if (__mem_blocks[0] == 0) __mem_blocks[0] = __BLOCK0;

	int i = 0;
	int j = 0;
	int bsize = __BLOCKS(size);

	if (__largest_update == 0 && __largest_block < bsize)
	{
		printf("Not enough memory to allocate %i bytes (largest: %i)!\n", size, vlargestblock());
		return(0);
	}

	printf("allocating %i bytes, in %i blocks\n", size, bsize);

	// Find smallest block that still fits the requested size
	int bestblock = -1;
	int bestblock_prev = 0;
	int bestblock_size = __MEM_BLOCKS + 1;
	while (i < __MEM_BLOCKS)
	{
		int csize = __BLOCK_GET_SIZE(__mem_blocks[i]);
		if (__BLOCK_GET_FREEBLOCK(__mem_blocks[i]) == 3 && csize >= bsize)
		{
			if (csize < bestblock_size)
			{
				bestblock = i;
				bestblock_prev = j;
				bestblock_size = csize;
			}

			if (csize == bsize)
				break;
		}
		j = i;
		i += csize;
	}

	if (bestblock < 0)
	{
		printf("Not enough memory to allocate %i bytes (largest: %i)!\n", size, vlargestblock());
		return(0);
	}

	i = bestblock;
	j = bestblock_prev;
	int csize = bestblock_size;
	__mem_blocks[i] = __BLOCK_MAKE(bsize, j, 0, 1);

	int next = i + bsize;
	if (csize > bsize&& next < __MEM_BLOCKS)
	{
		__mem_blocks[next] = __BLOCK_MAKE(csize - bsize, i, 1, 1);
		int nextnext = i + csize;
		if (nextnext < __MEM_BLOCKS)
		{
			__BLOCK_SET_PREV(__mem_blocks[nextnext], next);
		}
	}

	__mem_free -= bsize;
	if (__largest_block == csize)		// if we just allocated from one of the largest blocks
	{
		if ((csize - bsize) > (__mem_free / 2))
			__largest_block = (csize - bsize);		// there can't be another largest block
		else
			__largest_update = 1;
	}
	return ((void*)(__MEM_START + (i * __BLOCK_SIZE)));
}

void vfree(void* ptr)
{
	if (ptr == 0) return;

	int block = ((unsigned int)ptr - __MEM_START) / __BLOCK_SIZE;
	if (block<0 || block>__MEM_BLOCKS)
	{
		printf("Block is out of range: %i (0x%x)\n", block, (int)ptr);
		return;
	}
	int csize = __BLOCK_GET_SIZE(__mem_blocks[block]);

	printf("freeing block %i (0x%x), size: %i\n", block, (int)ptr, csize);


	if (__BLOCK_GET_FREEBLOCK(__mem_blocks[block]) != 1 || csize == 0)
	{
#ifdef _DEBUG
		printf("Block was not allocated!\n");
#endif
		return;
	}

	__BLOCK_SET_FREE(__mem_blocks[block], 1); //Mark block as free
	__mem_free += csize;

	int next = block + csize;
	int prev = __BLOCK_GET_PREV(__mem_blocks[block]); //Merge with previous block if possible
	if (prev < block)
	{
		if (__BLOCK_GET_FREEBLOCK(__mem_blocks[prev]) == 3)
		{
			__BLOCK_ADD_SIZE(__mem_blocks[prev], csize);
			__BLOCK_SET_BLOCK(__mem_blocks[block], 0);//mark current block as inter block
			if (next < __MEM_BLOCKS)
				__BLOCK_SET_PREV(__mem_blocks[next], prev);
			block = prev;
		}
	}

	if (next < __MEM_BLOCKS)//Merge with next block if possible
	{
		if (__BLOCK_GET_FREEBLOCK(__mem_blocks[next]) == 3)
		{
			__BLOCK_ADD_SIZE(__mem_blocks[block], __BLOCK_GET_SIZE(__mem_blocks[next]));
			__BLOCK_SET_BLOCK(__mem_blocks[next], 0);//mark next block as inter block
			int nextnext = next + __BLOCK_GET_SIZE(__mem_blocks[next]);
			if (nextnext < __MEM_BLOCKS)
				__BLOCK_SET_PREV(__mem_blocks[nextnext], block);
		}
	}

	if (__largest_block < __BLOCK_GET_SIZE(__mem_blocks[block])) //Update if a new largest block emerged
	{
		__largest_block = __BLOCK_GET_SIZE(__mem_blocks[block]);
		__largest_update = 0;//No update necessary any more, because update only necessary when largest has shrinked at most
	}
}

size_t vmemavail()
{
	return __mem_free * __BLOCK_SIZE;
}

size_t vlargestblock()
{
	if (__largest_update) __find_largest_block();
	return __largest_block * __BLOCK_SIZE;
}