#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "btree.h"

int main(int argc, char **argv) {

	btree b;

	b = B_tree_create();

	struct data elem;
	elem.key = 42;
	elem.value = 24;

	assert(B_tree_search(b, 42) == -1);
    B_tree_insert(b, elem);
	assert(B_tree_search(b, 42) == 24);
    B_tree_destroy(b);

	b = B_tree_create();
	for (int i = 0; i < 100; i++) {
		printf("%d\n", i);
		elem.key = i;
		elem.value = i*i;
		assert(B_tree_search(b, i) == -1);
		printf("\tsearch1\n", i);
        B_tree_insert(b, elem);
		printf("\tinsert\n", i);
		assert(B_tree_search(b, i) == i*i);
		printf("\tsearch2\n", i);
	}

	for (int i = 0; i < 100; i++) {
		printf("%d\n", i);
		elem.key = i;
		elem.value = i*i*i;
		assert(B_tree_search(b, i) == i*i);
		printf("\tsearch1\n", i);
		B_tree_insert(b, elem);
		printf("\tinsert\n", i);
		assert(B_tree_search(b, i) == i*i*i);
		printf("\tsearch2\n", i);
	}

    B_tree_destroy(b);

	return 0;
}
