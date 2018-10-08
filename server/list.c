//
//  list.c
//  server
//
//  Created by anton on 29.09.2018.
//  Copyright © 2018 anton. All rights reserved.
//
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "list.h"




node_tlist* create(user_tlist* user)
{
    node_tlist * head = malloc(sizeof(node_tlist));
    head->element = (void*)malloc(sizeof(typeof (user_tlist)));
    memcpy((user_tlist*)head->element, (user_tlist*)user, sizeof(typeof (user_tlist)));
    head->next = NULL;
    return head;
}

void setMessageU(struct User* whom, char* message)
{
    char* message_copy = malloc(sizeof(char)*strlen(message));
    strcpy(message_copy, message);
    pthread_mutex_lock(&(whom->locker));
    (whom)->message = message_copy;
    
    if((whom)->message!=NULL)
    {
       write(whom->sock, message, strlen(message));
       free(whom->message);
    }
   
    whom->message = NULL;
    pthread_mutex_unlock(&(whom->locker));
}

void killU(struct User* self)
{
    pthread_kill(self->pthread, self->thread);
}
char* getMessageU(struct User* whom)
{
    return whom->message;
}

char* print_list(node_tlist * head) {
    node_tlist * current = head;
    char *string3 = createString(1000);
    memset(string3, 0,  1000);
    
    char *pwr = createString(10);
    memset(pwr, 0,  10);
    
    while (current != NULL) {
        
        sprintf(pwr, " %d\n", ((user_tlist*)current->element)->live);
        strcat(string3, ((user_tlist*)current->element)->name);
        strcat(string3, pwr);
        
        current = current->next;
    }

    return string3;
}

 node_tlist * push(node_tlist * head, user_tlist* user) {
    
    if(head==NULL)
    {
        head = create(user);
        return head;
    }
    
    node_tlist * current = head;
    
    while (current->next != NULL) {
        current = current->next;
    }
    
    /* now we can add a new variable */
    current->next = malloc(sizeof(node_tlist));
    current->next->element = (void*)user;
    current->next->next = NULL;
     return head;
}
node_tlist* remove_by_index(node_tlist ** head, int n) {
    int i = 0;
    node_tlist* retval = NULL;
    node_tlist* current = *head;
    node_tlist* temp_node = NULL;
    
    if (n == 0) {
        node_tlist* t = *head;
         free(*head);
         *head=t->next;
        return current;
    }
    
    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return NULL;
        }
        temp_node = current;
        current = current->next;
    }
    
    
    
    temp_node->next = current->next;
    free(current);
    
    return NULL;
    
}
node_tlist* contains_name(node_tlist* head, char* name)
{
    if(name==NULL)
    {
        return NULL;
    }
    node_tlist * current = head;
    while (current != NULL) {
        if(strcmp(name, ((user_tlist*)(current->element))->name)==0)
           {
               return current;
           }
    
        current = current->next;
    }
    return NULL;
}

int contains_name_index(node_tlist* head, char* name)
{
    if(name==NULL)
    {
        return -1;
    }
    int i = 0;
    node_tlist * current = head;
    while (current != NULL) {
        
        if(strcmp(name, ((user_tlist*)(current->element))->name)==0)
        {
            return i;
        }
        i++;
        current = current->next;
    }
    return -1;
}
