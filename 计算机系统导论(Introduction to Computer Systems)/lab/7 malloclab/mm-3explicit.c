/*
 * mm.c
 *
 * explicit
 */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
#define DEBUG
#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
# define dbg_printf(...)
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

/*
 * If NEXT_FIT defined use next fit search, else use first-fit search 
 */
#define NEXT_FITx

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(p) (((size_t)(p) + (ALIGNMENT-1)) & ~0x7)

/* Basic constants and macros */
#define WSIZE       4       /* Word and header/footer size (bytes) */ 
#define DSIZE       8       /* Double word size (bytes) */
#define CHUNKSIZE  (1<<12)  /* Extend heap by this amount (bytes) */  

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc)) 

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))            
#define PUT(p, val)  (*(unsigned int *)(p) = (val))    

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)                   
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Read the previously allocated fields from address p */
#define GET_PREV_ALLOC(p) (GET(p) & 0x2)                    

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)                      
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) 

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) 
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) //需要脚部

/* Additonal micro */
#define REL_DIS(bp)  ((char*)(bp)-(heap_listp))

/* Global variables */
static char *heap_listp = 0;  /* Pointer to first block */  
static char *root = 0;
#ifdef NEXT_FIT
static char *rover;           /* Next fit rover */
#endif

/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void insert_node(const void *bp);
static void delete_node(const void *bp);
static void edit_node(const void *bp);

/*
 * Initialize: return -1 on error, 0 on success.
 */
int mm_init(void) {
    // printf("mm_init called\n");
    /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) 
        return -1;
    PUT(heap_listp, 0);                          /* Alignment padding */
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */ 
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */ 
    PUT(heap_listp + (3*WSIZE), PACK(0, 3));     /* Epilogue header */
    heap_listp += (2*WSIZE);                 
    root =  0;   //初始化显式链表头节点  
    // mm_checkheap(__LINE__); 

#ifdef NEXT_FIT
    rover = heap_listp;
#endif

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL) 
        return -1;
    // mm_checkheap(__LINE__);
    return 0;
}

/*
 * malloc
 */
void *malloc (size_t size) {
    // printf("malloc called\n");
    size_t asize;      /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;    

    if (heap_listp == 0){
        mm_init();
    }
    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= 3 * WSIZE)                                          
        asize = 2 * DSIZE;                                        
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE); 
    
    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {  
        // printf("bp:%p\n",bp);
        // mm_checkheap(__LINE__); 
        place(bp, asize);
        // mm_checkheap(__LINE__);         
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize,CHUNKSIZE);                 
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)  
        return NULL;                                  
    // mm_checkheap(__LINE__); 
    place(bp, asize);
    // mm_checkheap(__LINE__);                            
    return bp;
}

/*
 * free
 */
void free (void *bp) {
    // printf("free called\n");
    void *head;

    if (bp == 0) 
        return;

    head = HDRP(bp);
    size_t size = GET_SIZE(head);
    if (heap_listp == 0){
        mm_init();
    }

    PUT(head, PACK(size, GET_PREV_ALLOC(head)));
    PUT(FTRP(bp), PACK(size, 0));

    head=HDRP(NEXT_BLKP(bp));
    PUT(head,PACK(GET_SIZE(head),GET_ALLOC(head))); //清除下一块的prev位

    // mm_checkheap(__LINE__);
    coalesce(bp);
    // mm_checkheap(__LINE__); 
}

/*
 * realloc - you may want to look at mm-naive.c
 */
void *realloc(void *oldptr, size_t size) {
    size_t oldsize;
    void *newptr;

    /* If size == 0 then this is just free, and we return NULL. */
    if(size == 0) {
        free(oldptr);
        return 0;
    }

    /* If oldptr is NULL, then this is just malloc. */
    if(oldptr == NULL) {
        return malloc(size);
    }

    newptr = malloc(size);

    /* If realloc() fails the original block is left untouched  */
    if(!newptr) {
        return 0;
    }

    /* Copy the old data. */
    oldsize = GET_SIZE(HDRP(oldptr));
    if(size < oldsize) oldsize = size;
    memcpy(newptr, oldptr, oldsize);

    /* Free the old block. */
    free(oldptr);

    return newptr;
}

