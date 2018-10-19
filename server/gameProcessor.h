//
//  gameProcessor.h
//  server
//
//  Created by anton on 02.10.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#ifndef gameProcessor_h
#define gameProcessor_h

#include "list.h"

struct Game{
    struct UserStore* userStore;
    int commandsLen;
    struct Command* commands;
    char* (*who)(struct Game*);
    void (*wall)(struct Game*,char* );
    void (*say)(struct Game* ,char* , char* );
    void (*killUser)(struct Game* ,char* , char*);
    void (*heal)(struct Game* ,char* );
};

char* who(struct Game* game);
void wall(struct Game* game,char* message);
void say(struct Game* game,char* message, char* user);
void killUser(struct Game* game,char* user, char* who);
void heal(struct Game* game,char* user);
void initializeGame(struct Game* game);
void initializeUserStore(struct UserStore* userStore);

struct UserStore
{
    list_node* head;
    list_node* (*create)(void);
    char* (*print_list)(list_node * );
    list_node *  (*push)(list_node * , user_element* );
    list_node* (*remove_by_index)(list_node ** , int );
    list_node* (*contains_name)(list_node* , char* );
    int (*contains_name_index)(list_node* , char* );
};
struct Command{
    char* FromUser;
    char* User;
    int change;
};


#endif /* gameProcessor_h */
