/*
 * mm.c
 * Name: 刘沛雨
 * ID: 2100012289
 * @brief - a general purpose dynamic virtual memory allocator
 * Strategy: Segreted implicit free list + First fit
 * Allocated block: Header + Payload + Padding
 * Free block: Header + Prev & Next list pointer + Footer
 * Free lists are divided into 11 groups according to different list sizes.
 * The allocator manipulates the lists by handling 11 root nodes and commits
 * to a LIFO strategy.
 *
 */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
#define DEBUGx
#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif
/* def DRIVER */

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(p) (((size_t)(p) + (ALIGNMENT - 1)) & ~0x7)

/* Basic constants and macros */
#define WSIZE 4             /* Word and header/footer size (bytes) */
#define DSIZE 8             /* Double word size (bytes) */
#define CHUNKSIZE (1 << 12) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Read the previously allocated fields from address p */
#define GET_PREV_ALLOC(p) (GET(p) & 0x2)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp)-WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp)-WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp)-DSIZE))) // 需要脚部

/* Additonal macro */
#define REL_DIS(bp) ((char *)(bp) - (heap_listp))
#define LIST_NUM 11

/* Global variables */
static char *heap_listp = 0; /* Pointer to first block */

/* Pointers to first node */
static char *size16_63root = 0;
static char *size64_255root = 0;
static char *size256_1023root = 0;
static char *size1024_2047root = 0;
static char *size2048_4095root = 0;
static char *size4096_8191root = 0;
static char *size8192_16383root = 0;
static char *size16384_32767root = 0;
static char *size32768_65535root = 0;
static char *size65536_131071root = 0;
static char *size131072_infroot = 0;

/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);

static inline size_t adjust_size(size_t size);
static inline void insert_node(const void *bp);
static inline void _insert_node(const void *bp, char **root);
static inline void delete_node(const void *bp);
static inline void _delete_node(const void *bp, char **root);
static inline void edit_node(const void *bp, size_t csize);
static inline void _edit_node(const void *bp, char **root, size_t csize);

static void info(const void *bp);
static void check_prologue(const void *bp, int lineno);
static void check_alignment(const void *bp, int lineno);
static void check_header_footer(const void *prev, const void *bp, int lineno);
static void check_coalescing(const void *prev, const void *bp, int lineno);
static void check_epilogue(const void *bp, int lineno);
static void check_count(int free_cnt1, int free_cnt2, int lineno);
static void check_range(const void *bp, int lineno);
static void check_pointer(const void *prev, const void *bp, int lineno);
static void check_size(const void *bp, int index, int lineno);

static inline char **num2list(int index);
static inline int list2num(const char *root);
static inline unsigned get_list_size(int index);

/*
 * mm_init - Initialize VM and acquire some free space.
 * Initialize: return -1 on error, 0 on success.
 */
int mm_init(void) {
  /* Create the initial empty heap */
  if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
    return -1;
  PUT(heap_listp, 0);                            /* Alignment padding */
  PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); /* Prologue header */
  PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
  PUT(heap_listp + (3 * WSIZE), PACK(0, 3));     /* Epilogue header */
  heap_listp += (2 * WSIZE);

  /* Initialize all list pointers */
  size16_63root = 0;
  size64_255root = 0;
  size256_1023root = 0;
  size1024_2047root = 0;
  size2048_4095root = 0;
  size4096_8191root = 0;
  size8192_16383root = 0;
  size16384_32767root = 0;
  size32768_65535root = 0;
  size65536_131071root = 0;
  size131072_infroot = 0;

  /* Extend the empty heap with a free block of CHUNKSIZE bytes */
  if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
    return -1;
  return 0;
}

/*
 * malloc - Allocate a block with at least size bytes of payload
 */
