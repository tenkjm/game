//
//  gameProcessor.c
//  server
//
//  Created by anton on 02.10.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#include "gameProcessor.h"


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
    userStore->create = create;
    userStore->print_list = print_list;
    userStore->push = push;
    userStore->remove_by_index = remove_by_index;
    userStore->contains_name = contains_name;
    userStore->contains_name_index = contains_name_index;
}

char* who(struct Game* game)
{
    return game->userStore->print_list(game->userStore->head);
}

void wall(struct Game* game,char* message)
{
    list_node* current =game->userStore->head;
    while(current!=NULL)
    {
        ((user_element*)current->element)->setMessage(((user_element*)current->element), message);
        current=current->next;
    }
}

void say(struct Game* game,char* message, char* user)
{
    list_node* elem = game->userStore->contains_name(game->userStore->head, user);
    ((user_element*)elem->element)->setMessage((user_element*)elem->element, message);
}
void killUser(struct Game* game,char* user,  char* who)
{
    if (game->commandsLen==0)
    {
        if(game->commands!=NULL)
        {
            free(game->commands);
        }
        game->commands = malloc(sizeof(struct Command));
        game->commands[0].User=user;
        game->commands[0].change=50;
        game->commands[0].FromUser = who;
        game->commandsLen=1;
        return;
    }
    
    game->commands = realloc(game->commands, sizeof(struct Command)*(game->commandsLen+1));
    game->commandsLen+=1;
    game->commands[game->commandsLen-1].User = user;
    game->commands[game->commandsLen-1].FromUser = who;
    game->commands[game->commandsLen-1].change=50;
}
void heal(struct Game* game,char* user)
{
    if (game->commandsLen==0)
    {
        if(game->commands!=NULL)
        {
            free(game->commands);
        }
        game->commands = malloc(sizeof(struct Command));
        game->commands[0].User=user;
        game->commands[0].change=-50;
        game->commandsLen=1;
        return;
    }
    game->commands = realloc(game->commands, sizeof(struct Command)*(game->commandsLen+1));
    game->commandsLen+=1;
    game->commands[game->commandsLen-1].User=user;
    game->commands[game->commandsLen-1].change=-50;
}
