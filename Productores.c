
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>


#define BUFF_SIZE 1024
#define COLADELIVERYPRODUCTORES "/prueba"

mqd_t qda;
char *mensajeProductor = NULL;

void EnvioADelivery(int qd, char *sent_msg){
    unsigned int prio = 1;
    if ( mq_send(qd, sent_msg, BUFF_SIZE, (unsigned int) 1) == -1){
        perror("Sending");
        exit(EXIT_FAILURE);
    }
    bzero(sent_msg, BUFF_SIZE);
}


char * MemoriaLibre(char *directorio, char *msjPorPantalla, char *dato) {
    FILE *file=NULL;
    file = fopen(directorio, "r");

    mensajeProductor = (char *) malloc(20 * sizeof(char));
    char buffer[1024];
    int aux = 0;
    char *num = 0;
    while (!feof(file)) {
        fgets(buffer, 100, file);
        aux = strncmp(dato, buffer,
                      strlen(dato));//La función de biblioteca C int strncmp (const char * str1, const char * str2, size_t n) compara como máximo los primeros n bytes de str1 y str2.
        if (aux == 0) {
            num = strtok(buffer, ":");
            if (num != NULL) {
                num = strtok(NULL, ":");
                // printf(num);
                strcpy(mensajeProductor, msjPorPantalla);
                strcat(mensajeProductor, num);

                printf("%s", mensajeProductor);

                break;
            }
        }
    }
    
    return mensajeProductor;
}
    char* Mensajerandom(int tamanio)
{
    mensajeProductor = (char *) malloc( tamanio* sizeof(char));
    char Mensajerandom[tamanio];
    srand(time(NULL));
    for( int i = 0; i < 10; i++){
        Mensajerandom[i]='a' + rand() % (('z' - 'a') + 1);
        //printf("La letra es: '%c'n", 'a' + rand() % (('z' - 'a') + 1));
    }
    //for(int i=0; i<strlen(Mensajerandom); i++)
      //  printf("%c",Mensajerandom[i]);
    strcpy(mensajeProductor,Mensajerandom);

    return mensajeProductor;


}

    int main(int argc, char *argv[]) {

        __pid_t productor1;
        __pid_t productor2;
        __pid_t productor3;
        char msjserver[BUFF_SIZE];
        char msjclient[BUFF_SIZE];


//######################Configuracion de  cola de mensaje############################################
        struct mq_attr queue_atributes = {0};
        queue_atributes.mq_maxmsg = 10;
        queue_atributes.mq_msgsize = BUFF_SIZE;
        char sent_msg[BUFF_SIZE];
        char recv_msg[BUFF_SIZE];

//Cola de mensajes entre Productores y Delivery
        qda = mq_open(COLADELIVERYPRODUCTORES, O_WRONLY|O_CREAT, 0666 , &queue_atributes);
        if (qda == -1){
            printf("Error al crear/unirse a la cola en el servidor principal: ");
            exit(EXIT_FAILURE);
        }



        productor1 = fork();
        if (productor1 < 0) printf("Error! no se pudo crear un proceso hijo");
        else {
            
            strcpy(sent_msg,"P1:");//Encabezado para poder descriminar luego en la cola de mensaje entre productor
            strcat(sent_msg,  Mensajerandom(10));
            free(mensajeProductor);
            printf("%s", sent_msg);
            EnvioADelivery(qda,sent_msg);


        productor2 = fork();
        if (productor2 < 0) printf("Error! no se pudo crear un proceso hijo");
        else {

            strcpy(sent_msg,"P2:");//Encabezado para poder descriminar luego en la cola de mensaje entre productor
            strcat(sent_msg, (const char *) MemoriaLibre("/proc/meminfo", "Memoria libre:", "MemFree:"));
            free(mensajeProductor);
            printf("%s", sent_msg);
            EnvioADelivery(qda,sent_msg);

        }


        productor3 = fork();
        if (productor3 < 0) printf("Error! no se pudo crear un proceso hijo");
        else {

            strcpy(sent_msg,"P3:");//Encabezado para poder descriminar luego en la cola de mensaje entre productor
            strcat(sent_msg,  "Hola soy productor numero 3");
            EnvioADelivery(qda,sent_msg);

        }
        mq_close(qda);
        return 0;
    }

