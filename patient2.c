#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define HEALTHCENTERSERVER 21440
struct doc{
	char name[50];
	char port_no[50];
};
main()
{
	struct sockaddr_in server,client,doctor;
	struct doc d;
	int s,sock,getsock_check,sa_len,i,j,flag=0,k=0,match=0,doc_port,recvlen,slen,fd,rc;
	char b1[100]="",b2[100]="";
	char credentials[1000];
	char username[50];
	char pwd[50];
	char slots[100][100];
	char *token;
	char sel[100];
	char times[100];
	char insurance[100];
	char buf[100];
	FILE *fp;
	memset(credentials,0,sizeof(credentials));
	memset(username,0,sizeof(username));
	memset(pwd,0,sizeof(pwd));
	memset(slots,0,sizeof(slots));
	memset(sel,0,sizeof(sel));
	memset(times,0,sizeof(times));
	memset(insurance,0,sizeof(insurance));
	memset(buf,0,sizeof(buf));
	fp = fopen("patient2.txt","r");
	if(NULL == fp)
    {
        printf("\n fopen() Error!!!\n");
        return 1;
    }
    j=0;
    if(fgets(credentials,1000,fp)!=NULL)
    {
		for(i=0;i<strlen(credentials);i++)
		{
			if(credentials[i]==' ')
			  {
				   flag=1;
				   j=0;
			  }
	     if(flag==0 && credentials[i]!=' ')		
		   username[j++]=credentials[i];
		 else
		 {
			  
			 pwd[j++]=credentials[i];   
	     }
	    }
	}
	fclose(fp);
	char authentication[200];
	memset(authentication,0,sizeof(authentication));
	strcpy(authentication,"authenticate ");
	strcat(authentication,credentials);
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");
	client.sin_port=htons(0);
	s=socket(AF_INET,SOCK_STREAM,0);
	sa_len = sizeof(client);
	
	server.sin_family=AF_INET;
	server.sin_port=htons(HEALTHCENTERSERVER);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	connect(s,(struct sockaddr *)&server,sizeof(server));
	getsock_check=(getsockname(s,(struct sockaddr *)&client,(socklen_t *)&sa_len));
	printf("\nPhase 1:Patient 2 has TCP port number %d and IP address %s\n",(int) ntohs(client.sin_port),inet_ntoa(client.sin_addr));	
		send(s,authentication,sizeof(authentication),0);
		printf("\nPhase 1:Authentication request from Patient 2 with username %s and password %s has been sent to the Health Center Server",username,pwd);
		recv(s,b1,sizeof(b1),0);
		printf("\nPhase 1:Patient 2 Authentication result:%s\n",b1);
			fflush(stdout);
		printf("\nPhase 1: End of Phase 1 for Patient 2\n");	
		if(strcmp(b1,"failure")==0) {
		  exit(0);
	  }
		else// if(strcmp(b1,"success")==0)
		{
		  close(s);
		  s=socket(AF_INET,SOCK_STREAM,0);
		  server.sin_family=AF_INET;
	      server.sin_port=htons(HEALTHCENTERSERVER);
	      server.sin_addr.s_addr=inet_addr("127.0.0.1");
	      int pq=connect(s,(struct sockaddr *)&server,sizeof(server));
	      //printf("connect status: %d",pq);
	      getsock_check=(getsockname(s,(struct sockaddr *)&client,(socklen_t *)&sa_len));
	      //printf("%d",(int) ntohs(client.sin_port));
	     // printf("hello..");
	      strcpy(b2,"available");
	      int vv=send(s,b2,sizeof(b2),0);
	      //printf("avail:%d",vv);
	      int pp=recv(s,b1,sizeof(b1),0);
	      strcpy(times,b1);
	      if(strcmp(b1,"notavailable")==0)
	       close(s);
	      else{  
	      token=strtok(times,"\n");
	      
	      while(token!=NULL)
	      {
			  strcpy(slots[k++],token);
			  token=strtok(NULL,"\n");
		  }
	      
	      printf("\nPhase 2:The following appointments are available for Patient 2\n %s",b1);
	      printf("\nPlease enter the preferred appointment index and press enter:");
	      while(1)
	      {
			  scanf("%s",b2);
			  for(i=0;i<k;i++)
	         {
			  token=strtok(slots[i]," ");
			  if(strcmp(b2,token)==0)
			   { match=1;
				   break;
			   }
		   }
			   if(match==1)
			      break;
			     
			      printf("Re enter the choice");
		  }
		  strcat(sel,"selection ");
		  strcat(sel,b2);
		  //printf("selection is %s",sel);
		  send(s,sel,sizeof(sel),0);
	      recv(s,b1,sizeof(b1),0);
	     // printf("%s",b1);
	      strcpy(times,b1);
	      token=strtok(times," ");
	      if(strcmp(token,"notavailable")==0)
	       { 
			   printf("\nPhase 2:The requested appointment from Patient 2 is not available.Exiting..\n");
			   close(s);
			   exit(0);
		   }
	      else
	      {
			 //printf("%s...",token);
			  strcpy(d.name,token);
			  token=strtok(NULL," ");
			  strcpy(d.port_no,token);
			  doc_port=atoi(d.port_no)+440;
			  sprintf(d.port_no,"%d",doc_port);
			  
			  printf("\nPhase 2:The requested appointment is available and reserved to Patient2.The assigned doctor port no is %s",d.port_no);
		      close(s);
		      fp=fopen("patient2insurance.txt","r");
		      if(fgets(insurance,1000,fp)!=NULL)
		      {
			  }
			  fd=socket(AF_INET,SOCK_DGRAM,0);
		       
		       rc = bind (fd, (struct sockaddr *) &client,sizeof(client));
	           if(rc<0) {
                printf("\nCannot Bind\n");
               exit(0);
                }
                getsock_check=(getsockname(fd,(struct sockaddr *)&client,(socklen_t *)&sa_len));
		       printf("\nPhase 3:Patient 2 has the dynamic port number %d and IP address %s\n",(int) ntohs(client.sin_port),inet_ntoa(client.sin_addr));
	         
	          doctor.sin_family = AF_INET;
	          doctor.sin_addr.s_addr =inet_addr("127.0.0.1");;
	          doctor.sin_port = htons(doc_port);
	          slen=sizeof(doctor);
	          if (sendto(fd,insurance, strlen(insurance), 0, (struct sockaddr *)&doctor, slen)==-1) {
			    perror("sendto");
			    exit(1);
		      }
		      printf("\nPhase 3:The cost estimation request  from Patient 2 with insurance plan %s has been sent to the doctor with port number %s and IP address %s\n",insurance,d.port_no,inet_ntoa(doctor.sin_addr));
		      recvlen = recvfrom(fd, buf,1000, 0, (struct sockaddr *)&doctor, &slen);
                if (recvlen >= 0) {
                        buf[recvlen] = 0;	/* expect a printable string - terminate it */
                        memset(token,0,sizeof(token));
                        
                        printf("\nPhase 3:Patient 2 receives  %s$ estimation cost from doctor with port number %s and name %s \n",buf,d.port_no,d.name);
                        fflush(stdout);
                        printf("\nPhase 3:End of Phase 3 for Patient 2\n");
                       // printf("received message: \"%s\"\n", buf);
                       close(fd);
                }
		}
	}
}
}
		
		 

