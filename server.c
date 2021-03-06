#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#define PORT 5000

int nClients = 0;
int numberCustomers, numbersInstallments, mode;
double result = 0;
int threadTotal = 0;
clock_t t;

typedef struct
{
    int sock;
    int mode;
    int init;
    int end;
} Client;

void *connection_handler(void *socket_desc)
{
    Client *c = (Client *)socket_desc;
    int sock = c->sock;
    int mode = c->mode;
    int parc = c->init;
    int parcEnd = c->end;
    int read_size;

    int close = 1;
    double msgSend;

    if(mode == 1){
        msgSend = (double)mode;
        send(sock, &msgSend, sizeof(msgSend), 0);

        double msgRead;
        double msgSend = (double)parc;

        send(sock, &msgSend, sizeof(msgSend), 0);
        //Receive a message from client
        while (1)
        {
            if(read_size = read(sock, &msgRead, sizeof(msgRead)) > 0 && close > 0){
                result += msgRead;
                if(msgSend+1 > parcEnd){
                    close = -1;
                    send(sock, &close, sizeof(close),0);
                    threadTotal++;
                } else {
                    msgSend = msgSend+1;
                    send(sock, &msgSend, sizeof(msgSend), 0);
                }
            }
        }
    } else if (mode == 2){
        msgSend = (double)mode;
        send(sock, &msgSend, sizeof(msgSend), 0);

        double arrayMsgSend[2] = {c->init,c->end};
        double loteMsgRead;
        send(sock, &arrayMsgSend, 2 * sizeof(arrayMsgSend),0);

        while (1)
        {
            if(read(sock, &loteMsgRead, sizeof(loteMsgRead)) > 0){
                result += loteMsgRead;
                break;
            }
        }
    }

    if (read_size == 0)
    {   
        threadTotal++;
        puts("Cliente desconectado");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("Falha ao receber informacao.");
    }
    pthread_exit((void*) 0);
    return 0;
}

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    numbersInstallments = atoi(argv[1]);
    numberCustomers = atoi(argv[2]);
    mode = atoi(argv[3]);

    int installmentClient = ceil((double) numbersInstallments/ (double)numberCustomers );
    int arrayInstallment[numbersInstallments];

    for (int i = 0; i < numbersInstallments; i++)
    {
        arrayInstallment[i] = i;
    }

    Client arrayClient[numberCustomers];

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Falha ao criar socket");
    }
    puts("Socket criado");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Falha no bind");
        return 1;
    }

    listen(socket_desc, 1);

    puts("Esperando por conexoes...");
    c = sizeof(struct sockaddr_in);

    pthread_t thread_id;
    int i = 0, positionInstallment = 0;

    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        arrayClient[i].sock = client_sock;
        arrayClient[i].mode = mode;
        arrayClient[i].init = arrayInstallment[positionInstallment];
        positionInstallment += installmentClient;
        if ((sizeof(arrayInstallment) / sizeof(arrayInstallment[0])) > positionInstallment)
        {
            arrayClient[i].end = arrayInstallment[positionInstallment - 1];
        }
        else
        {
            arrayClient[i].end = arrayInstallment[(sizeof(arrayInstallment) / sizeof(arrayInstallment[0])) - 1];
        }
        i += 1;

        if (nClients <= numberCustomers - 1)
        {
            puts("Conexao aceita");
            if (nClients == numberCustomers - 1)
            {
                t = clock();
                printf("\nIniciando calculo, tempo = %ld\n", t);
                for (int count = 0; count < numberCustomers; count++)
                {
                    if (pthread_create(&thread_id, NULL, connection_handler, (void *)&arrayClient[count]) < 0)
                    {
                        perror("Erro ao criar thread");
                        return 1;
                    }
                }
                break;
            }
            nClients = nClients + 1;
        }
        else
        {
            write(client_sock, "Limite de cliente", strlen("Limite de cliente"));
            printf("Limite de cliente");
        }
    }

    while(1){
        if(threadTotal == numberCustomers){
            t = clock() - t;
            printf("\nFim do programa = %ld\n", t);
            double time_taken = ((double)t)/CLOCKS_PER_SEC;
            printf("\nResultado do calculo: %f\n", result*4);
            printf("Tempo de execucao: %f segundos \n", time_taken);
            break;
        }    
    }
    
    if (client_sock < 0)
    {
        perror("Falha");
        return 1;
    }

    return 0;
}