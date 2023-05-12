#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/array.h"

#include "bitvector.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

/* Malloc a buffer of size + alignment bytes and returns the aligned part.
The offset between the real pointer and returned value was stored in p[-1].
*/
static inline void *pg_aligned_malloc(size_t alignment, size_t size) {
    void *p;
    void *porg;
    assert(alignment <= 256);
    porg = palloc(size + alignment);
    p = (void *)((((uint64)porg + alignment) / alignment) * alignment);
    *((unsigned char *)p-1) = (unsigned char)((uint64)p - (uint64)porg);
    return p;
}

static inline void pg_aligned_free(void *memblock) {
    void *porg;
    if (memblock == NULL)
        return;
    porg = (void *)((uint64)memblock - *((unsigned char *)memblock-1));
    if (porg == memblock)
        porg = (void *)((uint64)porg - 256);
    pfree(porg);
}

/*
 * Redefine standard memory allocation interface to pgsql's one.
*/
#ifdef malloc
#undef malloc
#endif
#define malloc(a)        palloc(a)

#ifdef calloc
#undef calloc
#endif
#define calloc(a, b)        palloc0((a) * (b))

#ifdef realloc
#undef realloc
#endif
#define realloc(a, b)    ((a)==NULL ? palloc(b) : repalloc((a),(b)))

#ifdef free
#undef free
#endif
#define free(a)            ((a)==NULL ? free(a) : pfree(a))

#define psql_masc_aligned_malloc(a,b)  pg_aligned_malloc((a),(b))
#define psql_masc_aligned_free(a)  pg_aligned_free(a)

#include "bitvector.cpp"

