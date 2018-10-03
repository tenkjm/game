//
//  utils.h
//  server
//
//  Created by anton on 03.10.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>

#endif /* utils_h */

enum CommandType{
    WHO,
    WALL,
    SAY,
    KILL,
    HEAL,
    UNKNOWN
};



struct HahdlerParameter{
    int sock;
    struct Game* game;
    struct node* myNode;
};

char* getParamTwoString(char* message);
char* getParamThreeString(char* message);
