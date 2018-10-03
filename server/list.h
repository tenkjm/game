//
//  list.h
//  server
//
//  Created by anton on 29.09.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#ifndef list_h
#define list_h

#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#endif /* list_h */



typedef struct node {
    
    struct node * next;
    void* element;
        
} node_t;

typedef struct User{
    int live;
    char* name;
    pthread_mutex_t* locker;
    void (*setMessage)(struct User* , char* );
    char* (*getMessage)(struct User* );
    void (*freeMessage)(struct User* );
    
    char* message;
} user_t;


void setMessage(struct User* self, char* message);
char* getMessage(struct User* self);
void freeMessage(struct User* self);

node_t* create(void);
char* print_list(node_t * head);
void push(node_t * head, user_t * user);
node_t* remove_by_index(node_t ** head, int n);
node_t* contains_name(node_t* head, char* name);
