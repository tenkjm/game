//
//  utils.h
//  server
//
//  Created by anton on 03.10.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#ifndef utils_h
#define utils_h
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
// типы присылаемых клиентом команд
enum CommandType{
    WHO,
    WALL,
    SAY,
    KILL,
    HEAL,
    UNKNOWN
};

//Передаем параметры потоку чтения из сокета
struct HahdlerParameter{
    int sock;
    pthread_t* thread;
    struct Game* game;
    int* thread_num;
};
// Получаем параметр номер два ( одним словом или до конца )
char* getParamTwoString(char* message, int all);
// Получаем строку на месте третьего параметра
char* getParamThreeString(char* message);
// Выделяем память под строку
char* createString(int length);
enum CommandType getCommandType(char* Message);

#endif /* utils_h */
