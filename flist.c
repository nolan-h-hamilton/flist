
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "flist.h"

#define FL_EPSILON 1e-9

/**
 * current method for determining equality between floating point vals.
 * uses epsilon relative to size of values being compared, unless the
 * values are near zero, in which case it uses absolute epsilon as defined
 * by `FL_EPSILON`
 *
 * @nolan-h-hamilton
 */
int fl_near(double a, double b)
{
	double diff = fabs(a - b);
	double d;
	a = fabs(a);
	b = fabs(b);
	
	/* d = max(abs(a),abs(b)) */
	if (a > b) d = a;
	else d = b;

	/* for comparisons near 0, use absolute epsilon */
	double rel_cmp = d * FL_EPSILON;
	if (FL_EPSILON > rel_cmp) {
		return diff <= FL_EPSILON;
	}

	/* otherwise, use relative epsilon */
	return diff <= rel_cmp;
}


/**
 * allocate memory for node and assign value.
 *
 * @nolan-h-hamilton
*/
fl_node fl_make_node(double n)
{
        fl_node nd = (fl_node) malloc(sizeof(fl_node_type));
	
	if (nd == NULL) {
		printf("\nmemory allocation for node failed...returning NULL\n");
		return NULL;
	}
	
        nd->num = n;
        nd->next = NULL;
        nd->prev = NULL;
        return nd;
}


/**
 * allocate memory for and initialize new flist 
 *
 * @nolan-h-hamilton
*/
flist fl_make_flist()
{
        flist l = (flist) malloc(sizeof(flist_type));
	
	if (l == NULL) {
		printf("\nmemory allocation for flist failed..returning NULL\n");
		return NULL;
	}
	
        l->head = NULL;
        l->tail = NULL;
        l->len = 0;
	l->sumsq = 0;
	l->variance = 0;
	l->std_dev = 0;
	l->mean=0;
	l->sum=0;
        return l;
}


/**
 * after a node has been removed or added to flist, update mean, sum, len fields in constant time.
 *
 * @nolan-h-hamilton
 */
flist fl_update_measures(flist l, double n, int add)
{
	if (l == NULL) {
		printf("\nfl_update_measures(): flist `l` is NULL, returning NULL\n");
	}
	
        if (add) {
                l->len++;
                l->sum += n;
		l->sumsq += n*n;
        } else {
                if (l->len <= 1) {
			l->head->num = 0;
                        l->len = 0;
                        l->sum = 0;
                        l->mean = 0;
			l->variance = 0;
			l->sumsq = 0;
                        return l;
                }
                l->sum -= n;
		l->sumsq -= n*n;
                l->len -= 1;
	}
	/* variance is calculated as: second moment - first moment */
	double moment2 = l->sumsq / l->len;
        l->mean = (l->sum) / (l->len);
	l->variance = moment2 - (l->mean*l->mean);
	l->std_dev = sqrt(l->variance);
        return l;
}


/**
 * search for a value in flist and return fl_node if found. O(N).
 *
 * @nolan-h-hamilton
*/
fl_node fl_find(flist l, double n) {
	if (l == NULL || l->len == 0) {
		printf("\nfl_find(): flist `l` is NULL, returning NULL\n");
		return NULL;
	}
	
	fl_node nd = l->head;
	while (nd != l->tail) {
		if (fl_near(nd->num, n)) {
			return nd;
		}
		nd = nd->next;
	}
	
	if (fl_near(nd->num, n))
		return nd;
	
	return NULL;
}



/**
 *  Compare 2 Flists. If they are equal, return 1. Otherwise return 0. O(N).
 * 
 * @jamesdevftw
*/
int fl_equals(flist l, flist m)
{
	/* rewrote some of the edge-case/early disqualification
	 * code (len check, head = sentinel same as head = NULL, etc.
	 * @nolan-h-hamilton
	*/
	if (l == NULL) 
		return m == NULL;
	
	if (m == NULL)
		return l == NULL;
	
	if (l->len != m->len || l->sum != m->sum)
		return 0;
	
	if (l->len == 0 && m->len == 0)
		return 1;
	
	if (l->head == NULL) 
		return m->head == NULL;
	
	if (m->head == NULL)
		return l->head == NULL;

        fl_node nd = l->head;
        fl_node md = m->head;
        /* Iterate through both lists */
        while ((nd->next != l->head) && (md->next != m->head))
        {

	        if (!fl_near(nd->num, md->num))
                {
                        return 0;
                }
                else
                {
                        nd = nd->next;
                        md = md->next;
                }
        }
        /* Check the last nodes */
	if (!fl_near(nd->num, md->num))
        {
                return 0;
        }
        else
        {
                return 1;
        }
}