void *malloc(size_t size) {
#ifdef DEBUG
  mm_checkheap(__LINE__);
#endif

  size_t asize;      /* Adjusted block size */
  size_t extendsize; /* Amount to extend heap if no fit */
  char *bp;

  if (heap_listp == 0) {
    mm_init();
  }
  /* Ignore spurious requests */
  if (size == 0)
    return NULL;

  /* Modify size to get better performance */
  size = adjust_size(size);

  /* Adjust block size to include overhead and alignment reqs. */
  if (size <= 3 * WSIZE)
    asize = 2 * DSIZE;
  else
    asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

  /* Search the free list for a fit */
  if ((bp = find_fit(asize)) != NULL) {
    place(bp, asize);
#ifdef DEBUG
    mm_checkheap(__LINE__);
#endif
    return bp;
  }

  /* No fit found. Get more memory and place the block */
  extendsize = MAX(asize, CHUNKSIZE);
  if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
    return NULL;
  place(bp, asize);
#ifdef DEBUG
  mm_checkheap(__LINE__);
#endif
  return bp;
}

/*
 * free - Free a block
 */
void free(void *bp) {
#ifdef DEBUG
  mm_checkheap(__LINE__);
#endif

  void *head;

  if (bp == 0)
    return;

  head = HDRP(bp);
  size_t size = GET_SIZE(head);
  if (heap_listp == 0) {
    mm_init();
  }

  PUT(head, PACK(size, GET_PREV_ALLOC(head)));
  PUT(FTRP(bp), PACK(size, 0));

  head = HDRP(NEXT_BLKP(bp));
  PUT(head, PACK(GET_SIZE(head), GET_ALLOC(head))); 

  coalesce(bp);
#ifdef DEBUG
  mm_checkheap(__LINE__);
#endif
}

/*
 * realloc - Reallocate a block and retain previous block data.
 */
void *realloc(void *oldptr, size_t size) {
  size_t oldsize;
  void *newptr;

  /* If size == 0 then this is just free, and we return NULL. */
  if (size == 0) {
    free(oldptr);
    return 0;
  }

  /* If oldptr is NULL, then this is just malloc. */
  if (oldptr == NULL) {
    return malloc(size);
  }

  newptr = malloc(size);

  /* If realloc() fails the original block is left untouched  */
  if (!newptr) {
    return 0;
  }

  /* Copy the old data. */
  oldsize = GET_SIZE(HDRP(oldptr));
  if (size < oldsize)
    oldsize = size;
  memcpy(newptr, oldptr, oldsize);

  /* Free the old block. */
  free(oldptr);

  return newptr;
}

/*
 * calloc - Allocate a block that is initialized with 0
 */
void *calloc(size_t nmemb, size_t size) {
  size_t bytes = nmemb * size;
  void *newptr;

  newptr = malloc(bytes);
  memset(newptr, 0, bytes);

  return newptr;
  return NULL;
}

/*
 * The remaining routines are internal helper routines
 */

/*
 * extend_heap - Extend heap with free block and return its block pointer
 */
static void *extend_heap(size_t words) {
  char *bp;
  size_t size;
  void *head;

  /* Allocate an even number of words to maintain alignment */
  size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
  if ((long)(bp = mem_sbrk(size)) == -1)
    return NULL;

  /* Initialize free block header/footer and the epilogue header */
  head = HDRP(bp);
  PUT(head, PACK(size, GET_PREV_ALLOC(head)));
  /* Free block header */     
  PUT(FTRP(bp), PACK(size, 0)); /* Free block footer */
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
  /* New epilogue header */ 

  /* Coalesce if the previous block was free */
  return coalesce(bp);
}

/*
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block
 */
static void *coalesce(void *bp) {
  size_t prev_alloc = GET_PREV_ALLOC(HDRP(bp));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));

  if (prev_alloc && next_alloc) { /* Case 1 */
    insert_node(bp);
    return bp;
  }

  else if (prev_alloc && !next_alloc) { /* Case 2 */
    delete_node(NEXT_BLKP(bp));
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(bp), PACK(size, 2));
    PUT(FTRP(bp), PACK(size, 0));
  }

  else if (!prev_alloc && next_alloc) { /* Case 3 */
    delete_node(PREV_BLKP(bp));
    size += GET_SIZE(HDRP(PREV_BLKP(bp)));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, GET_PREV_ALLOC(HDRP(PREV_BLKP(bp)))));
    bp = PREV_BLKP(bp);
  }

  else { /* Case 4 */
    delete_node(NEXT_BLKP(bp));
    delete_node(PREV_BLKP(bp));
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, GET_PREV_ALLOC(HDRP(PREV_BLKP(bp)))));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  }
  insert_node(bp);
  return bp;
}

