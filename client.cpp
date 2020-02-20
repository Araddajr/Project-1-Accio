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

using namespace std;

int
main(int argc, char*argv[])
{
  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  int pnum = atoi(argv[2]);

  if (pnum <= 1023) {
      std::cerr << "ERROR: Incorrect port. Port must be greater than 1023\n";
      exit(1);
  }

  // struct sockaddr_in addr;
  // addr.sin_family = AF_INET;
  // addr.sin_port = htons(40001);     // short, network byte order
  // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  // memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));
  // if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
  //   perror("bind");
  //   return 1;
  // }

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(pnum);     // short, network byte order
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

  // connect to the server
  if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
    perror("connect");
    return 2;
  }

  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
    perror("getsockname");
    return 3;
  }

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
  std::cout << "Set up a connection from: " << ipstr << ":" <<
    ntohs(clientAddr.sin_port) << std::endl;

  /*
  char buffer[100];

  FILE* f;

  f = fopen("send.txt", "r");
  
  while (!feof(f)) {

      fgets(buffer, sizeof(buffer), f);

     // if (fgets(buffer, sizeof(buffer), f) == -1) { //(fscanf(f, "%s %99[^\n]", buffer) == -1) {
       //   std::cerr << "ERROR on fscanf";
         // exit(1);
      //}

      if (write(sockfd, buffer, 100) == -1) {
          std::cerr << "ERROR on write";
          exit(1);
      }
  }
  printf("the file was sent successfully\n");
  
  */

  /*
  //std::string file_name = argv[3];
  std::ifstream read (argv[3]);
  //std::cout << read;
  string line; 
  //char buf[1024] = {0};

  if (read.is_open())
  {
      while (!read.eof())
      {
          getline(read, line);

         // cout << line << endl;

          
          //memset(buf, '\0', sizeof(buf));
          //if (read.getline(buf, sizeof(buf)) != NULL);

          if (send(sockfd, line.c_str(), line.size(), 0) == -1)
          {
              std::cerr << "Error on send file";
          }
          
          //if (recv(sockfd, buf, sizeof(buf), 0) == -1)
          //{
              //std::cerr << "Error on recv";
          //}
      }
  }
  read.close();
  */
 

  
  
  // send/receive data to/from connection
  bool isEnd = false;
  //std::string input;
  char buf[20] = {0};
  std::stringstream ss;
  std::ifstream read(argv[3]);
  string line;

  while (!isEnd) {
    memset(buf, '\0', sizeof(buf));

    //std::cout << "send: ";
    //std::cin >> input;



    getline(read, line);
    if (send(sockfd, line.c_str(), line.size(), 0) == -1) {
      perror("send");
      return 4;
    }


    if (recv(sockfd, buf, 20, 0) == -1) {
      perror("recv");
      return 5;
    }
    //ss << buf << std::endl;
    //std::cout << "echo: ";
    //std::cout << buf << std::endl;

    if (ss.str() == "close\n")
      break;

    ss.str("");
  }

  


  close(sockfd);

  return 0;
}