/**
 * append to flist in O(1) time
 *
 * @nolan-h-hamilton
*/
flist fl_append(flist l, double n)
{
	if (l == NULL) {
		printf("\nfl_append(): flist `l` does not exist...returning NULL\n");
	        return NULL;
	}
	
        fl_node nd = fl_make_node(n);
        if (l->head == NULL || l->len == 0) {
                l->head = nd;
                l->tail = nd;
                nd->next = nd;
                nd->prev = nd;
                return fl_update_measures(l, n, 1);
        }

        nd->prev = l->tail;
        nd->next = l->head;
        l->tail->next = nd;
        l->tail = nd;
        l->head->prev = nd;
        return fl_update_measures(l, n, 1);
}


/**
 * returns the 0-indexed k-th element in O(N)
 *
 * begins traversal at head if `k` <= (l->len / 2)
 * otherwise, begins traversal from tail. N/2 steps
 * are performed in worst-case scenario.
 *
 * @nolan-h-hamilton
 */
fl_node fl_get_kth(flist l, int k)
{
        if (l == NULL) {
		printf("\nfl_get_kth(): flist `l` is NULL...returning NULL\n");
                return NULL;
        }
        if (l->head == NULL) {
                return NULL;
        }

	/* if index n is past midpoint, traverse list from back */
	if (k > (l->len/2.0)) {
		fl_node nd = l->tail;
		int i = l->len;
		while (i > k+1  && i > 0) {
			i--;
			nd = nd->prev;
		}
		return nd;
	}
	else {
		int i = 0;
		int l_len = l->len;
		fl_node nd = l->head;
		while (i < k && i < l_len) {
			i++;
			nd = nd->next;
		}
		return nd;
	}
	return NULL;	
}


/**
 * add node to beginning of flist. O(1).
 *
 * @nolan-h-hamilton @jamesdevftw
*/

flist fl_push(flist l, double n)
{
	if (l == NULL) {
		printf("\nfl_push(): flist `l` does not exist...returning NULL\n");
		return NULL;
	}
        fl_node new = fl_make_node(n);
        if (l->head == NULL) {
                fl_append(l, n);
                return l;
        }
        new->prev = l->tail;
        new->next = l->head;
        l->tail->next = new;
        l->head->prev = new;
        l->head = new;
        return fl_update_measures(l, n, 1);
}


/**
 * remove head node and return head->num in O(1)
 *
 * allows for use of flist as a stack
 *
 * @nolan-h-hamilton
*/
double fl_pop(flist l)
{
        if (l == NULL || l->len == 0) {
                printf("\nfl_pop(): cannot pop empty flist\n");
		exit(1);
        }
	
        if (l->head == NULL) {
                printf("\nfl_pop(): cannot pop emtpy flist\n");
                exit(1);
        }
	
        double ret;
        if (l->len == 1) {
                ret = l->head->num;
                l->head = l->tail;
                printf("\nfl_pop(): flist is now NULL\n");
		fl_update_measures(l, ret, 0);
                return ret;
        }
        fl_node head_cpy = l->head;
	ret = head_cpy->num;
        l->tail->next = l->head->next;
        l->head->next->prev = l->tail;
        l->head = l->head->next;
	fl_update_measures(l, ret, 0);
        free(head_cpy);
        return ret;
}


/**
 * remove tail node from flist and return tail->num in O(1).
 * 
 * allows flists to be used as queues
 *
 * @nolan-h-hamilton
*/
double fl_dequeue(flist l)
{
        if (l == NULL || l->len == 0) {
                printf("\nfl_dequeue(): cannot dequeue empty flist\n");
                exit(1);
        }

        if (l->len == 1) {
                printf("\nfl_dequeue(): flist has length 1, calling fl_pop()\n");
                return fl_pop(l);
        }

        fl_node remove = l->tail;
        double ret = remove->num;
        remove->prev->next = l->head;
        l->head->prev = remove->prev;
        l->tail = remove->prev;
        free(remove);
        l = fl_update_measures(l, ret, 0);
        return ret;
}


