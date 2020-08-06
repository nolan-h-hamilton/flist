/* Compile with 'gcc -o example example.c flist.c -lm'*/

#include <stdio.h>
#include "flist.h"

int main() {

	/* various manipulations, additions, removals, etc. to flist */
	flist fl7 = fl_make_flist();
	fl_append(fl7, 65);
	fl_push(fl7,10);
	fl_insert(fl7, 20);
	fl_insert_index(fl7,1,25);
	fl_insert_index(fl7,0,5);
	fl_insert_index(fl7,fl7->len-1, 68);
	fl_insert(fl7, 100);
	fl_insert(fl7, -5);
	fl_insert_index(fl7, fl7->len-1, -500);
	fl_state(fl7);
	
	fl_sort(fl7);
	fl_state(fl7);
	
	fl_pop(fl7);
	fl_pop(fl7);
	fl_remove_index(fl7, 0);
	fl_remove_index(fl7, fl7->len-1);
	fl_dequeue(fl7);
	fl_state(fl7);
	
	fl_reverse(fl7);
	fl_state(fl7);


	printf("variance: %.3f, s_dev: %.3f, mean: %.3f, sum: %.3f, len: %d, are all just a pointer away...",\
	        fl7->variance, fl7->std_dev, fl7->mean, fl7->sum, fl7->len);
		
	return 0;
}

/*
example.c output

sum: -212.000
mean: -23.556
(est.) variance: 29447.802
(est.) standard deviation: 171.604
length: 9
head: (0x55b428ce6390, -5.000)
head prev: (0x55b428ce63b0, -500.000)
tail: (0x55b428ce63b0, -500.000)
tail next: (0x55b428ce6390, -5.000)
empty?: 0
flist elements:
0x55b428ce6390: (0x55b428ce63b0, -5.000, 0x55b428ce6330)
0x55b428ce6330: (0x55b428ce6390, 5.000, 0x55b428ce62d0)
0x55b428ce62d0: (0x55b428ce6330, 10.000, 0x55b428ce6310)
0x55b428ce6310: (0x55b428ce62d0, 25.000, 0x55b428ce62f0)
0x55b428ce62f0: (0x55b428ce6310, 20.000, 0x55b428ce62b0)
0x55b428ce62b0: (0x55b428ce62f0, 65.000, 0x55b428ce6350)
0x55b428ce6350: (0x55b428ce62b0, 68.000, 0x55b428ce6370)
0x55b428ce6370: (0x55b428ce6350, 100.000, 0x55b428ce63b0)
0x55b428ce63b0: (0x55b428ce6370, -500.000, 0x55b428ce6390)


sum: -212.000
mean: -23.556
(est.) variance: 29447.802
(est.) standard deviation: 171.604
length: 9
head: (0x55b428ce6390, -500.000)
head prev: (0x55b428ce63b0, 100.000)
tail: (0x55b428ce63b0, 100.000)
tail next: (0x55b428ce6390, -500.000)
empty?: 0
flist elements:
0x55b428ce6390: (0x55b428ce63b0, -500.000, 0x55b428ce6330)
0x55b428ce6330: (0x55b428ce6390, -5.000, 0x55b428ce62d0)
0x55b428ce62d0: (0x55b428ce6330, 5.000, 0x55b428ce6310)
0x55b428ce6310: (0x55b428ce62d0, 10.000, 0x55b428ce62f0)
0x55b428ce62f0: (0x55b428ce6310, 20.000, 0x55b428ce62b0)
0x55b428ce62b0: (0x55b428ce62f0, 25.000, 0x55b428ce6350)
0x55b428ce6350: (0x55b428ce62b0, 65.000, 0x55b428ce6370)
0x55b428ce6370: (0x55b428ce6350, 68.000, 0x55b428ce63b0)
0x55b428ce63b0: (0x55b428ce6370, 100.000, 0x55b428ce6390)


sum: 120.000
mean: 30.000
(est.) variance: 437.500
(est.) standard deviation: 20.917
length: 4
head: (0x55b428ce6310, 10.000)
head prev: (0x55b428ce6350, 65.000)
tail: (0x55b428ce6350, 65.000)
tail next: (0x55b428ce6310, 10.000)
empty?: 0
flist elements:
0x55b428ce6310: (0x55b428ce6350, 10.000, 0x55b428ce62f0)
0x55b428ce62f0: (0x55b428ce6310, 20.000, 0x55b428ce62b0)
0x55b428ce62b0: (0x55b428ce62f0, 25.000, 0x55b428ce6350)
0x55b428ce6350: (0x55b428ce62b0, 65.000, 0x55b428ce6310)


sum: 120.000
mean: 30.000
(est.) variance: 437.500
(est.) standard deviation: 20.917
length: 4
head: (0x55b428ce6350, 65.000)
head prev: (0x55b428ce6310, 10.000)
tail: (0x55b428ce6310, 10.000)
tail next: (0x55b428ce6350, 65.000)
empty?: 0
flist elements:
0x55b428ce6350: (0x55b428ce6310, 65.000, 0x55b428ce62b0)
0x55b428ce62b0: (0x55b428ce6350, 25.000, 0x55b428ce62f0)
0x55b428ce62f0: (0x55b428ce62b0, 20.000, 0x55b428ce6310)
0x55b428ce6310: (0x55b428ce62f0, 10.000, 0x55b428ce6350)

variance: 437.500, s_dev: 20.917, mean: 30.000, sum: 120.000, len: 4, are all just a pointer away...```
*/
