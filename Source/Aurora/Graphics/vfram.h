#ifndef vram_h__
#define vram_h__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
void* vrelptr( void *ptr );		// make a pointer relative to memory base address (ATTENTION: A NULL rel ptr is not illegal/invalid!)
void* vabsptr( void *ptr );		// make a pointer absolute (default return type of valloc)

void* valloc( size_t size );
void vfree( void* ptr );
size_t vmemavail();
size_t vlargestblock();

#ifdef _DEBUG
void __memwalk();
#endif
#ifdef __cplusplus
}
#endif
#endif