/**
  * ECE254 Linux Dynamic Memory Management Lab
  * @file: main_test.c
  * @brief: The main file to write tests of memory allocation algorithms
  */ 

/* includes */
/* system provided header files. You may add more */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* non-system provided header files. 
   Do not include more user-defined header files here
 */
#include "mem.h"
// TODO: void print_mem_info(){}

//tests deallocation and merging free blocks
//fills mem space then deallocates blocks from left to right and merges
void test_worst_fit() {
	printf("Test Worst Fit \n");
	worst_fit_memory_init(1000);

	void* a = worst_fit_alloc(205);
	void* b = worst_fit_alloc(210);
	void* c = worst_fit_alloc(210);
	void* d = worst_fit_alloc(205);

	print_mem(0);

	worst_fit_dealloc(a);
	printf("Block 1 is freed.\n");
	print_mem(0);

	worst_fit_dealloc(b);
	printf("Block 2 is freed.\n");
	print_mem(0);

	worst_fit_dealloc(c);
	printf("Block 3 is freed.\n");
	print_mem(0);

	worst_fit_dealloc(d);
	printf("Block 4 is freed.\n");
	print_mem(0);

	return;
}

void test_best_fit() {
	printf("Test Best Fit \n");
	best_fit_memory_init(1000);

	void* a = best_fit_alloc(205);
	void* b = best_fit_alloc(210);
	void* c = best_fit_alloc(210);
	void* d = best_fit_alloc(205);

	print_mem(1);

	best_fit_dealloc(a);
	printf("Block 1 is freed.\n");
	print_mem(1);

	best_fit_dealloc(b);
	printf("Block 2 is freed.\n");
	print_mem(1);

	best_fit_dealloc(c);
	printf("Block 3 is freed.\n");
	print_mem(1);

	best_fit_dealloc(d);
	printf("Block 4 is freed.\n");
	print_mem(1);

	return;
}

void worst_vs_best_fragments() { 
	printf("Test Worst Fit Vs. Best Fit Fragments: Best Fit Case \n");
	int result = best_fit_memory_init(1200);
	if(result == -1){
		return;
	}

	void *a = best_fit_alloc(52);
	best_fit_alloc(4);
	void *b = best_fit_alloc(100);
	best_fit_alloc(4);

	void *c = best_fit_alloc(52);
	best_fit_alloc(4);
	void *d = best_fit_alloc(100);
	best_fit_alloc(4);

	void *e = best_fit_alloc(52);
	best_fit_alloc(4);
	void *f = best_fit_alloc(100);
	best_fit_alloc(4);

	void *g = best_fit_alloc(52);
	best_fit_alloc(4);
	void *h = best_fit_alloc(100);
	

	printf("After allocating \n");
	print_mem(1);
	printf("---\n");

	best_fit_dealloc(a);
	best_fit_dealloc(b);
	best_fit_dealloc(c);
	best_fit_dealloc(d);
	best_fit_dealloc(e);
	best_fit_dealloc(f);
	best_fit_dealloc(g);
	best_fit_dealloc(h);
	
	printf("After deallocating \n");
	print_mem(1);
	printf("---\n");

	best_fit_alloc(8);
	best_fit_alloc(8);
	best_fit_alloc(8);
	best_fit_alloc(8);

	printf("After allocating 8\n");
	print_mem(1);
	printf("---\n");

	printf("Best fit fragmentation count below 8 bytes: %d \n", best_fit_count_extfrag(8));
	printf("---\n");

	printf("Test Worst Fit Vs. Best Fit Fragments: Worst Fit Case \n");
	int result2 = worst_fit_memory_init(1236);
	if(result2 == -1){
		return;
	}

	void *a2 = worst_fit_alloc(52);
	worst_fit_alloc(4);
	void *b2 = worst_fit_alloc(100);
	worst_fit_alloc(4);

	void *c2 = worst_fit_alloc(52);
	worst_fit_alloc(4);
	void *d2 = worst_fit_alloc(100);
	worst_fit_alloc(4);

	void *e2 = worst_fit_alloc(52);
	worst_fit_alloc(4);
	void *f2 = worst_fit_alloc(100);
	worst_fit_alloc(4);

	void *g2 = worst_fit_alloc(52);
	worst_fit_alloc(4);
	void *h2 = worst_fit_alloc(100);
	

	printf("After allocating \n");
	print_mem(0);
	printf("---\n");

	worst_fit_dealloc(a2);
	worst_fit_dealloc(b2);
	worst_fit_dealloc(c2);
	worst_fit_dealloc(d2);
	worst_fit_dealloc(e2);
	worst_fit_dealloc(f2);
	worst_fit_dealloc(g2);
	worst_fit_dealloc(h2);
	
	printf("After deallocating \n");
	print_mem(0);
	printf("---\n");

	worst_fit_alloc(8);
	worst_fit_alloc(8);
	worst_fit_alloc(8);
	worst_fit_alloc(8);

	printf("After allocating 8 \n");
	print_mem(0);
	printf("---\n");

	printf("worst fit fragmentation count below 8 bytes: %d \n", worst_fit_count_extfrag(8));
	printf("---\n");

}

void best_vs_worst_large_data() {
	worst_fit_memory_init(2124);
	void* a = worst_fit_alloc(1000);
	worst_fit_alloc(4);
	void* b = worst_fit_alloc(1000);
	worst_fit_dealloc(a);
	worst_fit_dealloc(b);
	best_fit_memory_init(2124);
	void* c = best_fit_alloc(1000);
	best_fit_alloc(4);
	void* d = best_fit_alloc(1000);
	best_fit_dealloc(c);
	best_fit_dealloc(d);

	int i;
	for(i = 0; i < 16; i++) {
		worst_fit_alloc(20);
		best_fit_alloc(20);
	}

	void* kB_worst = worst_fit_alloc(1000);
	void* kB_best = best_fit_alloc(1000);

	printf("worst kB: %lu best kB: %lu\n", (long unsigned int)kB_worst, (long unsigned int)kB_best);
}

int main(int argc, char *argv[]) {
	test_worst_fit();
	test_best_fit();
	worst_vs_best_fragments();
	best_vs_worst_large_data();

	return 0;
}