//
//  utils.c
//  server
//
//  Created by anton on 03.10.2018.
//  Copyright © 2018 anton. All rights reserved.
//

#include "utils.h"
#include <stdio.h>

char* getParamTwoString(char* message, int all)
{
    int index1 = 0;
    int index2 = 0;
    
    for (int i=0; i < strlen(message); i++)
    {
        
        if(all==0)
        {
            if(index1==0&&message[i]==' ')
            {
                index1 = i+1;
              
                continue;
            }
            if((index1!=0&&message[i]==' ')||i==strlen(message)-1)
            {
                index2 = i;
                break;
            }
        }
        else
        {
            if(message[i]==' ')
            {
                index1 = i+1;
                index2 = strlen(message)-1;
                break;
                
            }
            
        }
    }
    if(index1!=0&&index2!=0)
    {
        char* str = malloc(sizeof(char)*(index2-index1+1));
        for (int k = index1; k<index2;k++)
        {
            str[k-index1] = message[k];
        }
        str[index2-index1] = 0;
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
        if(message[i]==' '&& qnt>1)
        {
            index1 = i+1;
            index2 = strlen(message)-1;
            break;
        }
       
    }
    if(index1!=0&&index2!=0)
    {
        char* str = malloc(sizeof(char)*(index2-index1+1));
        for (int k = index1; k < index2; k++)
        {
            str[k-index1] = message[k];
        }
        str[index2-index1]=0;
        return str;
    }
    return NULL;    
}




