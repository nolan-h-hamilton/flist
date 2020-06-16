#include <stdlib.h>
#include <stdio.h>


/**
 * flist is a doubly-linked, circular linked-list of double values with O(1) access
 * to head and tail. In addition, each creation/removal of a node in flist results
 * in immediate constant-time recomputing of mean, std_dev, variance, sum, and
 * length of the flist. These values are stored as fields in the flist struct
 * for access in O(1) time.
 *
 * @nolan-h-hamilton
 * @jamesdevftw
 */


#ifndef FLIST_H_
#define FLIST_H_

/* Structs */

typedef struct fl_node {
        double num;
        struct fl_node *prev;
        struct fl_node *next;
} fl_node_type, *fl_node;


/*
 * only add fields to this struct which can be computed
 * at each addition/removal to the flist with a single computation. some
 * features to consider might be min, max, std_dev(single-pass estimator)
 */
typedef struct {
        fl_node head;
        fl_node tail;
        double mean;
	double variance;
	double std_dev;
	double sumsq;
        double sum;
        int len;
} flist_type, *flist;

/***************/

/* Functions */

fl_node fl_make_node(double n);



flist fl_make_flist();


/* after a node has been removed or added to flist, update mean, sum, len fields appropriately */
flist fl_update_measures(flist l, double n, int add);

/* append to flist in O(1) time */
flist fl_append(flist l, double n);


fl_node fl_find(flist l, double n);

/* returns the (0-indexed) k-th element of flist l in O(n) (n/2 max steps)*/
fl_node fl_get_kth(flist l, int k);


/* allows for use of flist as a stack. O(1). */
flist fl_push(flist l, double n);


/* allows for use of flist as a stack. O(1). */
double fl_pop(flist l);

/* allows for use of flist as a queue. O(1). */
double fl_dequeue(flist l);

/* remove element at index in O(k) k <= len(list) */
flist fl_remove_index(flist l, int index);


/* inserts element with value `n` after the provided index in O(k) time, k = index <= len(list)*/
flist fl_insert_index(flist l, int index, double n);

flist fl_insert(flist l, double n);


flist fl_subflist(flist l, int a, int b);

flist fl_reverse(flist l);

void fl_destroy(flist l);


/* starting at head and ending at tail, print nodes of flist in format: "node_ptr: (*prev, num, *next)" */
void fl_print(flist l);

/* print single node of flist in format: "node_ptr: (*prev, num, *next)" */
void fl_print_node(fl_node nd);


/* print basic info of flist */
void fl_state(flist l);

int fl_equals(flist l, flist m);

flist fl_combine(flist l, flist m);

flist fl_copy(flist currentFlist);

fl_node fl_merge(fl_node A, fl_node B);

fl_node fl_split(fl_node head);

fl_node fl_merge_sort(fl_node head);

flist fl_sort(flist l);
/*********************/

#endif
