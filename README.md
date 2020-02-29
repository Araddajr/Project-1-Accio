#Name: Alan Radda Jr.
#NUID: 54265365
#Class: CSCI 3550
#Assignment: Project 1: "Accio" File using TCP
#Date: 2/21/20

High Level Design:

server.cpp is a TCP server program that takes in a port number and directory from the command-line.
A socket is created and then the port number and ip address is binded to the socket.
Next, the socket is set to listen for incoming connections.
If a valid connection is found, the server accepts the connection. 
Then the server checks for the filename 1.file. If 1.file does not exist, 1.file is created in the passed in directory from the command-line.
If 1.file does exist, the filename is incremented by 1 and created in the directory (ex. 2.file, 3.file, ect.).
Whlie the filesize is greater than 0, data is recieved from the client and written into the file. 

client.cpp is a TCP client program that takes in a hostname, port number, and a file from the command-line.
A socket is created and then the port number and ip address is binded to the socket.
Next, the client attempts to connect to the server.
Upon a successful connection, the client sends the data from the file to the server.
The client stops sending data when the end of the file is reached.

Problems:

I have never used C++ before, and have barely done any C, so learning the syntax of C++ was challenging.
I was unable to figure out multithreading or implementing a timeout.
Truth be told, my problems ran from figuring out how to take in command-line arguments to how to use a filestream.
As I said, the majority of my problems stemmed from being unfamiliar with C++ and a lack of understanding the low-level programming. 

I did my best to solve problems by regularly checking piazza, scouring the internet for examples, and I found a minor amount of help in the CSLC.

Going forward, it is clear to me that I needed to meet with either the professor or the TA to seek guidance on how to implement this project.
Unfortunately, I did not anticipate how difficult I would find this project, and did not coordinate my schedule to allow time for meeting with the professor or TA.
For the next project, I plan on changing that and changing my schedule so that I can hopefully have more success.

Additional libraries:

I used the <fstream> library for file streaming (ifstream, ofstream).
I used the <netdb.h> library for resolving the host (gethostbyname).

Acknowledgements:

As stated above, I regulary checked piazza and visited the CSLC. I also looked at various examples on stackoverflow.com and cplusplus.com.