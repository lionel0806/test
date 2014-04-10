#ifndef _BTREE_H_
#define _BTREE_H_

typedef struct node btree_node_st;

struct node
{
    void *elem;
    int cnt;
    struct node *left;
    struct node *right;
};

btree_node_st *btree_add(btree_node_st *, void *);
void btree_print(btree_node_st *);
void btree_destory(btree_node_st *);

#endif