/**
 * remove element at index in O(N).
 *
 * uses fl_get_kth(), so at most N/2 steps
 *
 * @nolan-h-hamilton
*/
flist fl_remove_index(flist l, int index)
{
        if (l == NULL) {
		printf("\nfl_remove_index(): flist `l` is NULL...returning NULL\n");
                return NULL;
        }

        if (index > l->len) {
                printf("\nfl_remove_index(): index does not exist\n");
                return NULL;
        }

        if (index == 0) {
                fl_pop(l);
                return l;
        }

        if (index == l->len - 1) {
                fl_dequeue(l);
                return l;
        }

        fl_node remove = fl_get_kth(l, index);
	double remove_num = remove->num;
        remove->prev->next = remove->next;
        remove->next->prev = remove->prev;
	l = fl_update_measures(l, remove->num, 0);
        free(remove);
	return l;
}

/**
 * remove element in flist that has value `n`. removes the first element
 * with value `n` only. runtime is O(N).
 *
 * @nolan-h-hamilton
 */
flist fl_remove(flist l, double n)
{
	if (l == NULL) {
		printf("\nfl_remove(): flist if NULL...returning NULL\n");
		return NULL;
	}
	
	int index = 0;
	fl_node iter = l->head;
	while (iter->next != l->head) {
		fl_print_node(iter);
		if (fl_near(n, iter->num)) {
			return fl_remove_index(l, index);
		}
		index++;
		iter = iter->next;
	}

	if (fl_near(iter->num, n)) {
		return fl_remove_index(l, index);
	}

	printf("\nfl_remove(): key not found in flist...\n");
	return l;
}

/**
 * inserts element with value `n` after the provided index in O(N).
 *
 * uses fl_get_kth(), so at most N/2 steps
 *
 * @nolan-h-hamilton
*/
flist fl_insert_index(flist l, int index, double n)
{
        if (l == NULL) {
		printf("\nfl_insert_index(): flist `l` does not exist...returning NULL\n");
		return NULL;
        }
	
	if (index >= l->len && index > 0) {
		printf("\nfl_insert_index(): index does not exist\n");
		return l;
	}
	

        if (index == 0) {
                fl_push(l, n);
                return l;
        }

        if (index == l->len - 1) {
                fl_append(l, n);
                return l;
        }

        fl_node p = fl_get_kth(l, index - 1);
        fl_node new = fl_make_node(n);
        fl_node p_next_cpy = p->next;

        new->prev = p;
        p->next = new;

        new->next = p_next_cpy;
        p_next_cpy->prev = new;
        return fl_update_measures(l, n, 1);;
}


/**
 * insert new node into sorted list in correct index in O(N).
 *
 * @nolan-h-hamilton
 */
flist fl_insert(flist l, double n) {
	if (l == NULL) {
		printf("\nfl_insert(): flist `l` does not exist...returning NULL\n");
		return NULL;
	}

	if (l->len == 0) {
		fl_append(l,n);
		return l;
	}

	if (n < l->head->num) {
		
	        fl_push(l,n);
		return l;
	}

	if (n > l->tail->num) {
		fl_append(l, n);
		return l;
	}
	
	fl_node iter = l->head->next;
	while (iter != l->head) {
		if ((n > iter->prev->num || fl_near(n, iter->prev->num))
		     && (n < iter->num || fl_near(n, iter->num))) {
			fl_node new = fl_make_node(n);
			iter->prev->next = new;
			new-> prev = iter->prev;
			new->next = iter;
			iter->prev = new;
			return fl_update_measures(l, n, 1);
		}
		iter = iter->next;
	}
	fl_append(l, n);
	return l;
	
}


/**
 *  create a new flist containing elements of `l` at indexes a::b in O(N).
 *
 * @nolan-h-hamilton
*/
flist fl_subflist(flist l, int a, int b)
{
	if (l == NULL) {
		printf("\nfl_subflist(): flist `l` does not exist...returning NULL\n");
		return NULL;
	}
	
	if (!(a < b && b < l->len)) {
		printf("\nfl_subflist(): indices out of range...returning NULL\n");
		return NULL;
	}
		
        flist sub = fl_make_flist();
        fl_node start = fl_get_kth(l, a);
	int steps = (b - a) + 1;
	int i = 0;
        while (i++ < steps) {
		fl_append(sub, start->num);
		start = start->next;
        }
        return sub;
}


/**
 * reverses flist in O(N) time
 *
 * note: reverses list via pointers, not vals
 *
 * @nolan-h-hamilton
*/
flist fl_reverse(flist l)
{
        if (l == NULL || l->len == 1) {
                return l;
        }

        fl_node nd = l->head;
        fl_node head_cpy = l->head;
        fl_node nxt_cpy = NULL;
        while (nd != l->tail) {
                nxt_cpy = nd->next;
                nd->next = nd->prev;
                nd->prev = nxt_cpy;
                nd = nxt_cpy;
        }
        nd->next = nd->prev;
        nd->prev = nxt_cpy;
        l->head = nd;
        l->tail = head_cpy;
	l->head->prev = l->tail;
        return l;
}


