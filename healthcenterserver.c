#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#define HEALTHCENTERSERVER 21440
static struct timeslot{
	char id[10];
	char day[20];
	char time[20];
	char doc[20];
	char port_no[20];
	char avail;
}ts[6];
static struct timeslot *ptr=ts;
char buffer[100][100];

main()
{
	struct sockaddr_in patient,server;
	struct hostent *hp;
	struct in_addr ip_addr; 
	int i,k=0,s,sock,n,j=0,count=0,m=0,p=0,c,flag=0,sa_len,getsock_check,sel;
	FILE *fp,*fp1;
	char *token;
	char  credentials[100],avail[100],users[100][100],b1[100],temp[100][100],b2[100],send_avail[100],selection[100];
	memset(credentials,0,sizeof(credentials));
	memset(buffer,0,sizeof(buffer));
	memset(users,0,sizeof(users));
	memset(temp,0,sizeof(temp));
	memset(avail,0,sizeof(avail));
	memset(send_avail,0,sizeof(send_avail));
	memset(selection,0,sizeof(selection));
	for(i=0;i<6;i++)
	{
		memset(ptr,0,sizeof(ptr));
		ptr++;
	}
	fp=fopen("users.txt","r");
	if(NULL == fp)
    {
        printf("\n fopen() Error!!!\n");
        return 1;
    }
    while(fgets(credentials,1000,fp)!=NULL)
    {
		strcpy(users[k++],credentials);
		memset(credentials,0,sizeof(credentials));
		
	}
	fclose(fp);
	fp1=fopen("availabilities.txt","r");
	if(NULL == fp1)
	{
		printf("\n fopen() Error\n");
		return 1;
	}
	ptr=ts;
	while(fgets(avail,1000,fp1)!=NULL)
	{
		token=strtok(avail," ");
		strcpy(ptr->id,token);
		
		//printf("Id is %s \n",id[t]);
		token=strtok(NULL," ");
		strcpy(ptr->day,token);
		
		//printf("day is %s",day[t]);
		token=strtok(NULL," ");
		
		strcpy(ptr->time,token);
		//printf("time is %s",ts[t].time);
		token=strtok(NULL," ");
		
		strcpy(ptr->doc,token);
		token=strtok(NULL," ");
		strcpy(ptr->port_no,token);
		
		 ptr->avail='A';
		//printf("%s",availi[t]);
	    ptr++;
	    //t++;
	}
	fclose(fp1);
	s=socket(AF_INET,SOCK_STREAM,0);
	server.sin_port=htons(HEALTHCENTERSERVER);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.01");
	hp=gethostbyname("localhost");
	ip_addr = *(struct in_addr *)(hp->h_addr);
	printf("\nPhase 1: The Health Center Server has port number %d and IP address %s\n",HEALTHCENTERSERVER,inet_ntoa(ip_addr));
	bind(s,(struct sockaddr *)&server,sizeof server);
	listen(s,1);
	n=sizeof(patient);
	while(sock=accept(s,(struct sockaddr*)&patient,&n)){
		memset(b1,0,sizeof(b1));
		recv(sock,b1,sizeof b1,0);
		
			
		if(strstr(b1,"authenticate")!=NULL)
		{//printf("%s",b1);
		count=0;
		flag=0;
		j=0;
		memset(buffer,0,sizeof(buffer));
			for (i = 0;b1[i] != '\0';i++)
    {
         if (b1[i] == ' ')
         {   count++; 
			 j=0;
		 }
         else
         buffer[count][j++]=b1[i];
                  
    }  
        
		printf("\nPhase 1: The Health Center Server has received request from a patient with username %s and password %s\n",buffer[1],buffer[2]);
		c=0;
		flag=0;
		for(c=0;c<k;c++)
		{   
			m=0;p=0;
			for(i=0;users[c][i]!='\0';i++)
			{  
				if(users[c][i]==' ')
			    {
			  	  m++;
			  	  p=0;
			    }
			    else
			     temp[m][p++]=users[c][i];
			}
			if(strcmp(buffer[1],temp[0])==0 && strcmp(buffer[2],temp[1])==0)
			{ 
				strcpy(b2,"success");
				send(sock,b2,sizeof(b2),0);
				printf("\nPhase 1: The Health Center Server sends the response %s to patient with username %s\n",b2,buffer[1]);
				// printf("success\n");
			   break;
		     }
		     flag++;
		     memset(temp,0,sizeof(temp));
		}
		if(flag==2)
		{    strcpy(b2,"failure");
		   printf("\nPhase 1:The Health Center Server sends the response %s to patient with username %s\n",b2,buffer[1]);
		send(sock,b2,sizeof(b2),0);
		
	}
	
		
			
	}
	if(strcmp(b1,"available")==0)
	{
		sa_len = sizeof(patient);
		strcpy(send_avail,"");
		getsock_check=(getsockname(s,(struct sockaddr *)&patient,(socklen_t *)&sa_len));
		printf("\nPhase 2:The Health Center Server receives a request for available time slots from patient with port number %d and ip address %s\n",(int) ntohs(patient.sin_port),inet_ntoa(patient.sin_addr));
		ptr=ts;
		for(i=0;i<6;i++)
		{
			//printf("\n..%s\n",availi[i]);
			if(ptr->avail=='A')
			{
				strcat(send_avail,ptr->id);
				strcat(send_avail," ");
				strcat(send_avail,ptr->doc);
				strcat(send_avail," ");
				strcat(send_avail,ptr->time);
				strcat(send_avail,"\n");
				
			}
			ptr++;
		}
		if(strcmp(send_avail,"")==0)
		  strcpy(send_avail,"notavailable");
		send(sock,send_avail,sizeof(send_avail),0);
		printf("\nPhase 2: The Health Center Server sends available time slots to patient with username %s\n",buffer[1]);
		fflush(stdout);
		recv(sock,b2,sizeof(b2),0);
		strcpy(selection,b2);
		//printf("%s",b2);
		token=strtok(selection," ");
		if(strcmp(token,"selection")==0)
		{  token=strtok(NULL," ");
		  //printf("%s",token);
		  sa_len = sizeof(patient);
		  getsock_check=(getsockname(s,(struct sockaddr *)&patient,(socklen_t *)&sa_len));
		  printf("\nPhase 2: The Health Center Server receives a request for the appointment %s with port number %d and username %s\n",token,(int) ntohs(patient.sin_port),buffer[1]);
		  fflush(stdout);
		  strcpy(b2,"notavailable");
	     //Refer here (updating to R)
	     ptr=ts;
	    
		 for(i=0;i<6;i++)
		 {
			 if((strcmp(token,ptr->id)==0)&&(ptr->avail=='A'))
			  {   ptr->avail='R';
				  //strcpy(availi[i],"R");c p
				  //printf("%s",availi[i]);
				  strcpy(b2,ptr->doc);
				  strcat(b2," ");
				  strcat(b2,ptr->port_no);
				  sel=1;
				  break;
			  }	 
			  ptr++;
		 }
		 if(sel==0)
		   printf("\nPhase 2: The Health Center Server rejects the following appointment  %s to patient with username %s\n",token,buffer[1]);
		 else  
		 printf("\nPhase 2: The Health Center Server confirms the following appointment %s to patient with username %s\n",token,buffer[1]);
		 fflush(stdout);
		 send(sock,b2,sizeof(b2),0);
		
	}
}	
			
}
}
