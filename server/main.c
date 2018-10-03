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
#include "gameProcessor.h"
#include "utils.h"



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

void *connection_handler(void *handlerParameterPtr)
{
    //Get the socket descriptor
    struct HahdlerParameter handlerParameter = *(struct HahdlerParameter*)handlerParameterPtr;
    char packet_str[11];
    int sock = handlerParameter.sock;
    
    char *message;
    //Send some messages to the client
    message = "Enter your name please\n";
    
    write(sock , message , strlen(message));

    printf("read\n");
    int q;
    
    while((q = read(sock, packet_str, 10))>=0)
    {
        packet_str[q]='\0';
        if(strlen(packet_str)<1)
        {
            continue;
        }
        if(contains_name(head, packet_str))
        {
            char *message;
            //Send some messages to the client
            message = "name exists\n";
            write(sock , message , strlen(message));
        }
        else
        {
            push(head, 10, packet_str);
            message = "you entered successfully\n";
            write(sock , message , strlen(message));
            break;
        }
    }

    while((q = read(sock, packet_str, 10))>0){
        packet_str[q]='\0';
        printf("%s", packet_str);
        enum CommandType command = getCommandType(packet_str);
        char* message = NULL;
        char* user = NULL;
        switch (command) {
            case WHO:
                message = handlerParameter.game->who(handlerParameter.game);
                write(sock , message , strlen(message));
                free(message);
                break;
            case WALL:
                message = getParamTwoString(packet_str) ;
                handlerParameter.game->wall(handlerParameter.game, message );
                free(message);
                break;
            case SAY:
                user = getParamTwoString(packet_str);
                message = getParamThreeString(packet_str);
                handlerParameter.game->say(handlerParameter.game, message, user );
                free(user);
                free(message);
            case KILL:
                user = getParamTwoString(packet_str);
                handlerParameter.game->killUser(handlerParameter.game,  user );
                free(user);
            case HEAL:
                user = getParamTwoString(packet_str);
                handlerParameter.game->heal(handlerParameter.game,  user );
                free(user);
            default:
                break;
        }
        message =handlerParameter.myNode->getMessage(handlerParameter.myNode);
        if(message!=NULL)
        {
            write(sock , message , strlen(message));
            handlerParameter.myNode->freeMessage(handlerParameter.myNode);
            free(message);
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
    
}