/**
 * delete all nodes in `l` in O(N)
 *
 * @nolan-h-hamilton
*/
void fl_destroy(flist l)
{
	if (l == NULL) {
		printf("\nfl_destroy(): flist `l` does not exist...\n");
		return;
	}
	
	fl_node nd = l -> head;
	while (nd != l -> tail) {
		fl_node cpy = nd -> next;
		free(nd);
		nd = cpy;
	}
	free(nd);
}


/**
 * print nodes in `l` with format: "node_addr: (*prev, num, *next)". O(N).
 *
 * @nolan-h-hamilton
*/

void fl_print(flist l)
{
	if (l == NULL) {
		printf("\nfl_print(): flist `l` does not exist...\n");
		return;
	}
	
        if (l->head == NULL && l != NULL) {
                printf("\nfl_print(): empty flist\n");
                return;
        }

        fl_node nd = l->head;
        while (nd->next != l->head) {
                fl_print_node(nd);
                nd = nd->next;
        }
        fl_print_node(nd);
        printf("\n");

}


/**
 * print single node of flist in format: "node_ptr: (*prev, num, *next)"
 *
 * @nolan-h-hamilton
*/
void fl_print_node(fl_node nd)
{
        if (nd == NULL) {
                printf("\fl_print_node(): node is NULL\n");
		return;
        }
        int has_next = 0;
        int has_prev = 0;
        double n = nd->num;
        if (nd->next != NULL) {
                has_next = 1;
        }

        if (nd->prev != NULL) {
                has_prev = 1;
        }

        if (has_next && has_prev) {
                printf("\n%p: (%p, %.7f, %p)", nd, nd->prev, nd->num, nd->next);
                return;
        }

        if (has_prev && !(has_next)) {
                printf("\n%p: (%p, %.7f, NULL)", nd, nd->prev, nd->num);
                return;
        }
        if (has_next && !(has_prev)) {
                printf("\n%p: (NULL, %.7f, %p)", nd, nd->num, nd->next);
                return;
        }
        else {
                printf("\n%p: (NULL, %.7f, NULL)", nd, nd->num);
        }
        return;
}


/**
 * print basic info of flist
 *
 * @nolan-h-hamilton
*/
void fl_state(flist l)
{
        printf("\n\n");
        if (l == NULL) {
                printf("\nfl_state(): flist is null\n");
		return;
        }

        printf("sum: %.3f\n", l->sum);
        printf("mean: %.3f\n", l->mean);
	printf("(est.) variance: %.3f\n", l->variance);
	printf("(est.) standard deviation: %.3f\n", l->std_dev);
	printf("length: %d\n", l->len);
        if (l->head != NULL) {
                printf("head: (%p, %.3f)\n", l->head, l->head->num);
                if (l->head->prev != NULL) {
                        printf("head prev: (%p, %.3f)\n", l->head->prev, l->head->prev->num);
                } else {
                        printf("head -> prev is NULL but head != NULL\n");
                }
        }

        if (l->tail != NULL) {
                printf("tail: (%p, %.3f)\n", l->tail, l->tail->num);
                if (l->tail->next != NULL) {
                        printf("tail next: (%p, %.3f)\n", l->tail->next, l->tail->next->num);
                } else {
                        printf("tail -> next is NULL but tail != NULL\n");
                }
        } else {
                printf("fl_state(): flist is empty and has not been initialized\n");
                return;
        }

        printf("empty?: %d\n", l->len==0);
        printf("flist elements:");
        fl_print(l);
}


/**
 * Function that takes a flist and returns a complete copy of that flist using local references
 *
 * @jamesdevftw
 */
flist fl_copy(flist currentFlist)
{
	if (currentFlist == NULL || currentFlist->head == NULL) {
		printf("\nfl_copy(): flist `currentFlist` is NULL...\n");
		return NULL;
	}

        fl_node nd = currentFlist->head;
        flist newFlist = fl_make_flist();
        
        /* Iterate through both lists */
        while (nd->next != currentFlist->head)
        {
                fl_push(newFlist, nd->num);
                        nd = nd->next;
                                        
        }

        fl_push(newFlist, nd->num);
        return newFlist;
}


/**
 * Function that creates a copy of two lists and combines the copies together.
 * Function returns the combined copies.
 * @jamesdevftw
 */
