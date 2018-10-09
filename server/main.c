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
#include "gameProcessor.h"
#include "utils.h"
#include "list.h"

struct Game game;
struct UserStore userStore;

void *connection_handler(void *);
int totalConnected=0;
int socket_desc , new_socket , c , *new_sock, Bind, Listen;
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



void *timer_handler()
{
    while(1){
        game.wall(&game, "Round starts\n");
        sleep(20);
        if(game.commandsLen>0)
        {
            for (int i=0; i<game.commandsLen; i++)
            {
              struct Command cmd = game.commands[i];
                
              printf("User = %s\n", cmd.User);
              node_tlist* userNode =  game.userStore->contains_name(game.userStore->head, cmd.User);
                
              if(userNode != NULL)
              {
                  user_tlist* user =(user_tlist*)(userNode->element);
                  printf("life is %d\n", (user)->live);
                  int life = user->live-cmd.change;
                  printf("life= %d \n", life);
                  
                 
                  printf("kompare");
                  if(life==0)
                  {
                      printf("k");
                      shutdown((user)->sock, SHUT_RDWR);
                      (user)->kill((user));
                      printf("s");
                      
                      int index = game.userStore->contains_name_index(game.userStore->head, (user)->name);
                      printf("before remove");
                      game.userStore->remove_by_index(&(game.userStore->head), index);
                      game.wall(&game, "\nsomebody ripped\n");
                      node_tlist* userAttacker =  game.userStore->contains_name(game.userStore->head, cmd.FromUser);
                      ((user_tlist*)userAttacker->element)->setMessage(((user_tlist*)userAttacker->element),"\nyou killed\n");
                  }
                  else
                  {
                       (user->live)=life;//((user_tlist*)(user->element))->live+cmd.change;
                  }
              }
            }
            game.commandsLen = 0;
        }
    }
}

void *connection_handler(void *handlerParameterPtr)
{
    //Get the socket descriptor
    struct HahdlerParameter handlerParameter = *(struct HahdlerParameter*)handlerParameterPtr;
    char packet_str[100000];
    int sock = handlerParameter.sock;
    
    char *message;
    //Send some messages to the client
    message = "Enter your name please\n";
    
    write(sock , message , strlen(message));

    printf("read\n");
    int q;
    user_tlist* userElement;
    while((q = read(sock, packet_str, 100000))>=0)
    {
        packet_str[q-1]=0;
        if(strlen(packet_str)<1)
        {
            continue;
        }
        if(contains_name_index(handlerParameter.game->userStore->head, packet_str)>=0)
        {
            char *message;
            //Send some messages to the client
            message = "name exists\n";
            write(sock , message , strlen(message));
        }
        else
        {
            userElement = malloc(sizeof(typeof (user_tlist)));
            
            char* name = malloc(sizeof(typeof(char))*strlen(packet_str));
            
            strcpy(name, packet_str);
            
            userElement->name = name;
            userElement->live = 100;
            userElement->getMessage = getMessageU;
            userElement->setMessage = setMessageU;
            userElement->kill = killU;
            userElement->sock = handlerParameter.sock;
            handlerParameter.game->userStore->head=handlerParameter.game->userStore->push((handlerParameter.game->userStore->head), userElement);
            message = "you entered successfully\n";
            write(sock , message , strlen(message));
            break;
        }
    }

    while((q = read(sock, packet_str, 10000)) > 0){
        if(userElement->needStop)
        {
            close(sock);
            return 0;
        }
        packet_str[q]='\0';
        printf("%s", packet_str);
        enum CommandType command = getCommandType(packet_str);
        char* message = NULL;
        char* user = NULL;
        char* send_message = createString(1000);
        switch (command) {
            case WHO:
                message = handlerParameter.game->who(handlerParameter.game);
                if(message==NULL)
                {
                    break
                }
                write(sock , message , strlen(message));
                break;
            case WALL:
                message = getParamTwoString(packet_str, 1) ;
                if(message==NULL)
                {
                    break
                }
                sprintf(send_message, "\n %s shouts: %s\n", userElement->name, message);
                handlerParameter.game->wall(handlerParameter.game, send_message );
                free(message);
                break;
            case SAY:
                user = getParamTwoString(packet_str,0);
                if(user==NULL)
                {
                    break
                }
                if(game.userStore->contains_name(game.userStore->head,user)==NULL)
                {
                    userElement->setMessage(userElement, "No such a user to say\n");
                    break;
                }
                message = getParamThreeString(packet_str);
                if(message==NULL)
                {
                    break
                }
                sprintf(send_message, "\n %s say: %s\n", userElement->name, message);
                handlerParameter.game->say(handlerParameter.game, send_message, user );
                free(user);
                free(message);
                break;
            case KILL:
                user = getParamTwoString(packet_str,1);
                if(user==NULL)
                {
                    break
                }
                if(game.userStore->contains_name(game.userStore->head,user)==NULL)
                {
                    userElement->setMessage(userElement, "No such a user to say\n");
                    break;
                }
                if(game.userStore->contains_name(game.userStore->head,user)==NULL)
                {
                    userElement->setMessage(userElement, "No such a user to attack\n");
                    break;
                }
                else
                {
                    ((user_tlist*)game.userStore->contains_name(game.userStore->head,user)->element)->setMessage(((user_tlist*)game.userStore->contains_name(game.userStore->head,user)->element), "you under attack\n");
                }
                char * userCopu = createString(strlen(user));
                strcpy(userCopu, user);
                handlerParameter.game->killUser(handlerParameter.game, userCopu , userElement->name );
                free(user);
                break;
            case HEAL:
                user = getParamTwoString(packet_str,1);
                if(user==NULL)
                {
                    break
                }
                if(game.userStore->contains_name(game.userStore->head,user)==NULL)
                {
                    userElement->setMessage(userElement, "No such a user to say\n");
                    break;
                }
                handlerParameter.game->heal(handlerParameter.game,  user );
                free(user);
                break;
            default:
                break;
        }
    }

    //Free the socket pointer
   // free(socket_desc);

    return 0;
}

int main(int argc , char *argv[])
{
    initializeGame(&game);
    initializeUserStore(&userStore);
    game.userStore = &userStore;
    
    pthread_t sniffer_thread_main;
    pthread_create( &sniffer_thread_main , NULL ,  timer_handler , (void*) NULL);
    
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
        struct HahdlerParameter* handlerParameter = malloc(sizeof(struct HahdlerParameter));
        handlerParameter->sock = new_socket;
        handlerParameter->game = &game;
        
        struct node_tlist* node = malloc(sizeof(struct node_tlist*));
        int thread_num;
        handlerParameter->thread = &thread_num;
        
        (thread_num)= pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) handlerParameter);
        
        if( (thread_num) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        puts("Handler assigned");
    }
    
}


