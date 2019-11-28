#include <stdint.h>
#define MAX_SIZE 10

typedef struct btree_node *btree;

//data structure
struct data{
	uint64_t key;
	uint64_t value;
};

//node structure
struct btree_node {
    int size;
	int is_leaf;
	struct data data[MAX_SIZE + 1]; //for convinient
	btree children[MAX_SIZE + 2];
};

btree B_tree_create(void);
void B_tree_destroy(btree t);
uint64_t B_tree_search(btree t, uint64_t key);
void B_tree_insert(btree t, struct data elem);
void B_tree_delete(btree t, uint64_t key);

