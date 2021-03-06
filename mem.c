/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include "mem.h"

/* defines */
struct node {
	struct node* prev;
	struct node* next;
	int size;
	int state; 	// 1 for occupied, 0 for vacant
	void* start;
};
/* global variables */
void *test_mem_best;
void *test_mem_worst;
/* Functions */
void *find_best_node (size_t size) {
	int best_size = 9999999;
	int full = 1;
	int test_size;
	struct node *temp_mem_best = (struct node *)test_mem_best;
	struct node* return_node;
	while (temp_mem_best != NULL) {
		if (!temp_mem_best ->state) {
			test_size = temp_mem_best->size - size; 
			if (test_size > sizeof(struct node) && test_size < best_size) {
				return_node = (struct node *)temp_mem_best;
				full = 0;
			}
		}
		temp_mem_best = temp_mem_best -> next;
	}
	return (full == 1) ? NULL : return_node;
}

void *find_worst_node (size_t size) {
	int best_size = 0;
	int full = 1;
	int test_size;
	struct node *temp_mem_worst = (struct node *)test_mem_worst;
	struct node* return_node;
	while (temp_mem_worst != NULL) {
		if (!temp_mem_worst -> state) {
			test_size = temp_mem_worst->size - size;
			if (test_size > sizeof(struct node) && test_size > best_size) {
				return_node = (struct node *)temp_mem_worst;
				full = 0;
			}
		}
		temp_mem_worst = temp_mem_worst -> next;
	}
	return (full == 1) ? NULL : return_node;
}

/* memory initializer */
int best_fit_memory_init(size_t size)
{
	size_t size_n;
	size_n = size + sizeof(struct node);
	struct node *root_node;
	test_mem_best = malloc(size_n);
	root_node = test_mem_best;
	root_node->size = size;
	root_node->state = 0;
	root_node->start = test_mem_best + sizeof(struct node);
	root_node->prev = NULL;
	root_node->next = NULL;
	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.
	return 0;

}

int worst_fit_memory_init(size_t size)
{
	size_t size_n;
	size_n = size + sizeof(struct node);
	struct node *root_node;
	test_mem_worst = malloc(size_n);
	root_node = test_mem_worst;
	root_node->size = size;
	root_node->state = 0;
	root_node->start = test_mem_worst + sizeof(struct node);
	root_node->prev = NULL;
	root_node->next = NULL;
	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.
	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	int size_flag = size % 4;
	if (size_flag) {
		size += (4 - size_flag);
	}
	struct node *cur_node = find_best_node(size);
	if (cur_node == NULL) {
		return NULL;
	}
	cur_node->state = 1;
	if (cur_node->size > size) {
		struct node *new_node = cur_node->start + size;			
		new_node ->start = cur_node->start + size + sizeof(struct node);
		new_node ->prev = cur_node;
		new_node ->next = cur_node ->next;
		new_node ->size = cur_node->size - size - sizeof(struct node);
		new_node ->state = 0;
		if (cur_node -> next != NULL) {
			cur_node ->next -> prev = new_node;
		}
		cur_node ->size = size;
		cur_node ->next = new_node;
	}
	// To be completed by students
	return cur_node;
}


