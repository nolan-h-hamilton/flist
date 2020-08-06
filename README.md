<p align="center">
  <img src="https://raw.githubusercontent.com/nolan-h-hamilton/flist/master/docs/flist.png" alt="flist"/>
</p>

***flist 0.01***

[@nolan-h-hamilton](https://github.com/nolan-h-hamilton/) [@jamesdevftw](https://github.com/jamesdevftw/)


**flists** are numeric list types implemented in C that can store, traverse, manipulate, and order up to billions of nodes in a matter of seconds--All while keeping track of and updating the essential metrics of the data for instant access by users.

flists and have the following properties:

* numeric nodes (double type)
* doubly linked
* circularly linked
* O(1) access to head and tail nodes which allows flists to be
    easily utilized as *stacks* and *queues*
* **length**, **mean**, **variance**, **standard deviation**, and **sum**
    of flists are recalculated in *constant time* incrementally following each
    insert, append, remove, pop, push, deqeue, etc. and stored as fields in the
    flist struct for O(1) access. This allows users to avoid caling expensive 
    linear-time functions after data has already been stored.

flists aim to be **fully-abstracted**. See `example.c` for a showcase of *some* of
flists' functionality. 
