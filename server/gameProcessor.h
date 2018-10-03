//
//  gameProcessor.h
//  server
//
//  Created by anton on 02.10.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#ifndef gameProcessor_h
#define gameProcessor_h

#include <stdio.h>
#include "list.h"

#endif /* gameProcessor_h */

struct Game{
    struct UserStore* userStore;
    int commandsLen;
    struct Command* commands;
    char* (*who)(struct Game*);
    void (*wall)(struct Game*,char* );
    void (*say)(struct Game* ,char* , char* );
    void (*killUser)(struct Game* ,char* );
    void (*heal)(struct Game* ,char* );
};

char* who(struct Game* game);
void wall(struct Game* game,char* message);
void say(struct Game* game,char* message, char* user);
void killUser(struct Game* game,char* user);
void heal(struct Game* game,char* user);



struct UserStore
{
    node_t* head;
    node_t* (*create)(void);
    char* (*print_list)(node_t * head);
    void (*push)(node_t * head, int val, char* name);
    int (*remove_by_index)(node_t ** head, int n);
    node_t* (*contains_name)(node_t* head, char* name);
};
struct Command{
    char* User;
    int change;
};



void initializeGame(struct Game* game)
{
    game->who = who;
    game->wall = wall;
    game->say = say;
    game->killUser = killUser;
    game->heal = heal;
}

void initializeUserStore(struct UserStore* userStore)
{
    userStore->contains_name = contains_name;
    //userStore->create = creat;
    userStore->print_list = print_list;
    userStore->push = push;
    userStore->remove_by_index = remove_by_index;
    userStore->contains_name = contains_name;
}
