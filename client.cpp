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
#include <netdb.h>

int
main(int argc, char* argv[])
{
    if (argc < 4) { // check for valid number of arguments
        std::cerr << "ERROR: Please provide hostname, port, and filename.\n";
        exit(1);
    }

    struct hostent* server = gethostbyname(argv[1]); // resolve host from command-line

    if (server == NULL) { // check validity of host
        std::cerr << "ERROR: Incorrect host\n";
        exit(1);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int pnum = atoi(argv[2]); // get port number from command-line

    if (pnum <= 1023) { // check validity of port number
        std::cerr << "ERROR: Incorrect port. Port must be greater than 1023\n";
        exit(1);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(pnum);     // short, network byte order
    serverAddr.sin_addr.s_addr = *(long*)(server->h_addr_list[0]); // use host from command-line
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    // connect to the server
    if (connect(sockfd, (struct sockaddr*) & serverAddr, sizeof(serverAddr)) == -1) {
        perror("connect");
        return 2;
    }

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    if (getsockname(sockfd, (struct sockaddr*) & clientAddr, &clientAddrLen) == -1) {
        perror("getsockname");
        return 3;
    }

    char ipstr[INET_ADDRSTRLEN] = { '\0' };
    inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
    std::cout << "Set up a connection from: " << ipstr << ":" <<
        ntohs(clientAddr.sin_port) << std::endl;

  // send/receive data to/from connection
  bool isEnd = false;
  char buf[1024] = {0}; // create buffer
  std::ifstream read_file(argv[3], std::ios::binary); //read file argument from command-line in binary format

  while (!isEnd) {
    memset(buf, '\0', sizeof(buf));

    read_file.read(buf, sizeof(buf)); // read file 

    if (send(sockfd, buf, sizeof(buf), 0) == -1) { //send data stored into buffer
      perror("send");
      return 4;
    }

    if (read_file.eof()) { // reached end of the file, break out of loop   
        break;
    }
  }

  close(sockfd);

  return 0;
}