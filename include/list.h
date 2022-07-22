#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

struct list_node_struct {
        void *elem;
        struct list_node_struct *prev;
        struct list_node_struct *next;
};

struct list_struct {
        int size;
        struct list_node_struct *root_node;
};

struct list_struct* list_create();
bool list_add(struct list_struct *list, void *elem);

#endif