/*
 * place - Place block of asize bytes at start of free block bp
 *         and split if remainder would be at least minimum block size
 */
static void place(void *bp, size_t asize) {
  size_t csize = GET_SIZE(HDRP(bp));
  void *p, *nextbp;

  if ((csize - asize) >= (4 * WSIZE)) { //需要分割，最小块:4字
    p = HDRP(bp);
    PUT(p, PACK(asize, GET_PREV_ALLOC(p) | 1));
    //维护显式链表
    edit_node(bp, csize);
    //修改下一块(仍为空闲块)的prev
    nextbp = NEXT_BLKP(bp);
    PUT(HDRP(nextbp), PACK(csize - asize, 2));
    PUT(FTRP(nextbp), PACK(csize - asize, 0));
  } else { //不需要分割
    p = HDRP(bp);
    PUT(p, PACK(csize, GET_PREV_ALLOC(p) | 1));
    //维护显式链表
    delete_node(bp);
    //修改下一块（可能分配，可能空闲）的prev
    nextbp = NEXT_BLKP(bp);
    p = HDRP(nextbp);
    PUT(p, PACK(GET_SIZE(p), GET_ALLOC(p) | 2));
  }
}

/*
 * find_fit - Find a fit for a block with asize bytes
 */
static void *find_fit(size_t asize) 
{
  /* First-fit search */
  void *bp;
  char *root;
  unsigned list_size;
  for (int i = 0; i < LIST_NUM; i++) {
    list_size = get_list_size(i);
    if (asize <= list_size) {
      root = *num2list(i);
      if (root == 0)
        continue;
      for (bp = root; GET(bp + 4) > 0; bp = heap_listp + GET(bp + 4))
        if (GET_SIZE(HDRP(bp)) > 0 && !GET_ALLOC(HDRP(bp)) &&
            (asize <= GET_SIZE(HDRP(bp))))
          return bp;
      /* Judge the last node specifically */
      if (GET_SIZE(HDRP(bp)) > 0 && !GET_ALLOC(HDRP(bp)) &&
          (asize <= GET_SIZE(HDRP(bp))))
        return bp;
    }
  }
  return NULL; /* No fit */
}

/*
 * adjust_size - Modify block size to get better performance
 * I really don't know why it works. It's totally set by expierence.
 */
static inline size_t adjust_size(size_t size) {
  if (size >= 2000 && size<2048)
    return 2048;
  if (size>=1000 && size<1024)
    return 1024;
  else if (size >= 440 && size < 512)
    return 512;
  else if (size >= 240 && size < 256)
    return 256;
  else if (size >= 100 && size < 128)
    return 128;
  else
    return size;
}

/*
 * Return whether the pointer is in the heap.
 * May be useful for debugging.
 */
static int in_heap(const void *p) {
  return p <= mem_heap_hi() && p >= mem_heap_lo();
}

/*
 * Return whether the pointer is aligned.
 * May be useful for debugging.
 */
static int aligned(const void *p) { return (size_t)ALIGN(p) == (size_t)p; }

/*
 * mm_checkheap - Check heap and list consistency
 */

void mm_checkheap(int lineno) {
  void *bp, *prev;
  int cnt, free_cnt1, free_cnt2;

  cnt = 0;
  free_cnt1 = 0;
  /* Check heap */
  bp = heap_listp;
  check_prologue(bp, lineno);
  prev = heap_listp;
  bp = NEXT_BLKP(bp);
  cnt++;
  for (; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
    if (!GET_ALLOC(HDRP(bp)))
      free_cnt1++;
    check_alignment(bp, lineno);
    check_header_footer(prev, bp, lineno);
    check_coalescing(prev, bp, lineno);
    prev = bp;
    cnt++;
  }
  check_epilogue(bp, lineno);

  /* Check list */
  free_cnt2 = 0;
  for (int i = 0; i < LIST_NUM; i++) {
    bp = *num2list(i);
    if (bp) {
      cnt = 0;
      check_range(bp, lineno);
      check_size(bp, i, lineno);
      cnt++;
      free_cnt2++;
      while (GET(bp + 4) > 0) {
        prev = bp;
        bp = heap_listp + GET(bp + 4);
        check_range(bp, lineno);
        check_pointer(prev, bp, lineno);
        cnt++;
        free_cnt2++;
      }
    }
  }
  check_count(free_cnt1, free_cnt2, lineno);
}

