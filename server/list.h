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

typedef struct User{
    int live;
    char* name;
    pthread_mutex_t* locker;
    void (*setMessage)(struct node* whom, char* message);
    char* (*getMessage)(struct node* whom);
    void (*freeMessage)(struct node* whom);
    
    char* message;
} user_t;

typedef struct node {
    
    struct node * next;
    void* element;
        
} node_t;





node_t* create(void);
char* print_list(node_t * head);
void push(node_t * head, node_t * node);
int remove_by_index(node_t ** head, int n);
node_t* contains_name(node_t* head, char* name);
