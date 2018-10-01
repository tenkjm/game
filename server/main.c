#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "list.h"

void *connection_handler(void *);
int totalConnected=0;
int socket_desc , new_socket , c , *new_sock, Bind, Listen;
 node_t* head;
//void sig(int a) // подсчет кол-ва нажатий - обработка исключения
//{
//    static int k=0;
//    static time_t time=0;
//    char s[15];
//    printf("interrupt");
//    shutdown(Listen, SHUT_RDWR);
//    shutdown(Bind, SHUT_RDWR);
//    close(new_socket);
//    close(new_sock);
//    exit(0);
//}

enum CommandType{
    WHO,
    WALL,
    SAY,
    KILL,
    HEAL,
    UNKNOWN
};

enum CommandType getCommandType(char* Message)
{
    
    if(strncmp(Message, "who",3)==0){
        return WHO;
                }
    else if(strncmp(Message, "wall", 4)==0){
        return WALL;
                }
    else if(strncmp(Message,  "say", 3)==0){
        return SAY;
                }
    else if(strncmp(Message,  "kill", 4)==0){
        return KILL;
    }
    else if(strncmp(Message,  "heal", 4)==0){
        return HEAL;
    }
    else{
        return UNKNOWN;
    }

}

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    char mes[11];
    
    char *message;
    //Send some messages to the client
    message = "Enter your name please\n";
    
    write(sock , message , strlen(message));
    
   
    
    
    printf("read\n");
    int q;
    
    while((q = read(sock, mes, 10))>=0)
    {
        
        mes[q]='\0';
        if(strlen(mes)<1)
        {
            continue;
        }
        if(contains_name(head, mes))
        {
            char *message;
            //Send some messages to the client
            message = "name exists\n";
            write(sock , message , strlen(message));
        }
        else
        {
            push(head, 10, mes);
            message = "you entered successfully\n";
            write(sock , message , strlen(message));
            break;
        }
    }
    
    
    while((q = read(sock, mes, 10))>0){
        
        mes[q]='\0';
        printf("%s", mes);
        enum CommandType command = getCommandType(mes);
        
        
        if(command==WHO)
        {
            char* user_list= print_list(head);            
            write(sock , user_list , strlen(user_list));
            free(user_list);
        }
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}

int main(int argc , char *argv[])
{
   
    head = create();
    
    
        struct sockaddr_in server , client;
        char *message;
    
    
        //Create socket
        socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        if (socket_desc == -1)
        {
            printf("Could not create socket");
        }
    
        //Prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons( 8080 );
        int optval = 1;
        setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        //Bind
        if( (Bind=bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) )< 0)
        {
            puts("bind failed");
            return 1;
        }
        puts("bind done");
    
        //Listen
        Listen=listen(socket_desc , 3);
    
        //Accept and incoming connection
        puts("Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);
        while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
        {
            puts("Connection accepted");
    
            //Reply to the client
            message = "Hello Client\n";
            //  write(new_socket , message , strlen(message));
    
            pthread_t sniffer_thread;
            new_sock = malloc(1);
            *new_sock = new_socket;
            
            
            print_list(head);
    
            if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
            {
                perror("could not create thread");
                return 1;
            }
    
    
            puts("Handler assigned");
        }
    
    
    
//    signal(SIGINT, sig);
//    struct sockaddr_in server , client;
//    char *message;
//
//
//    //Create socket
//    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
//    if (socket_desc == -1)
//    {
//        printf("Could not create socket");
//    }
//
//    //Prepare the sockaddr_in structure
//    server.sin_family = AF_INET;
//    server.sin_addr.s_addr = INADDR_ANY;
//    server.sin_port = htons( 8080 );
//    int optval = 1;
//    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
//    //Bind
//    if( (Bind=bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) )< 0)
//    {
//        puts("bind failed");
//        return 1;
//    }
//    puts("bind done");
//
//    //Listen
//    Listen=listen(socket_desc , 3);
//
//    //Accept and incoming connection
//    puts("Waiting for incoming connections...");
//    c = sizeof(struct sockaddr_in);
//    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
//    {
//        puts("Connection accepted");
//
//        //Reply to the client
//        message = "Hello Client\n";
//        //  write(new_socket , message , strlen(message));
//
//        pthread_t sniffer_thread;
//        new_sock = malloc(1);
//        *new_sock = new_socket;
//
//        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
//        {
//            perror("could not create thread");
//            return 1;
//        }
//
//
//        puts("Handler assigned");
//    }
//
//    if (new_socket<0)
//    {
//        perror("accept failed");
//        return 1;
//    }
//
//    return 0;
//}
//
///*
// * This will handle connection for each client
// * */
//void *connection_handler(void *socket_desc)
//{
//    //Get the socket descriptor
//    int sock = *(int*)socket_desc;
//    char mes[11];
//    totalConnected++;
//    char *message;
//    //Send some messages to the client
//    message = "Hello enter time or count or quit\n";
//    printf("write\n");
//    write(sock , message , strlen(message));
//    printf("read\n");
//
//    while(1){
//
//        int q =read(sock,  mes, 10);
//        mes[q]='\0';
//        printf("%s", mes);
//        printf("select %d\n",strncmp(mes, "time",4) );
//
//        if(strncmp(mes, "time",4)==0){
//            printf("time:\n");
//
//            char str[115];
//            sprintf(str, "time: %d\n", time(NULL));
//            write(sock , str, strlen(str));
//        }
//
//        else if(strncmp(mes, "count", 5)==0){
//            printf("coutn:\n");
//            char str[115];
//            sprintf(str, "count: %d\n", totalConnected);
//            write(sock , str, strlen(str));
//
//
//        }
//
//        else if(strncmp(mes,  "buy", 3)==0){
//            printf("close:\n");
//            //Free the socket pointer
//            close (sock);
//            free(socket_desc);
//
//            return 0;
//        }
//
//        else{
//            printf("Default:\n");
//            close (sock);
//            free(socket_desc);
//
//            return 0;
//        }
//
//    }
//
//    //Free the socket pointer
//    free(socket_desc);
//
//    return 0;
}