/*
 * info - Print block information
 * It's useful when debug. 
 */
static void info(const void *bp) {
  printf("    Address: %p\n", HDRP(bp));
  printf("    Block size: %d\n", GET_SIZE(HDRP(bp)));
  printf("    Block alloc: %d\n", GET_ALLOC(HDRP(bp)));
  printf("    Block prev_alloc: %d\n", GET_PREV_ALLOC(HDRP(bp)));
  if (!GET_ALLOC(HDRP(bp))) {
    printf("    Prev pointer: %p\n", heap_listp + GET(bp));
    printf("    Next pointer: %p\n", heap_listp + GET(bp + 4));
  }
}

/*
 * check_alignment - Check block alignment
 */
static void check_alignment(const void *bp, int lineno) {
  if (!aligned(bp)) {
    printf("Heap check at line %d, Heap_listp ALIGNMENT Error\n", lineno);
    exit(0);
  }
}

/*
 * check_prologue - Check prologue block
 */
static void check_prologue(const void *bp, int lineno) {
  void *head, *foot;
  check_alignment(bp, lineno);
  head = HDRP(bp);
  foot = FTRP(bp);
  if (GET_SIZE(head) != DSIZE) {
    printf("Heap check at line %d, Prologue Header SIZE Error\n", lineno);
    exit(0);
  }
  if (GET_ALLOC(head) != 1) {
    printf("Heap check at line %d, Prologue Header ALLOC Error\n", lineno);
    exit(0);
  }
  if (GET_SIZE(foot) != DSIZE) {
    printf("Heap check at line %d, Prologue Footer SIZE Error\n", lineno);
    exit(0);
  }
  if (GET_ALLOC(foot) != 1) {
    printf("Heap check at line %d, Prologue Footer ALLOC Error\n", lineno);
    exit(0);
  }
}

/*
 * check_epilogue - Check epilogue block
 */
static void check_epilogue(const void *bp, int lineno) {
  void *head;
  check_alignment(bp, lineno);
  head = HDRP(bp);
  if (GET_SIZE(head) != 0) {
    printf("Heap check at line %d, Epilogue Header SIZE Error\n", lineno);
    exit(0);
  }
  if (GET_ALLOC(head) != 1) {
    printf("Heap check at line %d, Epilogue Header ALLOC Error\n", lineno);
    exit(0);
  }
}

/*
 * check_header_footer - Check block header and foot (if the block is free)
 */
static void check_header_footer(const void *prev, const void *bp, int lineno) {
  if (GET_SIZE(HDRP(bp)) < 4 * WSIZE) {
    printf("Heap check at line %d, Block Header SIZE Error\n", lineno);
    exit(0);
  }
  if (GET_SIZE(HDRP(bp)) % 2 != 0) {
    printf("Heap check at line %d, Block Header SIZE Error\n", lineno);
    exit(0);
  }
  if (GET_ALLOC(HDRP(prev)) != (GET_PREV_ALLOC(HDRP(bp)) >> 1)) {
    printf("Heap check at line %d, Block Header PREV_ALLOC Error\n", lineno);
    exit(0);
  }
  if (GET_ALLOC(HDRP(bp)) == 0) {
    if (GET_SIZE(HDRP(bp)) != GET_SIZE(FTRP(bp))) {
      printf("Heap check at line %d, Block Footer SIZE Error\n", lineno);
      exit(0);
    }
    if (GET_ALLOC(HDRP(bp)) != GET_ALLOC(FTRP(bp))) {
      printf("Heap check at line %d, Block Footer ALLOC Error\n", lineno);
      exit(0);
    }
  }
}

/*
 * check_coalescing - Check block coalescing consistency
 */
static void check_coalescing(const void *prev, const void *bp, int lineno) {
  if (GET_ALLOC(HDRP(prev)) == 0 && GET_ALLOC(HDRP(bp)) == 0) {
    printf("Heap check at line %d, Block Coalescing Error\n", lineno);
    exit(0);
  }
}

/*
 * check_range - Check block pointer range
 */
