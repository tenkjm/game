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
    
    ((user_t*)head->element)->live = 1;
    head->next = NULL;
    ((user_t*)head->element)->name = "Vasya\n";
    ((user_t*)head->element)->message = NULL;
    
    return head;
}

void setMessage(struct node* whom, char* message)
{
    pthread_mutex_lock(((user_t*)whom->element)->locker);
    if(((user_t*)whom->element)->message!=NULL)
    {
        free(((user_t*)whom->element)->message);
    }
    ((user_t*)whom->element)->message = message;
    
    pthread_mutex_unlock(((user_t*)whom->element)->locker);
}

void freeMessage(struct node* whom)
{
    pthread_mutex_lock(((user_t*)whom->element)->locker);
    if(((user_t*)whom->element)->message!=NULL)
    {
        free(((user_t*)whom->element)->message);
    }
    pthread_mutex_unlock(((user_t*)whom->element)->locker);
}

char* print_list(node_t * head) {
    node_t * current = head;
    char *string3 = malloc(1000);
    memset(string3, 0,  1000);
    
    while (current != NULL) {
        strcat(string3, ((user_t*)current->element)->name);
        
        current = current->next;
    }
    return string3;
}

void push(node_t * head, user_t* user) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    /* now we can add a new variable */
    current->next = malloc(sizeof(node_t));
    current->next->element = (void*)user;
    current->next->next = NULL;
}
node_t* remove_by_index(node_t ** head, int n) {
    int i = 0;
    node_t* retval = NULL;
    node_t * current = *head;
    node_t * temp_node = NULL;
    
    if (n == 0) {
         free(head);
    }
    
    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return NULL;
        }
        current = current->next;
    }
    
    temp_node = current->next;
    
    current->next = temp_node->next;
    free(temp_node);
    
    return temp_node;
    
}
node_t* contains_name(node_t* head, char* name)
{
    node_t * current = head;
    while (current != NULL) {
        if(strncmp(name, ((user_t*)current->element)->name, sizeof(name))==0)
           {
               return current;
           }
    
        current = current->next;
    }
    return NULL;
}

//void setMessage(struct User* self, char* message)
//{
//    pthread_mutex_lock( self->locker);
//    self->message = message;
//    pthread_mutex_unlock( self->locker);
//}
//char* getMessage(struct User* self)
//{
//    return self->message;
//}
//void freeMessage(struct User* self)
//{
//    pthread_mutex_lock( self->locker);
//    free(self->message);
//    self->message = NULL;
//    pthread_mutex_unlock( self->locker);
//}
