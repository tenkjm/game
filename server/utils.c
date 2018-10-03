//
//  utils.c
//  server
//
//  Created by anton on 03.10.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#include "utils.h"

char* getParamTwoString(char* message)
{
    int index1 = 0;
    int index2 = 0;
    
    for (int i=0; i<strlen(message); i++)
    {
        if(index1==0&&message[i]!=' ')
        {
            index1 = i;
        }
        if(index1!=0&&i>index1+1&&message[i-1]!=' ')
        {
            index2 = i;
            break;
        }
    }
    if(index1!=0&&index2!=0)
    {
        char* str = malloc(sizeof(char)*(index2-index1));
        return str;
    }
    return NULL;
}
char* getParamThreeString(char* message)
{
    int index1 = 0;
    int index2 = 0;
    int qnt=0;
    for (int i=0; i<strlen(message); i++)
    {
        if(message[i]==' ')
        {
            qnt++;
        }
        if(index1==0&&message[i]!=' '&& qnt>2)
        {
            index1 = i;
        }
        if(index1!=0&&i>index1+1&&message[i-1]!=' ')
        {
            index2 = i;
            break;
        }
    }
    if(index1!=0&&index2!=0)
    {
        char* str = malloc(sizeof(char)*(index2-index1));
        return str;
    }
    return NULL;    
}