void *worst_fit_alloc(size_t size)
{
	int size_flag = size % 4;
	if (size_flag) {
		size += (4 - size_flag);
	}
	struct node *cur_node = find_worst_node(size);
	if (cur_node == NULL) {
		return NULL;
	}
	cur_node->state = 1;
	if (cur_node->size > size + sizeof(struct node)) {
		struct node *new_node = cur_node->start + size;
		new_node ->start = cur_node->start + size + sizeof(struct node);
		new_node ->prev = cur_node;
		new_node ->next = cur_node ->next;
		new_node ->size = cur_node->size - size - sizeof(struct node);
		new_node ->state = 0;
		if (cur_node -> next != NULL) {
			cur_node ->next->prev = new_node;
		}
		cur_node ->size = size;
		cur_node ->next = new_node;
	}
	return cur_node;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr) 
{
	if (ptr == NULL) {
		return;
	}
	struct node *temp_mem_best = (struct node *)test_mem_best;
	int isNode = 0;
	while (temp_mem_best != NULL) {
		if (ptr == temp_mem_best) {
			isNode = 1;
			break;
		}
		temp_mem_best = temp_mem_best ->next;
	}
	if (!isNode) {
		return;
	}
	struct node *dealloc_target = (struct node *)ptr;
	dealloc_target ->state = 0;
	struct node *dealloc_prev = dealloc_target ->prev;
	struct node *dealloc_next = dealloc_target ->next;
	while (dealloc_prev != NULL || dealloc_next != NULL) {
		if (dealloc_next != NULL) {
			if (dealloc_next ->state == 0) {
				dealloc_target ->size += dealloc_next ->size + sizeof(struct node);
				if (dealloc_target ->next ->next != NULL) {
					dealloc_target ->next ->next ->prev = dealloc_target;
				}
				dealloc_target ->next = dealloc_target ->next ->next;
				dealloc_next = dealloc_next ->next;
			} else {
				dealloc_next = NULL;
			}
		}

		if (dealloc_prev != NULL) {
			if (dealloc_prev ->state == 0) {
				dealloc_target ->size += dealloc_prev ->size + sizeof(struct node);
				if (dealloc_target ->prev ->prev != NULL) {
					dealloc_target ->prev ->prev ->next = dealloc_target;
				}
				dealloc_target ->prev = dealloc_target ->prev ->prev;
				dealloc_target ->start = dealloc_prev ->start;
				dealloc_prev = dealloc_prev ->prev;
			} else {
				dealloc_prev = NULL;
			}
		} 
	}
}

void worst_fit_dealloc(void *ptr) 
{
	if (ptr == NULL) {
		return;
	}
	struct node *temp_mem_worst = (struct node *)test_mem_worst;
	int isNode = 0;
	while (temp_mem_worst != NULL) {
		if (ptr == temp_mem_worst) {
			isNode = 1;
			break;
		}
		temp_mem_worst = temp_mem_worst ->next;
	}
	if (!isNode) {
		return;
	}
	struct node *dealloc_target = (struct node *)ptr;
	dealloc_target ->state = 0;
	struct node *dealloc_prev = dealloc_target ->prev;
	struct node *dealloc_next = dealloc_target ->next;
	while (dealloc_prev != NULL || dealloc_next != NULL) {
		if (dealloc_next != NULL) {
			if (dealloc_next ->state == 0) {
				dealloc_target ->size += dealloc_next ->size + sizeof(struct node);
				if (dealloc_target ->next ->next != NULL) {
					dealloc_target ->next ->next ->prev = dealloc_target;
				}
				dealloc_target ->next = dealloc_target ->next ->next;
				dealloc_next = dealloc_next ->next;
			} else {
				dealloc_next = NULL;
			}
		}

		if (dealloc_prev != NULL) {
			if (dealloc_prev ->state == 0) {
				dealloc_target ->size += dealloc_prev ->size + sizeof(struct node);
				if (dealloc_target ->prev ->prev != NULL) {
					dealloc_target ->prev ->prev ->next = dealloc_target;
				}
				dealloc_target ->prev = dealloc_target ->prev ->prev;
				dealloc_target ->start = dealloc_prev ->start;
				dealloc_prev = dealloc_prev ->prev;
			} else {
				dealloc_prev = NULL;
			}
		} 
	}
	// To be completed by students
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */
int best_fit_count_extfrag(size_t size)
{
	int count;
	struct node *temp_mem_best = test_mem_best;
	while (temp_mem_best != NULL) {
		if (temp_mem_best ->state == 0 && temp_mem_best ->size < size) {
			count++;
		}
		temp_mem_best = temp_mem_best ->next;
	}
	// To be completed by students
	return count;
}

int worst_fit_count_extfrag(size_t size)
{
	int count;
	struct node *temp_mem_worst = test_mem_worst;
	while (temp_mem_worst != NULL) {
		if (temp_mem_worst ->state == 0 && temp_mem_worst ->size < size) {
			count++;
		}
		temp_mem_worst = temp_mem_worst ->next;
	}
	// To be completed by students
	return count;
}

void print_mem_best() {
	struct node* test_node = (struct node*)test_mem_best; 
	while(test_node != NULL){
		printf("Root Node Address: %lu\tSize: %d\tState: %d\n",
			(long unsigned int)test_node, test_node->size, test_node->state);
		test_node = test_node->next;
	}
}

void print_mem_worst() {
	struct node* test_node = (struct node*)test_mem_worst; 
	while(test_node != NULL){
		printf("Root Node Address: %lu\tSize: %d\tState: %d\n",
			(long unsigned int)test_node, test_node->size, test_node->state);
		test_node = test_node->next;
	}
}
