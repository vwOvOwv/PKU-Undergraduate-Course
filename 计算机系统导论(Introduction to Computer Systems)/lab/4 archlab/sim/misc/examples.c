/* 
 * Architecture Lab: Part A 
 * 
 * High level specs for the functions that the students will rewrite
 * in Y86-64 assembly language
 */

/* $begin examples */
/* linked list element */
typedef struct ELE {
    long val;
    struct ELE *next;
} *list_ptr;

/* sum_list - Sum the elements of a linked list */
long sum_list(list_ptr ls)
{
    long val = 0;
    while (ls) {
	val += ls->val;
	ls = ls->next;
    }
    return val;
}

/* rsum_list - Recursive version of sum_list */
long rsum_list(list_ptr ls)
{
    if (!ls)
	return 0;
    else {
	long val = ls->val;
	long rest = rsum_list(ls->next);
	return val + rest;
    }
}

/* bubble_sort - Sort long numbers at data in ascending order */
void bubble_sort(long *data, long count)
{
     long *i, *last;
     for(last = data + count - 1; last > data; last--) {
         for(i = data; i < last; i++) {
             if(*(i + 1) < *i) {
                 long t = *(i + 1);
                 *(i + 1) = *i;
                 *i = t;
             }
         }
    }
}
/* $end examples */
