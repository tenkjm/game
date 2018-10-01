//
//  list.c
//  server
//
//  Created by anton on 29.09.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#include "list.h"




node_t* create()
{
    node_t * head = NULL;
    head = malloc(sizeof(node_t));
    if (head == NULL) {
        return NULL;
    }
    
    head->live = 1;
    head->next = NULL;
    head->name = "Vasya\n";
    
    return head;
}

char* print_list(node_t * head) {
    node_t * current = head;
    char *string3 = malloc(1000);
    memset(string3, 0,  1000);
    
    while (current != NULL) {
        strcat(string3, current->name);
        
        current = current->next;
    }
    return string3;
}

void push(node_t * head, int val, char* name) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    /* now we can add a new variable */
    current->next = malloc(sizeof(node_t));
    current->next->live = val;
    current->next->name = malloc(strlen(name));
    strcpy(current->next->name, name);
    current->next->next = NULL;
}
int remove_by_index(node_t ** head, int n) {
    int i = 0;
    int retval = -1;
    node_t * current = *head;
    node_t * temp_node = NULL;
    
    if (n == 0) {
         free(head);
    }
    
    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return -1;
        }
        current = current->next;
    }
    
    temp_node = current->next;
    retval = temp_node->live;
    current->next = temp_node->next;
    free(temp_node);
    
    return retval;
    
}
int contains_name(node_t* head, char* name)
{
    node_t * current = head;
    while (current != NULL) {
        if(strncmp(name, current->name, sizeof(name))==0)
           {
               return 1;
           }
    
        current = current->next;
    }
    return 0;
}
