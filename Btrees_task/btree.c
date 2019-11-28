#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"

//create an empty B-tree node
btree B_tree_create(void) {
	btree b = (btree) malloc(sizeof(*b));
	b->is_leaf = 1;
	b->size = 0;
	return b;
}

//recursive tree destruction
void B_tree_destroy(btree b) {
	if (!(b->is_leaf))
		for (int i = 0; i < b->size + 1; i++)
            B_tree_destroy(b->children[i]);
	free (b);
}

//binary search in an array
int array_searchhigh_key(int size, const struct data *array, uint64_t key) {
    int left  = -1;
    int middle = 0;
    int right = size;
    while (left + 1 < right) {
        middle = (left + right) / 2;
        if (array[middle].key == key)
            return middle;
        else if (array[middle].key < key)
            left = middle;
        else
            right = middle;
    }

    return right;
}


//returns value by key
uint64_t B_tree_search(btree b, uint64_t key) {
	//if there are no elements in the tree, it will return -1
    if (b->size == 0)
		return -1;

	//if there are elements in the tree, it will do recursive tree search over all arrays
	int position = array_searchhigh_key(b->size, b->data, key);
    //if the required element in this node return its value
	if (position < b->size && b->data[position].key == key)
		return b->data[position].value;
    //we fell in the leaf, so we don't find required element
	if (b->is_leaf)
		return -1;
    //recursive search in a suitable child node
	return B_tree_search(b->children[position], key);
}

//Create right (vice versa to left lol) children node if required, or returns NULL if remake is not required
static btree btree_insert_right(btree b, struct data elem, struct data *median) {
	int position;
	int mid;

	position = array_searchhigh_key(b->size, b->data, elem.key);

	//Such key is already inserted - just remove data
	if (position < b->size && b->data[position].key == elem.key) {
		b->data[position] = elem;
		return NULL;
	}

	//if we can add an element to the node without violating its maximum length
	if (b->is_leaf) {
		//insert new element in data with shift of other elements
		memmove(&b->data[position + 1], &b->data[position], sizeof(struct data) * (b->size - position));
		b->data[position] = elem;
		b->size++;
	}
	else {
		//if we can not add an element to the node without violating its maximum length => insert into child
		struct data child_median;
		btree B_tree_child = btree_insert_right(b->children[position], elem, &child_median);

		//If stuff left, insert in current tree
		if (B_tree_child) {
			//Insert popped element in array with shift of other elements
			//Data from left element
			memmove(&b->data[position + 1], &b->data[position], sizeof(struct data) * (b->size - position));
			b->data[position] = child_median;
			//Node itself
			memmove(&b->children[position + 2], &b->children[position + 1], sizeof(struct btree_node) * (b->size - position));
			b->children[position + 1] = B_tree_child;
			b->size++;
		}
	}

	if (b->size >= MAX_SIZE) {
		//Search median to split node
		mid = b->size / 2;
		*median = b->data[mid];

		//Make a new node only right, the left one is old
		btree b_right = (btree) malloc(sizeof(struct btree_node));

		b_right->size = b->size - mid - 1;
		b_right->is_leaf = b->is_leaf;

		//copying data to right tree
		memmove(b_right->data, &b->data[mid + 1], sizeof(struct data) * b_right->size);
		if (!(b->is_leaf))
			memmove(b_right->children, &b->children[mid + 1], sizeof(btree) * (b_right->size + 1));

		//And trim the left tree
		b->size = mid;

		return b_right;
	} else {
		return NULL;
	}
}

void B_tree_insert(btree b, struct data elem) {
	struct data median;

	btree b_right = btree_insert_right(b, elem, &median);

	if (b_right) {
		//We should split root node - then we should change root to another node
		btree b_left = (btree) malloc(sizeof(*b_left));
		memmove(b_left, b, sizeof(*b));

		//Make b to have only 2 children
		b->size = 1;
		b->is_leaf = 0;
		b->data[0] = median;
		b->children[0] = b_left;
		b->children[1] = b_right;
	}
}

//delete with insert delete marker
void B_tree_delete(btree b, uint64_t key) {
	struct data elem;
	elem.key = key; elem.value = -1;
    B_tree_insert(b, elem);
}
