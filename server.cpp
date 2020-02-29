/*
Name: Alan Radda Jr.
NUID: 54265365
Class: CSCI 3550
Assignment: Project 1: "Accio" File using TCP
Date: 2/21/20
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <fstream>

#define MAX_BYTES 1024

bool file_created(const std::string& file); //function declaration

int
main(int argc, char* argv[])
{

    if (argc < 3) { // check for valid number of arguments
        std::cerr << "ERROR: Please provide port and directory.\n";
        exit(1);
    }

    // create a socket using TCP IP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int pnum = atoi(argv[1]); //get port number from command-line

    if (pnum <= 1023) { // check validity of port number
        std::cerr << "ERROR: Incorrect port. Port must be greater than 1023\n";
        exit(1);
    }


    // allow others to reuse the address
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        return 1;
    }

    // bind address to socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(pnum);     // short, network byte order
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // use any interface
    memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

    if (bind(sockfd, (struct sockaddr*) & addr, sizeof(addr)) == -1) {
        perror("bind");
        return 2;
    }

    // set socket to listen status
    if (listen(sockfd, 15) == -1) {
        perror("listen");
        return 3;
    }

    // accept a new connection
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSockfd = accept(sockfd, (struct sockaddr*) & clientAddr, &clientAddrSize);

    if (clientSockfd == -1) {
        perror("accept");
        return 4;
    }

    char ipstr[INET_ADDRSTRLEN] = { '\0' };
    inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
    std::cout << "Accept a connection from: " << ipstr << ":" <<
        ntohs(clientAddr.sin_port) << std::endl;

    std::string directory = argv[2]; //get directory from command-line argument
    int remove = 1; // delete / from path
    int count = 1; //counter
    std::string filename = std::to_string(count); //set filename
    std::string path = directory + "/" + filename + ".file"; // set path with directory and filename
    bool file_create = true; // flag for while loop

    while (file_create)
    {
        if (!file_created(path)) { //directory does not exist
            std::ofstream write_file(path.c_str() + remove, std::ios::binary); // create file with directory and file
            file_create = false; // set flag to false
        }
        else {
            count++; // increment counter
            filename = std::to_string(count); // file exists, create filename with next increment
            path = directory + "/" + filename + ".file"; //set filename to path
        }
    }

    // read/write data from/into the connection  
    std::ofstream write_file(path.c_str() + remove, std::ios::binary);
    int filesize = write_file.end; //get size of file
    char buf[MAX_BYTES] = { 0 }; // create buffer to store data
    int bytes; //store number of bytes

    while (filesize) { // while filesize is greater than 0
    
        bytes = recv(clientSockfd, buf, MAX_BYTES, 0); //recieve data and store into buffer

        if (bytes < 0) { // if no data print error
            perror("recv");
        }
        
        write_file.write(buf, bytes); //write data from buffer into file
       
        filesize -= bytes; //decrease
    }

  close(clientSockfd);

  return 0;
}

bool file_created(const std::string& file) // function used to check if file exists
{
    struct stat check;

    if (stat(file.c_str() + 1, &check) != -1) { //used to check and increment file name
        return true;
    }
    return false;
}