/*
 * calloc - you may want to look at mm-naive.c
 * This function is not tested by mdriver, but it is
 * needed to run the traces.
 */
void *calloc (size_t nmemb, size_t size) {
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
static void *extend_heap(size_t words) 
{
    // printf("extend_heap called\n");
    char *bp;
    size_t size;
    void *head;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE; //双字对齐
    if ((long)(bp = mem_sbrk(size)) == -1)  
        return NULL;                                        

    /* Initialize free block header/footer and the epilogue header */
    head=HDRP(bp);
    PUT(head, PACK(size, GET_PREV_ALLOC(head)));         /* Free block header */   //保留原来结尾块的prev位
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */   
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */ //新结尾块的prev=0

    /* Coalesce if the previous block was free */
    return coalesce(bp);                                          
}

/*
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block
 */
static void *coalesce(void *bp) 
{
    // printf("coalesce called\n");
    size_t prev_alloc = GET_PREV_ALLOC(HDRP(bp));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {            /* Case 1 */
        insert_node(bp);
        return bp;
    }

    else if (prev_alloc && !next_alloc) {      /* Case 2 */
        //维护显式链表
        delete_node(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 2));
        PUT(FTRP(bp), PACK(size, 0));
    }

    else if (!prev_alloc && next_alloc) {      /* Case 3 */
        //维护显式链表
        delete_node(PREV_BLKP(bp));

        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), 
            PACK(size, GET_PREV_ALLOC(HDRP(PREV_BLKP(bp)))));

        bp = PREV_BLKP(bp);
    }

    else {                                     /* Case 4 */
        //维护显式链表
        delete_node(NEXT_BLKP(bp));
        delete_node(PREV_BLKP(bp));

        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
            GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), 
            PACK(size, GET_PREV_ALLOC(HDRP(PREV_BLKP(bp)))));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));

        bp = PREV_BLKP(bp);
    }
    //维护显式链表
    // printf("Called2\n");
    insert_node(bp);
#ifdef NEXT_FIT
    /* Make sure the rover isn't pointing into the free block */
    /* that we just coalesced */
    if ((rover > (char *)bp) && (rover < NEXT_BLKP(bp))) 
        rover = bp;
#endif
    return bp;
}

