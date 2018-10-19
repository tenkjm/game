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




list_node* create(user_element* user)
{
    list_node * head = malloc(sizeof(list_node));
    head->element = (void*)malloc(sizeof(typeof (user_element)));
    memcpy((user_element*)head->element, (user_element*)user, sizeof(typeof (user_element)));
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
    self->needStop = 1;
}
char* getMessageU(struct User* whom)
{
    return whom->message;
}

char* print_list(list_node * head) {
    list_node * current = head;
    char *string3 = createString(1000);
    memset(string3, 0,  1000);
    
    char *pwr = createString(10);
    memset(pwr, 0,  10);
    
    while (current != NULL) {
        
        sprintf(pwr, " %d\n", ((user_element*)current->element)->live);
        strcat(string3, ((user_element*)current->element)->name);
        strcat(string3, pwr);
        
        current = current->next;
    }
    free(pwr);
    return string3;
}

 list_node * push(list_node * head, user_element* user) {
    
    if(head==NULL)
    {
        head = create(user);
        return head;
    }
    
    list_node * current = head;
    
    while (current->next != NULL) {
        current = current->next;
    }
    
    /* now we can add a new variable */
    current->next = malloc(sizeof(list_node));
    current->next->element = (void*)user;
    current->next->next = NULL;
     return head;
}
list_node* remove_by_index(list_node ** head, int n) {
    int i = 0;
    
    list_node* current = *head;
    list_node* temp_node = NULL;
    
    if (n == 0) {
        list_node* t = (*head)->next;
         free(*head);
         *head=t;
        return current;
    }
    
    for (i = 0; i < n; i++) {
        if (current == NULL) {
            return NULL;
        }
        temp_node = current;
        current = current->next;
        if(i==n)
        {
            break;
        }
    }
    
    
    if(current->next!=NULL)
    {
        temp_node->next = current->next;
    }
    else
    {
        temp_node->next = NULL;
    }
    printf("Before free\n");
    free(current->element);
    free(current);    
    printf("After free\n");
    return NULL;
    
}
list_node* contains_name(list_node* head, char* name)
{
    if(name==NULL)
    {
        return NULL;
    }
    list_node * current = head;
    while (current != NULL) {
        if(strcmp(name, ((user_element*)(current->element))->name)==0)
           {
               return current;
           }
    
        current = current->next;
    }
    return NULL;
}

int contains_name_index(list_node* head, char* name)
{
    if(name==NULL)
    {
        return -1;
    }
    int i = 0;
    list_node * current = head;
    while (current != NULL) {
        
        if(strcmp(name, ((user_element*)(current->element))->name)==0)
        {
            return i;
        }
        i++;
        current = current->next;
    }
    return -1;
}
