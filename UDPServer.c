#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

//Uptime function.
char *getUpTime()
{
 FILE * uptimefile;
 char uptime_chr[28];
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
    {
        puts("Could not open /proc/loadavg");
        exit(1);
    }

    // Read file and save in buffer
    bzero(loadAvg_chr, sizeof(loadAvg_chr));
    fgets(loadAvg_chr, 50, file);

    // Clean up
    fclose(file); // Closes file
    
    return(strdup(loadAvg_chr));
}

int main(int argc, char *argv[])
{
    /////////////////////////////////////////////
    //Definition of structures and variables.
    int sock, length, n;
    socklen_t fromlen;
    struct sockaddr_in server; 
    struct sockaddr_in from;
    char buf[1024];

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   /////////////////////////////////////////////
   //Create socket with protocol UDP
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   
   ////////////////////////////////////////////
   //Theese lines initializes server to zeros.
   length = sizeof(server);
   bzero(&server,length);
   
   ////////////////////////////////////////////
   //Use INTERNET domain (internet sockets).
   server.sin_family=AF_INET;
   //As server, use own IP
   server.sin_addr.s_addr=INADDR_ANY;
   //Convert port number to network byte order and save it to structure.
   server.sin_port=htons(atoi(argv[1]));
   
   ///////////////////////////////////////////////
   //The bind() system call binds a socket to an address (address of the current host and port number).
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
       
   fromlen = sizeof(struct sockaddr_in);
   
   //Continuesly run and accept commands.
   while (1) {
        memset(&buf, 0, sizeof(buf));
        
        //Recieve data from client
        n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
        if (n < 0) error("recvfrom");
       
       //Write what was recieved.
       write(1,"Received a datagram: ",21);
       write(1,buf,n);
       
       char* tmpBuf;
       // Send data back if U
       if(*buf == 'u' || *buf == 'U')
       {
            memset(&buf, 0, sizeof(buf));
            tmpBuf = getUpTime();
            
            strcpy(buf, tmpBuf);
            
            n = sendto(sock,buf,strlen(buf) + 1,0,(struct sockaddr *)&from,fromlen);
                    if(n  < 0) error("sendto");
                    
            free(tmpBuf); 
        }
        // Send data back if L
        else if(*buf == 'l' || *buf == 'L')
        {
            memset(&buf, 0, sizeof(buf));
            tmpBuf = getLoadAvg();
            
            strcpy(buf, tmpBuf);
            
            n = sendto(sock,buf,strlen(buf) + 1, 0,(struct sockaddr *)&from,fromlen);
                    if(n  < 0) error("sendto");
            
            free(tmpBuf); 
        }
        else
        {
			strcpy(buf, "Screw you\n");
			sendto(sock,buf,strlen(buf) + 1, 0,(struct sockaddr *)&from,fromlen);
		}
		//if(tmpBuf != NULL)
		//	free(tmpBuf);  
   }
   return 0;
 }