/* 
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 */
static void place(void *bp, size_t asize)
{
    // printf("place called\n");
    size_t csize = GET_SIZE(HDRP(bp));
    void *p,*nextbp; 
    
    if ((csize - asize) >= (4*WSIZE)) { //需要分割，最小块:4字
        // mm_checkheap(__LINE__); 
        p = HDRP(bp);
        PUT(p, PACK(asize, GET_PREV_ALLOC(p) | 1));
        //维护显式链表
        edit_node(bp);
        //修改下一块(仍为空闲块)的prev
        nextbp = NEXT_BLKP(bp);
        PUT(HDRP(nextbp), PACK(csize-asize, 2));
        PUT(FTRP(nextbp), PACK(csize-asize, 0));
    }
    else { //不需要分割
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
    // printf("find_fit called\n");
#ifdef NEXT_FIT 
    /* Next fit search */
    char *oldrover = rover;

    /* Search from the rover to the end of list */
    for ( ; GET_SIZE(HDRP(rover)) > 0; rover = NEXT_BLKP(rover))
        if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
            return rover;

    /* search from start of list to old rover */
    for (rover = heap_listp; rover < oldrover; rover = NEXT_BLKP(rover))
        if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
            return rover;

    return NULL;  /* no fit found */
#else 
    /* First-fit search */
    void *bp;
    //搜索显式链表而非块链表
    if(root==0)
        return NULL;
    for (bp = root; GET(bp+4) > 0; bp = heap_listp+GET(bp+4)) {
        // printf("next_bp = %p\n",heap_listp+GET(bp+4));
        if (GET_SIZE(HDRP(bp)) > 0 && !GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
            return bp;
        }
    }
    // mm_checkheap(__LINE__);
    //特判最后一个节点
    if (GET_SIZE(HDRP(bp)) > 0 && !GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
        return bp;
    }
    return NULL; /* No fit */
#endif
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
static int aligned(const void *p) {
    return (size_t)ALIGN(p) == (size_t)p;
}

/*
 * mm_checkheap
 */

void info(const void* bp);
void checkPrologue(const void *bp, int lineno);
void checkAlignment(const void *bp, int lineno);
void checkHeaderFooter(const void *prev, const void *bp, int lineno);
void checkCoalescing(const void *prev, const void *bp, int lineno);
void checkEpilogue(const void *bp, int lineno);

void checkCount(int free_cnt1,int free_cnt2,int lineno);
void checkRange(const void *bp,int lineno);
void checkPointer(const void *prev,const void *bp,int lineno);

void mm_checkheap(int lineno) {
    printf("Heap check (at line %d) start:\n",lineno);
    void *bp, *prev;
    int cnt,free_cnt1,free_cnt2;
    
    cnt=0;
    free_cnt1=0;
    //1.Check heap
    printf("1.Check heap...\n");
    printf("block %d:\n",cnt);
    bp = heap_listp;
    info(bp);
    checkPrologue(bp,lineno);
    prev = heap_listp;
    bp = NEXT_BLKP(bp);
    cnt++;
    for (; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        printf("block %d:\n",cnt);
        info(bp);
        if(!GET_ALLOC(HDRP(bp)))
            free_cnt1++;
        checkAlignment(bp, lineno);
        checkHeaderFooter(prev,bp,lineno);
        checkCoalescing(prev,bp,lineno);
        prev = bp;
        cnt++;
    }
    printf("block %d:\n",cnt);
    info(bp);
    checkEpilogue(bp, lineno);

    //2.Check list
    printf("2.Check list...\n");
    bp=root;
    // printf("bp=%p\n",bp);
    free_cnt2=0;
    if(!bp){
        printf("Empty list\n");
    }
    else{
        cnt=0;
        // printf("free block %d:\n",cnt);
        // info(bp);
        checkRange(bp,lineno);
        cnt++;
        free_cnt2++;
        while(GET(bp+4)>0){
            prev=bp;
            bp = heap_listp+GET(bp+4);
            // printf("free block %d:\n",cnt);
            // info(bp);
            checkRange(bp,lineno);
            checkPointer(prev,bp,lineno);
            cnt++;
            free_cnt2++;
        }
    }
    checkCount(free_cnt1,free_cnt2,lineno);
    printf("Heap check (at line %d) end!\n",lineno);
}

void info(const void* bp){
    printf("    Address: %p\n",HDRP(bp));
    printf("    Block size: %d\n",GET_SIZE(HDRP(bp)));
    printf("    Block alloc: %d\n",GET_ALLOC(HDRP(bp)));
    printf("    Block prev_alloc: %d\n",GET_PREV_ALLOC(HDRP(bp)));
    if(!GET_ALLOC(HDRP(bp))){
        printf("    Prev pointer: %p\n",heap_listp+GET(bp));
        printf("    Next pointer: %p\n",heap_listp+GET(bp+4));
    }
}

void checkAlignment(const void *bp, int lineno){
    if (!aligned(bp)){
        printf("Heap check at line %d, Heap_listp ALIGNMENT Error\n", lineno);
        exit(0);
    }
}

void checkPrologue(const void *bp, int lineno){
    void *head, *foot;
    checkAlignment(bp, lineno);
    head = HDRP(bp);
    foot = FTRP(bp);
    if (GET_SIZE(head) != DSIZE){
        printf("Heap check at line %d, Prologue Header SIZE Error\n", lineno);
        exit(0);
    }
    if (GET_ALLOC(head) != 1){
        printf("Heap check at line %d, Prologue Header ALLOC Error\n", lineno);
        exit(0);
    }
    if (GET_SIZE(foot) != DSIZE){
        printf("Heap check at line %d, Prologue Footer SIZE Error\n", lineno);
        exit(0);
    }
    if (GET_ALLOC(foot)!= 1){
        printf("Heap check at line %d, Prologue Footer ALLOC Error\n", lineno);
        exit(0);
    }
}

void checkEpilogue(const void *bp, int lineno){
    void *head;
    checkAlignment(bp, lineno);
    head = HDRP(bp);
    if (GET_SIZE(head) != 0){
        printf("Heap check at line %d, Epilogue Header SIZE Error\n", lineno);
        exit(0);
    }
    if (GET_ALLOC(head) != 1){
        printf("Heap check at line %d, Epilogue Header ALLOC Error\n", lineno);
        exit(0);
    }
}

void checkHeaderFooter(const void *prev, const void *bp, int lineno){
    if(GET_SIZE(HDRP(bp))<4*WSIZE){
        printf("Heap check at line %d, Block Header SIZE Error\n", lineno);
        exit(0);
    }
    if(GET_SIZE(HDRP(bp))%2!=0){
        printf("Heap check at line %d, Block Header SIZE Error\n", lineno);
        exit(0);
    }
    if(GET_ALLOC(HDRP(prev))!=(GET_PREV_ALLOC(HDRP(bp))>>1)){
        printf("Heap check at line %d, Block Header PREV_ALLOC Error\n", lineno);
        exit(0);
    }
    if(GET_ALLOC(HDRP(bp))==0){
        if(GET_SIZE(HDRP(bp))!=GET_SIZE(FTRP(bp))){
            printf("Heap check at line %d, Block Footer SIZE Error\n", lineno);
            exit(0);
        }
        if(GET_ALLOC(HDRP(bp))!=GET_ALLOC(FTRP(bp))){
            printf("Heap check at line %d, Block Footer ALLOC Error\n", lineno);
            exit(0);
        }
    }
}

void checkCoalescing(const void *prev, const void *bp, int lineno){
    if (GET_ALLOC(HDRP(prev))==0&&GET_ALLOC(HDRP(bp))==0) {
        printf("Heap check at line %d, Block Coalescing Error\n", lineno);
        exit(0);
    }
}

void checkRange(const void *bp, int lineno){
    if(!in_heap(HDRP(bp))){
        printf("Heap check at line %d, Block Range Error\n", lineno);
        exit(0);
    }
}

void checkPointer(const void *prev,const void *bp,int lineno){
    if(heap_listp+GET(prev+4)!=bp){
        printf("Heap check at line %d, Pointer Error\n", lineno);
        exit(0);
    }
    if(heap_listp+GET(bp)!=prev){
        printf("Heap check at line %d, Pointer Error\n", lineno);
        exit(0);
    }
}

void checkCount(int free_cnt1,int free_cnt2,int lineno){
    if(free_cnt1!=free_cnt2){
        printf("Heap check at line %d, Free Block Count Error\n", lineno);
        printf("free_cnt1 = %d, free_cnt2 = %d\n",free_cnt1,free_cnt2);
        exit(0);
    }
}

/*
 *  list operations
 */

 void insert_node(const void *bp){
    // mm_checkheap(__LINE__);
    // printf("insert_node called\n");
    if(root){
        // printf("root = %p\n",root);
        PUT(root,REL_DIS(bp));
        // printf("%d\n",GET(root));
        // printf("%d\n",GET(root+4));
        PUT(bp,0);
        // printf("%d\n",GET(bp));
        PUT(bp+WSIZE,REL_DIS(root));
        // printf("%d\n",GET(bp+WSIZE));
        root = (char*)bp;
        // printf("root = %p\n",root);
    }
    else{
        root = (char*)bp;
        PUT(bp,0);
        PUT(bp+WSIZE,0);
    }
    // mm_checkheap(__LINE__);
 }

 void delete_node(const void *bp){
    // mm_checkheap(__LINE__);
    // printf("delete_node called\n");
    char *prev,*next;
    prev = heap_listp+GET(bp);
    next = heap_listp+GET(bp+WSIZE);
    if (prev!=heap_listp) {
        PUT(prev+WSIZE,GET(bp+WSIZE));
        if (next!=heap_listp) 
            PUT(next,GET(bp));
    }
    else{
        if (next!=heap_listp) {
            PUT(next,0);
            root = next;
        } 
        else 
            root = 0;
    }
 }

 void edit_node(const void *bp){
    // printf("edit_node called\n");
    void *nextbp;
    nextbp = NEXT_BLKP(bp);
    PUT(nextbp,GET(bp));
    PUT(nextbp+WSIZE,GET(bp+WSIZE));
    if(GET(nextbp)!=0)
        PUT(heap_listp+GET(nextbp)+4,REL_DIS(nextbp));
    if(GET(nextbp+4)!=0)
        PUT(heap_listp+GET(nextbp+4),REL_DIS(nextbp));
    if(root==bp)
        root=nextbp;
 }