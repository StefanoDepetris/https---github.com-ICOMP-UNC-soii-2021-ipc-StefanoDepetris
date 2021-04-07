
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include<time.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/stat.h>

#define TAM 80
#define BUFF_SIZE 8192
#define COLADELIVERYPRODUCTORES "/prueba" //Nombre de la cola de mensaje
#define max_args 13                       //Numero maximo de argumentos (-1) cuando se trate de un comando externo
#define maxln_Com_Amb 10000               //Numero de caracteres maximo para comando las variables de a


char *args[max_args];                     //En este arreglo se almacenan los argumentos del comando ingresado
char comando[maxln_Com_Amb];              //Esta var lee el comando que ingrese el usuario
int pid1;
int pid2;
mqd_t qda;




void SepararArgs(void){
    int i;
    for(i=0;i<(max_args-1);i++)args[i]=NULL;        //Borrar argumento previo que pudiera existir
    strtok(comando," "), i=0;                       //separar palabras individuales usando tokens (espacio vacio)
    args[i]=comando;                                //El 1er argumento sera comando por un requerimiento de execvp
    while((args[++i]=strtok(NULL," "))!=NULL && i<(max_args-2));
}




void RecepcionDeProductores(int qd , char *recv_msg){
    unsigned int prio = 1;
    bzero(recv_msg, BUFF_SIZE);
    if (mq_receive(qd, recv_msg, BUFF_SIZE, &prio) == -1 ){
        perror("Receiving");
        exit(EXIT_FAILURE);
    }
}




int main(int argc, char *argv[]) {
    char msjserver[BUFF_SIZE];
    char msjclient[BUFF_SIZE];
    char eleccion=NULL;


//######################Configuracion de  cola de mensaje############################################
    struct mq_attr queue_atributes = {0};
    queue_atributes.mq_maxmsg = 10;
    queue_atributes.mq_msgsize = BUFF_SIZE;
    char sent_msg[BUFF_SIZE];
    char recv_msg[BUFF_SIZE];

    //Cola de mensajes entre Productores y Delivery
    qda = mq_open(COLADELIVERYPRODUCTORES, O_RDONLY);
    if (qda == -1) {
        printf("Error al crear/unirse a la cola en el servidor principal: ");
        exit(EXIT_FAILURE);
    }
    RecepcionDeProductores(qda,recv_msg);
    //mq_receive(qda, recv_msg, 8192, NULL);//NULL para la prioridades
    printf("Mensaje recibido %s \n", recv_msg);
    mq_close(qda);


    while (1) {
        scanf("%5000[^\n]s", comando);      //Esperar hasta que el usuario ingrese algun comando
        if (strlen(comando) >
            0) {                 //Actuar solo si comando contiene algo y solo un enter
            SepararArgs();                         //Separar comando de sus argumentos
            if (strcmp(comando, "add") == 0) {  //Si el comando es cd
                if (args[1] &&   args[2]) {                     //Verificar que cuente con el argumento necesario
                    eleccion=args[2];
                    switch (eleccion)
                    {
                    case 'Productor1':
                        /* code */
                        break;
                    
                    case 'Productor2':
                        /* code */
                        break;
                    
                    case 'Productor3':
                        /* code */
                        break;
                    
                    default:
                        break;
                    }




                    // Este comando agregara el socket a una lista correspondiente al servicio, para ser validado.

                   
                } else {
                    printf("Parametros insuficientes");
                }
            } else if (strcmp(comando, "delete") == 0) {  //Si el comando es cd
                if (args[1] && args[2]) {                     //Verificar que cuente con el argumento necesario
                    eleccion=args[2];
                    switch (eleccion)
                    {
                    case 'Productor1':
                        /* code */
                        break;
                    
                    case 'Productor2':
                        /* code */
                        break;
                    
                    case 'Productor3':
                        /* code */
                        break;
                    
                    default:
                        break;
                    }

                    //Este comando borra el host como suscriptor dejando de enviarle mensajes.

                } else {
                    printf("Parametros insuficientes");
                }
            } else if (strcmp(comando, "log") == 0) {  //Si el comando es cd
                if (args[1]) {                     //Verificar que cuente con el argumento necesario

                    //Este comando comprime el log local del DeliveryManager y lo envía a <socket>.

                } else {
                    printf("Parametros insuficientes");
                }
            }

        }
        return 0;
    }

}

