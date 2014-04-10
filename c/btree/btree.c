#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "btree.h"

#define LINE_BUF_MAX (4 * 1024)
#define WORD_LEN_MAX (128)

static btree_node_st * 
_init_node(void)
{
    btree_node_st * node;

    node = (btree_node_st *)malloc(sizeof(btree_node_st));
    if(!node)
    {
        printf("no enough memory!\n");
        return NULL;
    }
   
    node->elem = NULL;
    node->cnt = 0;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static void 
_insert_elem(btree_node_st *node, void *elem)
{
    char *elem_buf;

    if(!node)
        return;
    elem_buf = (char *)calloc(sizeof((char *)elem) + 1, sizeof(char));
    if(elem_buf)
    {
        strcpy(elem_buf, (char *)elem);
        node->elem = (void *)elem_buf;
        node->cnt ++;
    }
}

btree_node_st *
btree_create(void)
{
    return _init_node();
}



btree_node_st *
btree_add(btree_node_st *node, void *elem)
{
    int rc;

    if(node == NULL)
    {
        btree_node_st *n;

        n = _init_node();
        _insert_elem(n, elem); 
        return n;
    }

    if((rc = strcmp((char *)node->elem, (char *)elem)) == 0)
    {
        node->cnt ++;
    }
    else if(rc < 0)
    {
        node->left = btree_add(node->left, elem);
    }
    else
    {
        node->right = btree_add(node->right, elem);
    }

    return node;
}

void 
btree_print(btree_node_st *node)
{
    if(!node)
        return;
    printf("elem:%s \t cnt:%d\n", (char *)node->elem, node->cnt);
   // printf("elem len:%d\n", strlen((char *)node->elem));
    btree_print(node->left);
    btree_print(node->right);
}

static void 
_free_node(btree_node_st *node)
{
    if(!node)
        return;
    if(node->elem)
    {
       free(node->elem);
       node->elem = NULL;
    }
    _free_node(node->left);
    _free_node(node->right);
    
    free(node);
}


void
btree_destory(btree_node_st *node)
{
    _free_node(node);
}


static void
usage(char **argv)
{
    printf("%s <filepath>\n", argv[0]);
}

static char *
get_word(char *buf, char *word)
{
    int i;

    if(!buf || !word)
        return NULL;
   
    //for(i = 0; buf[i] = ' ';buf ++)

    for(i = 0; buf[i] != ' ' && i < WORD_LEN_MAX; i++)
    {
        word[i] = buf[i];
    }
    
    word[i] = 0;

    return &buf[++i];
}

int main(int argc, char **argv)
{
    char *filepath;
    FILE *fp;
    char line[LINE_BUF_MAX + 1] = {0};
    char word[WORD_LEN_MAX + 1];
    char *ptr;
    btree_node_st *root = NULL;
    int line_len;

    if(argc != 2)
    {
        usage(argv);
        return 1;
    }

    filepath = argv[1];
    fp = fopen(filepath, "r");
    if(fp == NULL)
    {
        printf("open file error !\n");
        return 1;
    }
    
   // root = btree_create();

    while(fgets(line, LINE_BUF_MAX, fp))
    {
        line_len = strlen(line);
        ptr = line;
        while(ptr - line < line_len)
        {
            ptr = get_word(ptr, word);
            printf("word:%s\n",word);
            root = btree_add(root, (void *)word);
        }
    }

    printf("element \t count\n");
    btree_print(root);
    btree_destory(root);

    fclose(fp);
    return 0;
}



