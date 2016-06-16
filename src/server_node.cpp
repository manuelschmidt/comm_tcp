#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main (int argc, char** argv)
{
  ros::init(argc, argv, "server_node");
  ros::NodeHandle nh;
  //Testing package is working fine
  cout << "Hello there! This node is listening on port 1024 for incoming connections" << endl;
  int sockfd, newsockfd, portno = 1024; //Socket file descriptors and port number
  socklen_t clilen; //object clilen of type socklen_t
  char buffer[256]; //buffer array of size 256
  struct sockaddr_in serv_addr, cli_addr; ///two objects to store client and server address
  int n;
  ros::Duration d(0.01); // 100Hz
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
      error("ERROR opening socket");
  int enable = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
      error("setsockopt(SO_REUSEADDR) failed");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0)
            error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd,
              (struct sockaddr *) &cli_addr,
              &clilen);
  if (newsockfd < 0)
       error("ERROR on accept");
  while(ros::ok()) {
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     //close(newsockfd);
     //close(sockfd);
     //ros::spinOnce();
     //d.sleep();
  }
  return 0;
}
