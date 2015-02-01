CC=gcc
CFLAGS = -g 

LIBS = -lsocket -lnsl -lresolv

all: healthcenterserver doctor1 doctor2 patient1 patient2 

healthcenterserver:healthcenterserver.o
	$(CC) -o healthcenterserver healthcenterserver.c $(LIBS)
	
doctor1:doctor1.o
	$(CC) -o doctor1 doctor1.c $(LIBS)
doctor2:doctor2.o
	$(CC) -o doctor2 doctor2.c $(LIBS)
patient1:patient1.o
	$(CC) -o patient1 patient1.c $(LIBS)
patient2:patient2.o
	$(CC) -o patient2 patient2.c $(LIBS)	

		    	
clean:
	rm -rf healthcenterserver doctor1 doctor2 patient1 patient2 healthcenterserver.o doctor1.o doctor2.o patient1.o patient2.o 
