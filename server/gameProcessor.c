//
//  gameProcessor.c
//  server
//
//  Created by anton on 02.10.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#include "gameProcessor.h"
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
    
}
struct Game{
    struct UserStore* userStore;
    int commandsLen;
    struct Command* commands;
    char* (*who)(struct Game* game);
    void (*wall)(struct Game* game,char* message);
    void (*say)(struct Game* game,char* message, char* user);
    void (*killUser)(struct Game* game,char* user);
    void (*heal)(struct Game* game,char* user);
};


char* who(struct Game* game)
{
    return game->userStore->print_list(game->userStore->head);
}

void wall(struct Game* game,char* message)
{
    node_t* current =game->userStore->head;
    while(current!=NULL)
    {
        current->setMessage(current, message);
        current=current->next;
    }
}

void say(struct Game* game,char* message, char* user)
{
    node_t* elem = game->userStore->contains_name(game->userStore, user);
    elem->setMessage(elem, message);
}
void killUser(struct Game* game,char* user)
{
    if (game->commands==NULL)
    {
        game->commands = malloc(sizeof(struct Command));
        game->commands[0].User=user;
        game->commands[0].change=-1;
        game->commandsLen=1;
        return;
    }
    game->commands = realloc(game->commands, sizeof(struct Command)*(game->commandsLen+1));
    game->commandsLen+=1;
    game->commands[game->commandsLen-1].User=user;
    game->commands[game->commandsLen-1].change=-1;
}
void heal(struct Game* game,char* user)
{
    if (game->commands==NULL)
    {
        game->commands = malloc(sizeof(struct Command));
        game->commands[0].User=user;
        game->commands[0].change=1;
        game->commandsLen=1;
        return;
    }
    game->commands = realloc(game->commands, sizeof(struct Command)*(game->commandsLen+1));
    game->commandsLen+=1;
    game->commands[game->commandsLen-1].User=user;
    game->commands[game->commandsLen-1].change=1;
}
