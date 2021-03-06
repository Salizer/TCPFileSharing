#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

#define BUFFSIZE 1024

//Uptime function.
char *getUpTime()
{
 FILE * uptimefile;
 char uptime_chr[28]; // 28 ud fra eksemplet
 long uptime = 0;

 if((uptimefile = fopen("/proc/uptime", "r")) == NULL)
  perror("supt"), exit(EXIT_FAILURE);

 bzero(uptime_chr, sizeof(uptime_chr));
 fgets(uptime_chr, 12, uptimefile);
 fclose(uptimefile);

 uptime = strtol(uptime_chr, NULL, 10);
 sprintf(uptime_chr, "%02ld:%02ld:%02ld", uptime/3600, ((uptime % 3600) / 60), ((uptime % 3600) - ((uptime % 3600) / 60) * 60));
 return(strdup(uptime_chr));
}

char* getLoadAvg()
{
    // Variables
    FILE* file;
    char loadAvg_chr[50];
    

    // Open file
    if((file = fopen("/proc/loadavg", "rt")) == NULL) // Open and check if error
		perror("loadAvg"), exit(EXIT_FAILURE);

    // Read file and save in buffer
    bzero(loadAvg_chr, sizeof(loadAvg_chr));
    fgets(loadAvg_chr, 50, file);

    // Clean up
    fclose(file); // Closes file
    
    return(strdup(loadAvg_chr));
}

// Main
int main(int argc, char *argv[])
{
    /////////////////////////////////////////////
    //Definition of structures and variables.
    int sock, length, n;
    socklen_t fromlen;
    struct sockaddr_in server; 
    struct sockaddr_in from;
    char buf[BUFFSIZE+1];

   if (argc < 2) {
      fprintf(stderr, "ERROR: No port provided\n");
      exit(0);
   }
   
   /////////////////////////////////////////////
   //Create socket with protocol UDP
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0)
   {
        printf("Error: Opening socket.\n");
        exit(1);
   }
   
   ////////////////////////////////////////////
   //Theese lines initializes server to zeros.
   length = sizeof(server);
   bzero(&server,length);
   
   ////////////////////////////////////////////
   //Use INTERNET domain (internet sockets).
   server.sin_family=AF_INET;
   //As server, use own IP(From all interfaces)
   server.sin_addr.s_addr=INADDR_ANY;
   //Convert port number to network byte order and save it to structure.
   server.sin_port=htons(atoi(argv[1]));
   
   ///////////////////////////////////////////////
   //The bind() system calls binds a socket to an address (address of the current host and port number).
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
    {
		printf("Error: Binding.\n");
        exit(1);
    }
       
   fromlen = sizeof(struct sockaddr_in);
   
   printf("Server started with port %s\n", argv[1]);
   
   //Continuesly run and accept commands.
   while (1) {
        memset(&buf, 0, sizeof(buf));
        
        //Recieve data from client
		puts("Waiting for client...\n");
        n = recvfrom(sock,buf,BUFFSIZE,0,(struct sockaddr *)&from,&fromlen);
        if (n < 0)
		{
		printf("Error: Recieving.\n");
		exit(1);
		}
		
       //Write what was recieved.
       write(1,"Received a datagram: ",21);
       write(1,buf,n); 
       
       char* tmpBuf;
       // Send data back if ASCII 'u' or 'U'
       if(*buf == 'u' || *buf == 'U')
       {
            memset(&buf, 0, sizeof(buf));
            tmpBuf = getUpTime();
            
            strcpy(buf, tmpBuf);
            
            n = sendto(sock,buf,strlen(buf) + 1,0,(struct sockaddr *)&from,fromlen); // Add +1 because of 0-termination
            if (n < 0)
			{
				printf("Error: Sending.\n");
				exit(1);
			}
                    
            free(tmpBuf); 
        }
        // Send data back if ASCII 'L' or 'l'
        else if(*buf == 'l' || *buf == 'L')
        {
            memset(&buf, 0, sizeof(buf));
            tmpBuf = getLoadAvg();
            
            strcpy(buf, tmpBuf);
            
            n = sendto(sock,buf,strlen(buf) + 1, 0,(struct sockaddr *)&from,fromlen);
			if (n < 0)
			{
				printf("Error: Sending.\n");
				exit(1);
			}
            
            free(tmpBuf); 
        }
        else
        {
			strcpy(buf, "Screw you\n");
			sendto(sock,buf,strlen(buf) + 1, 0,(struct sockaddr *)&from,fromlen);
		}

   }
   return 0;
 }


