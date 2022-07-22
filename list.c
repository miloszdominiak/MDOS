#include <list.h>
#include <stddef.h>
#include <stdlib.h>

static struct list_node_struct* list_node_create(void *elem)
{
        struct list_node_struct* node = malloc(sizeof(struct list_node_struct));

        node->elem = elem;
        node->prev = NULL;
        node->next = NULL;

        return node;
}

struct list_struct* list_create()
{
        struct list_struct *list = malloc(sizeof(struct list_struct));

        list->size = 0;
        list->root_node = NULL;

        return list;
}

bool list_add(struct list_struct *list, void *elem)
{
        struct list_node_struct* new_node = list_node_create(elem);

        if(!list->root_node) {
                list->root_node = new_node;
        } else {
                struct list_node_struct* current = list->root_node;

                while(current->next)
                        current = current->next;

                current->next = new_node;
        }

        list->size++;

        return 0;
}