static void check_range(const void *bp, int lineno) {
  if (!in_heap(HDRP(bp))) {
    printf("Heap check at line %d, Block Range Error\n", lineno);
    exit(0);
  }
}

/*
 * check_pointer - Check list pointer consistency
 */
static void check_pointer(const void *prev, const void *bp, int lineno) {
  if (heap_listp + GET(prev + 4) != bp) {
    printf("Heap check at line %d, Pointer Error\n", lineno);
    exit(0);
  }
  if (heap_listp + GET(bp) != prev) {
    printf("Heap check at line %d, Pointer Error\n", lineno);
    exit(0);
  }
}

/*
 * check_count - Check free block count consistency
 */
static void check_count(int free_cnt1, int free_cnt2, int lineno) {
  if (free_cnt1 != free_cnt2) {
    printf("Heap check at line %d, Free Block Count Error\n", lineno);
    exit(0);
  }
}

/*
 * check_count - Check free block count consistency
 */
static void check_size(const void *bp, int index, int lineno) {
  unsigned block_size, list_size;
  block_size = GET_SIZE(HDRP(bp));
  list_size = get_list_size(index);
  if (index <= 2) {
    if (block_size > list_size || block_size < (list_size + 1) / 4) {
      printf("Heap check at line %d, Block&List Size Error\n", lineno);
      exit(0);
    }
  } else {
    if (block_size > list_size || block_size < (list_size + 1) / 2) {
      printf("Heap check at line %d, Block&List Size Error\n", lineno);
      exit(0);
    }
  }
}

/*
 * list operations
 */

/*
 * insert_node & _insert_node - Insert node to the root of a specific list
 */
static inline void insert_node(const void *bp) {
  unsigned int size = GET_SIZE(HDRP(bp));
  if (size >= 16 && size < 64)
    _insert_node(bp, &size16_63root);
  else if (size >= 64 && size < 256)
    _insert_node(bp, &size64_255root);
  else if (size >= 256 && size < 1024)
    _insert_node(bp, &size256_1023root);
  else if (size >= 1024 && size < 2048)
    _insert_node(bp, &size1024_2047root);
  else if (size >= 2048 && size < 4096)
    _insert_node(bp, &size2048_4095root);
  else if (size >= 4096 && size < 8192)
    _insert_node(bp, &size4096_8191root);
  else if (size >= 8192 && size < 16384)
    _insert_node(bp, &size8192_16383root);
  else if (size >= 16384 && size < 32768)
    _insert_node(bp, &size16384_32767root);
  else if (size >= 32768 && size < 65536)
    _insert_node(bp, &size32768_65535root);
  else if (size >= 65536 && size < 131072)
    _insert_node(bp, &size65536_131071root);
  else
    _insert_node(bp, &size131072_infroot);
}
static inline void _insert_node(const void *bp, char **root) {
  if (*root) {
    PUT(*root, REL_DIS(bp));
    PUT(bp, 0);
    PUT(bp + WSIZE, REL_DIS(*root));
    *root = (char *)bp;
  } else {
    *root = (char *)bp;
    PUT(bp, 0);
    PUT(bp + WSIZE, 0);
  }
}

/*
 * delete_node & _delete_node - Delete a node in a specific list
 */
static inline void delete_node(const void *bp) {
  unsigned int size = GET_SIZE(HDRP(bp));
  if (size >= 16 && size < 64)
    _delete_node(bp, &size16_63root);
  else if (size >= 64 && size < 256)
    _delete_node(bp, &size64_255root);
  else if (size >= 256 && size < 1024)
    _delete_node(bp, &size256_1023root);
  else if (size >= 1024 && size < 2048)
    _delete_node(bp, &size1024_2047root);
  else if (size >= 2048 && size < 4096)
    _delete_node(bp, &size2048_4095root);
  else if (size >= 4096 && size < 8192)
    _delete_node(bp, &size4096_8191root);
  else if (size >= 8192 && size < 16384)
    _delete_node(bp, &size8192_16383root);
  else if (size >= 16384 && size < 32768)
    _delete_node(bp, &size16384_32767root);
  else if (size >= 32768 && size < 65536)
    _delete_node(bp, &size32768_65535root);
  else if (size >= 65536 && size < 131072)
    _delete_node(bp, &size65536_131071root);
  else
    _delete_node(bp, &size131072_infroot);
}
static inline void _delete_node(const void *bp, char **root) {
  char *prev, *next;
  prev = heap_listp + GET(bp);
  next = heap_listp + GET(bp + WSIZE);
  if (prev != heap_listp) {
    PUT(prev + WSIZE, GET(bp + WSIZE));
    if (next != heap_listp)
      PUT(next, GET(bp));
  } else {
    if (next != heap_listp) {
      PUT(next, 0);
      *root = next;
    } else
      *root = 0;
  }
}

