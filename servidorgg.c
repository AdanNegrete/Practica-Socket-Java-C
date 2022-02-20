#include <unistd.h>

/* librerias para sockets */
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

/* librerias de strings */
#include <errno.h>
#include <stdio.h>
#include <string.h>

/* creación de parametros del servidor */

#define BUFFER_LN	1000			//Tamaño máximo de entrada y salida de datos del buffer
#define BACKLOG	2			//No Max de clientes

int main(){
	
	int sockfd, connfd;			/* descriptores de conexión del socket y del cliente */
	unsigned int len;			/* longitud de la dirección del cliente */
	struct sockaddr_in servidor, cliente; /* estructura de servidor y de cliente para crear el socjet */
	
	int len_rx, len_tx=0; 			/* longitud de mensajes enviados y revividos */
	char dir_ip[12]="";
	char puerto[10]="";
	char buff_tx[BUFFER_LN] = "";
	char buff_rx[BUFFER_LN] = ""; 	/* buffer de recepción */
	
	/* Inciailizando servidor */
	printf("\nIP del servidor: ");
	gets(dir_ip);
	printf("\nPuerto del servidor: ");
	gets(puerto);
	
	/* creación del socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if(sockfd == -1){	
		fprintf(stderr, "Error: No se conecto el servidor. %d: %s \n",errno, strerror(errno));
		return -1;
	}else{
		printf("Server: Socket creado\n");
	}
	
	/* Limpiando la estructura */
	memset(&servidor,0, sizeof(servidor));
	
	/* Inicializando estructura */
	servidor.sin_family		= AF_INET;
	servidor.sin_addr.s_addr	= inet_addr(dir_ip);
	servidor.sin_port		= htons(atoi(puerto));
	
	/* asignación de socket */
	if ((bind(sockfd, (struct sockaddr *)&servidor, sizeof(servidor))) != 0) {
		fprintf(stderr, "Error: Asignación de socket fallida. %d: %s \n",errno, strerror(errno));
		return -1;
	}else{
		printf("Servidor: Socket asignado\n");
	}
	
	/* Socket escuchando */
	if((listen(sockfd,BACKLOG))!=0){
		fprintf(stderr, "Error: No se puede escuchar. %d: %s \n",errno, strerror(errno));
		return -1;
	}else{
		printf("Servidor: En linea");
	}
	
	
	len = sizeof(cliente); /* Guardar el tamaño en bites de la estructura del cliente */
	
	/* Inicializando servidor */
	
	int numero=1,resultado;
	int check=0;
		
	while(numero!=0){
	
		connfd = accept(sockfd, (struct sockaddr *)&cliente,&len);
		
		if(check==0)
			printf("\nEsperando conexión: \n");
		
		if(connfd < 0){
			fprintf(stderr, "Error: No se encuentra conexión. %d: %s \n",errno, strerror(errno));
			return -1;
		}else{
			
			if(check==0){	
				printf("Cliente conectado\n");
				check=1;
			}

			/* Leyendo datos del cliente al buffer */
			len_rx = read(connfd, buff_rx, sizeof(buff_rx));
			
			if(len_rx == -1){
				fprintf(stderr, "Error: Datos no leídos. %d: %s \n",errno, strerror(errno));
			}else if(len_rx==0){
				printf("Servidor: Conexión finalizada\n\n");
				close(connfd);
				break;
			}else{
				
				printf("Servidor: El numero recibido es: %s \n", buff_rx);
				numero = atoi(buff_rx);
				
				memset(&buff_rx,'\0',sizeof(buff_rx));
				memset(&buff_tx,'\0',sizeof(buff_tx));
				
				if(numero==0){
					sprintf(buff_tx,"%d",numero);
				}else{
					resultado = numero+1;
					sprintf(buff_tx,"%d",resultado);
				}
				
				write(connfd, buff_tx, strlen(buff_tx));
				
			}
			
		}
		
		close(connfd);
		
	}
	
	close(sockfd);
	
}

