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
    void (*killUser)(struct Game* ,char* );
    void (*heal)(struct Game* ,char* );
};

char* who(struct Game* game);
void wall(struct Game* game,char* message);
void say(struct Game* game,char* message, char* user);
void killUser(struct Game* game,char* user);
void heal(struct Game* game,char* user);
void initializeGame(struct Game* game);
void initializeUserStore(struct UserStore* userStore);

struct UserStore
{
    node_tlist* head;
    node_tlist* (*create)(void);
    char* (*print_list)(node_tlist * );
    node_tlist *  (*push)(node_tlist * , user_tlist* );
    node_tlist* (*remove_by_index)(node_tlist ** , int );
    node_tlist* (*contains_name)(node_tlist* , char* );
};
struct Command{
    char* User;
    int change;
};


#endif /* gameProcessor_h */