/*
 * edit_node & _edit_node - Edit node in a specific list
 */
static inline void edit_node(const void *bp, size_t csize) {
  if (csize >= 16 && csize < 64)
    _edit_node(bp, &size16_63root, csize);
  else if (csize >= 64 && csize < 256)
    _edit_node(bp, &size64_255root, csize);
  else if (csize >= 256 && csize < 1024)
    _edit_node(bp, &size256_1023root, csize);
  else if (csize >= 1024 && csize < 2048)
    _edit_node(bp, &size1024_2047root, csize);
  else if (csize >= 2048 && csize < 4096)
    _edit_node(bp, &size2048_4095root, csize);
  else if (csize >= 4096 && csize < 8192)
    _edit_node(bp, &size4096_8191root, csize);
  else if (csize >= 8192 && csize < 16384)
    _edit_node(bp, &size8192_16383root, csize);
  else if (csize >= 16384 && csize < 32768)
    _edit_node(bp, &size16384_32767root, csize);
  else if (csize >= 32768 && csize < 65536)
    _edit_node(bp, &size32768_65535root, csize);
  else if (csize >= 65536 && csize < 131072)
    _edit_node(bp, &size65536_131071root, csize);
  else
    _edit_node(bp, &size131072_infroot, csize);
}
static inline void _edit_node(const void *bp, char **root, size_t csize) {
  void *nextbp;
  unsigned next_size, list_size;

  nextbp = NEXT_BLKP(bp);
  PUT(nextbp, GET(bp));
  PUT(nextbp + WSIZE, GET(bp + WSIZE));

  next_size = csize - GET_SIZE(HDRP(bp));
  list_size = get_list_size(list2num(*root));

  /* The new node may need to placed into another list */
  int bound = list2num(*root);
  _delete_node(bp, root);
  for (int i = 0; i <= bound; i++) {
    if (next_size <= get_list_size(i)) {
      _insert_node(nextbp, num2list(i));
      return;
    }
  }
}

/*
 * num2list - Map a number to a list
 */
static inline char **num2list(int index) {
  switch (index) {
  case 0:
    return &size16_63root;
  case 1:
    return &size64_255root;
  case 2:
    return &size256_1023root;
  case 3:
    return &size1024_2047root;
  case 4:
    return &size2048_4095root;
  case 5:
    return &size4096_8191root;
  case 6:
    return &size8192_16383root;
  case 7:
    return &size16384_32767root;
  case 8:
    return &size32768_65535root;
  case 9:
    return &size65536_131071root;
  case 10:
    return &size131072_infroot;
  }
  return NULL;
}


/*
 * num2list - Map a list to a number
 */
static inline int list2num(const char *root) {
  if (root == size16_63root)
    return 0;
  if (root == size64_255root)
    return 1;
  if (root == size256_1023root)
    return 2;
  if (root == size1024_2047root)
    return 3;
  if (root == size2048_4095root)
    return 4;
  if (root == size4096_8191root)
    return 5;
  if (root == size8192_16383root)
    return 6;
  if (root == size16384_32767root)
    return 7;
  if (root == size32768_65535root)
    return 8;
  if (root == size65536_131071root)
    return 9;
  if (root == size131072_infroot)
    return 10;
  return -1;
}

/*
 * get_list_size - Calculate size of a specific list
 */
static inline unsigned get_list_size(int index) {
  if (index >= 0 && index <= 2)
    return 64 * (1 << (2 * index)) - 1;
  else if (index >= 3 && index < LIST_NUM - 1)
    return (1 << (index + 8)) - 1;
  else if (index == LIST_NUM - 1)
    return (unsigned)(-1);
  return 0;
}