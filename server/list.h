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
} user_tlist;
typedef struct node_element {
    struct node_element * next;
    user_tlist* element;
} node_tlist;


void setMessageU(struct User* self, char* message);
char* getMessageU(struct User* self);
void killU(struct User* self);


node_tlist* create(user_tlist* user);
char* print_list(node_tlist * head);
node_tlist * push(node_tlist * head, user_tlist* user);
node_tlist* remove_by_index(node_tlist ** head, int n);
node_tlist* contains_name(node_tlist* head, char* name);
int contains_name_index(node_tlist* head, char* name);
#endif /* list_h */
