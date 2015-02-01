#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#define DOC1_PORT 41440
main()
{
	struct sockaddr_in patient,doctor1;
	struct hostent *hp;
	struct in_addr ip_addr; 
	int sockfd,rc,patientlen,n,t=0,pid,i,port;
	char msg[255],text[255],ins_details[100];
	char *token;
	char ins[100][100];
	char price[100][100];
	FILE *fp;
	memset(ins,0,sizeof(ins));
	memset(price,0,sizeof(price));
	memset(msg,0,sizeof(msg));
	memset(text,0,sizeof(text));
	fp=fopen("doc1.txt","r");
	while(fgets(ins_details,1000,fp)!=NULL)
	{
		token=strtok(ins_details," ");
		strcpy(ins[t],token);
		token=strtok(NULL," ");
		strcpy(price[t],token);
		t++;
	}
	//printf("%s %s",ins[0],price[0]);
    sockfd=socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0)
	 {
      printf("\nSocket Not Created\n");
      exit(0);
   	}
	doctor1.sin_port=htons(DOC1_PORT);
	doctor1.sin_family=AF_INET;
	doctor1.sin_addr.s_addr=inet_addr("127.0.01");
	hp=gethostbyname("localhost");
	ip_addr = *(struct in_addr *)(hp->h_addr);
	rc = bind (sockfd, (struct sockaddr *) &doctor1,sizeof(doctor1));
	if(rc<0) {
      printf("\nCannot Bind\n");
      exit(0);
   }
   else 
    printf("\nPhase 3: Doctor 1 has the static UDP port %d and IP address %s\n",DOC1_PORT,inet_ntoa(ip_addr));
    fflush(stdout);
   for(;;){
    
      
      patientlen = sizeof(patient);
      n = recvfrom(sockfd, msg, 1000, 0,(struct sockaddr *) &patient, &patientlen);
      msg[n]='\0';
       port=(int) ntohs(patient.sin_port);
      printf("\n Phase 3: Doctor 1 receives the request from the patient with port no %d and insurance plan %s\n",port,msg);
      fflush(stdout);
       //printf("%d %s %s",n,msg,ins[0]);
       for(i=0;i<3;i++)
       { token=strstr(msg,ins[i]);
          if(token!=NULL)
          {  strcpy(text,price[i]);
             break;
		 }
           
		}
		text[strlen(text)]='\0';
         rc = sendto(sockfd, text, strlen(text), 0,(struct sockaddr *) &patient,sizeof(patient));
         printf("\nPhase 3: Doctor 1 sent the estimated price  %s $ to patient with port number %d",text,port);
         fflush(stdout);
         
  }

	return ;
}

