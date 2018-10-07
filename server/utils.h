//
//  utils.h
//  server
//
//  Created by anton on 03.10.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#ifndef utils_h
#define utils_h


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

};

char* getParamTwoString(char* message, int all);
char* getParamThreeString(char* message);

#endif /* utils_h */
