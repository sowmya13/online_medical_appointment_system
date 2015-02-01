a)FULL NAME:SOWMYA SAMPATH
b)STUDENT ID:8167173440
c)Completed All the three phases
Phase 1: Health Center Server receives authentication request from the patients, checks it along the users.txt file , and sends the response "success" or "failure" based on the match/no-match of username and password.
Phase 2:If the authentication is successful,patient asks for available slots from the healthcenterserver.Server checks the structure (timeslots) where details from availabilities.txt are stored.If the avail field is "A", then that entry is sent to the patient.
        The Patient replies with the selected index,it is checked by the server if it is reserved or still available.If it is reserved,it sends reply as "notavailable" and if it is available, it sends the doctor name and port number.
Phase 3:Once the patient receives the doctor port number,I have added "440" to the received number to get the working doctor port number and connection is initiated with the doctor.Patient asks for the proce of the insurance being selected by it.
        The docotor checks the file
d)healthcenterserver.c
  doctor1.c
  doctor2.c
  patient1.c
  patient2.c
  MakeFile    
  
  Along with this, I have included all the input text files -"users.txt" , "availabilities.txt", "patient1.txt", "patient2.txt","patient1insurance.txt","patient2insurance.txt", "doc1.txt","doc2.txt" 
  
  All static port numbers are  specified as a constant with #define.
  healhcenterserver.c
  On start up, the healthcenterserver stores the data from "users.txt" to local memory (data structure:array) called users, and from "avaialabilities.txt" to a strucure called timeslot.
  It has one more character member to check if the slot is available or not.Initially it is set as 'A' for all slots, and when reserved it is changed to 'R' for the reserved one.
  When a patient initiates the connection with healthcenter server for first phase, gets authentication reply from the server as "success", and if concurrently another patient is calling server for phase 1, it will be waiting until the former patient is served.
  When the former patient is replying back with selected availability in phase 2, the latter patient will get the reply for authentication and proceeds to next phase.
  
  In second phase , when patient sends "available" string asking for list of appointments, health center server sends the available slots , if no slots are available, healthcenterserver sends "notavailable" and the patient gets terminated on receiving this string.
  Whereas after getting the selection from the patient from the available appointment slots, if the selected id of appointment is not available , the health center sends "notavailable" , the patient displays the received message and exits the connection.
  doctor1.c & doctor2.c
  On start up, the insurance and price details are stored in local memory(data structure:array) called ins and price.
  
  patient1.c & patient2.c
  the username and password are retrieved from the respective input files and stored in the local memory(datastructure:array),that is then passed for authentication.

  Makefile
  to compile and clean 
  
e) Give command: make  
   Run 
   1)./healthcenterserver
   2)./doctor1
   3)./doctor2
   4)./patient1
   5)./patient2

f) The format mentioned in the project description file is followed.
    Authenticate request : "authenticate username password" from patients
    Authentication reply : "success" or "failure".
    Availability request : "available" from patients
    Response             : "Sends the available timeslots with id, day and time ,otherwise "notavailable"
    Selecting slot       : "selection id" from patients
    Appointment Confirmation: "doctorname port number" to patients if available ,otherwise "notavailable" 
    Insurance price request: "insurance name" to doctor with whom appointment is reserved.
    Price estimation reply: "cost" to patient.
g) No idiosyncrasy
h) No reused code.  
       
  
  
