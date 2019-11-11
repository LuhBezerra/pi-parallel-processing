//  gcc server.c -lpthread -o server

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#define PORT 2000

int nClients = 0;
int numberCustomers, numbersInstallments, mode;

typedef struct
{
    int sock;
    int installments[];
} Client;

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    Client *c = (Client *)socket_desc;
    int sock = c->sock;

    int read_size;
    char *message, client_message[2000];

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock, message, strlen(message));

    //Receive a message from client
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)
    {
        //end of string marker
        client_message[read_size] = '\0';

        //Send the message back to client
        write(sock, client_message, strlen(client_message));

        //clear the message buffer
        memset(client_message, 0, 2000);
    }

    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    numbersInstallments = atoi(argv[1]);
    numberCustomers = atoi(argv[2]);
    mode = atoi(argv[3]);

    Client arrayClient[numberCustomers];

    // arrayClient = realloc(arrayClient, numberCustomers * sizeof(Client));
    // arrayClient = malloc(sizeof(Client) * numberCustomers);

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 1);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;
    int i = 0;
    int sockT, idT;

    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        arrayClient[i].sock = client_sock;
        i = i + 1;

        if (nClients <= numberCustomers - 1)
        {
            puts("Connection accepted");
            if (nClients == numberCustomers - 1)
            {
                for (int count = 0; count < numberCustomers; count++)
                {
                    printf("\n%d\n", count);
                    // sockT = arrayClient[i].sock;
                    if (pthread_create(&thread_id, NULL, connection_handler, (void *)&arrayClient[count]) < 0)
                    {
                        perror("could not create thread");
                        return 1;
                    }
                }
            }
            nClients = nClients + 1;
            puts("Handler assigned");
        }
        else
        {
            write(client_sock, "Client limit", strlen("Client limit"));
            printf("Client limit");
        }
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}