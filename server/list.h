//
//  list.h
//  server
//
//  Created by anton on 29.09.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#ifndef list_h
#define list_h


#include <pthread.h>
#include <stdlib.h>
#include "utils.h"




typedef struct User{
    int live;
    char* name;
    int sock;
    int thread;
    int needStop;
    pthread_t* pthread;
    pthread_mutex_t locker;
    void (*setMessage)(struct User* , char* );
    char* (*getMessage)(struct User* );
    void (*kill)(struct User*);
    char* message;
} user_element;
typedef struct node_element {
    struct node_element * next;
    user_element* element;
} list_node;


void setMessageU(struct User* self, char* message);
char* getMessageU(struct User* self);
void killU(struct User* self);


list_node* create(user_element* user);
char* print_list(list_node * head);
list_node * push(list_node * head, user_element* user);
list_node* remove_by_index(list_node ** head, int n);
list_node* contains_name(list_node* head, char* name);
int contains_name_index(list_node* head, char* name);
#endif /* list_h */