flist fl_combine(flist l, flist m)
{
	if (l == NULL && m == NULL) {
		printf("\nfl_combine(): both flists empty, returning NULL\n");
		return NULL;
	}
	
	if (l == NULL && m != NULL)
		return m;
	if (m == NULL && l != NULL)
		return l;
        flist newL = fl_copy(l);
        flist newM = fl_copy(m);
        
        //Conncect the two flists
        newL->tail->next = newM->head;
        newM->tail->next = newL->head;

        //Recalculate measures
        //Length, sum and mean
        newL->len = (newL->len + newM->len);
        newL->sum = (newL->sum + newM->sum); 
        newL->mean = (newL->sum/newL->mean);

        //Sum squared
        newL->sumsq = ((newL->sumsq) + (newM->sumsq));
        double moment2 = newL->sumsq / newL->len;

        //Variance
        newL->variance = moment2 - (newL->mean*newL->mean);

        //Standard Deviation
        newL->std_dev = sqrt(newL->variance);

        return newL;
}


/**
 * convert flist to an array of double
 *
 * @nolan-h-hamilton
 */
double * fl_to_arr(flist l)
{
	if (l == NULL || l->head == NULL) return NULL;
	
	double * arr = (double *) malloc(sizeof(double)*l->len);
	
	if (arr == NULL) {
		printf("\nfl_to_arr(): memory allocation for array failed\n");
		return NULL;
	}
	
	for (int i = 0; i < l->len; i++) {
		arr[i] = l->head->num;
		l->head = l->head->next;
	}
	return arr;
}


/**
 * copy vallues from `arr` to `l`
 *
 * note: will cast array values to double
 *
 * @nolan-h-hamilton
 */
void fl_from_arr(flist l, void * arr, int arr_len)
{
	if (l == NULL) {
		printf("\nfl_from_arr(): flist is null...return\n");
		return;
	}
	if (arr == NULL) {
		printf("\nfl_from_arr(): array is null...return\n");
		return;
	}
	double *darr = (double *) arr;
	for (int i = 0; i < arr_len; i++) {
		l = fl_append(l, (double) darr[i]);
	}
}




/* used for fl_sort() */
fl_node fl_merge_(fl_node A, fl_node B) {
	if (A == NULL) return B;
	if (B == NULL) return A;
	fl_node C = NULL;
	fl_node last = NULL;
	while (A != NULL && B != NULL) {
		if (A->num  < B->num) {
			if (C == NULL) {
				C = A;
			}
			else {
				last->next = A;
			}
			last = A;
			last->prev = last;
			A = A->next;
		}
		else {
			if (C == NULL) {
				C = B;
			}
			else {
				last->next = B;
			}
			last = B;
			last->prev = last;
			B = B->next;
		}
	}
	if (A == NULL) {
		last->next = B;
		B->prev = last;
	}
	else {
		last -> next = A;
		A->prev = last;
	}
	return C;
}


/* used for fl_sort() */
fl_node fl_split_(fl_node head) 
{ 
    fl_node fast = head, slow = head; 
    while (fast->next && fast->next->next) 
    { 
        fast = fast->next->next; 
        slow = slow->next; 
    } 
    fl_node temp = slow->next; 
    slow->next = NULL; 
    return temp; 
}


/* used for fl_sort() */
fl_node fl_merge_sort_(fl_node head) {

	if (head == NULL || head->next == NULL) {
		return head;
	}
	fl_node second = fl_split_(head);

	head = fl_merge_sort_(head); 
	second = fl_merge_sort_(second); 
	return fl_merge_(head,second); 
}


/**
 * sort nodes in flist in O(N log N)
 *
 * @nolan-h-hamilton
 */
flist fl_sort(flist l) {
	if (l == NULL || l->len == 0)
		return l;
	fl_node hd_cpy = l->head;
	hd_cpy->prev->next = NULL;
	hd_cpy = fl_merge_sort_(hd_cpy);
	l->head = hd_cpy;
	fl_node iter = l->head;
	while (iter->next != NULL) {
		iter = iter->next;
	}
	l->tail = iter;
	l->tail->next = l->head;
	l->head->prev = l->tail;

	return l;
}
/**
 * determine if values in flist are sorted in O(N)
 *
 * @nolan-h-hamilton
 */
int fl_is_sorted(flist l) {
	if (l == NULL) {
		printf("\nfl_is_sorted(): flist is NULL, returning -1\n");
		return -1;
	}

	if (l->len == 0) return 1;
	
	fl_node hd_cpy = l->head->next;
	while (hd_cpy != l->head) {
		if (hd_cpy->num < hd_cpy->prev->num) {
			return 0;
		}
		hd_cpy = hd_cpy->next;
	}

	return 1;
}
