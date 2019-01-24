#include "SocketServer.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//static local variables
struct SocketServer
{
  int (*handler)(int);
};

SocketServer* SocketServer_Create(void)
{
  SocketServer* self = (SocketServer*)malloc(sizeof(SocketServer));
  memset(self, 0, sizeof(SocketServer));
  return self;
}

void SocketServer_Destroy(SocketServer* self)
{
  free(self);
}

#define BACKLOG 10 // how many pending connections queue will hold

void sigchld_handler(int s)
{
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr* sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void SocketServer_register_handler(SocketServer* self, int (*handlerFunction)(int))
{
  self->handler = handlerFunction;
}

void serve(SocketServer* self, int socket)
{
  (*self->handler)(socket);
}

int SocketServer_Run(SocketServer* self, char* listening_port_number)
{
  int                     sockfd, new_fd; // listen on sock_fd, new connection on new_fd
  struct addrinfo         hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t               sin_size;
  struct sigaction        sa;
  int                     yes = 1;
  char                    s[INET6_ADDRSTRLEN];
  int                     rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, listening_port_number, &hints, &servinfo)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and bind to the first we can
  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
    {
      perror("server: socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
      perror("setsockopt");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
    {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  if (p == NULL)
  {
    fprintf(stderr, "server: failed to bind\n");
    return 2;
  }

  freeaddrinfo(servinfo); // all done with this structure

  if (listen(sockfd, BACKLOG) == -1)
  {
    perror("listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1)
  {
    perror("sigaction");
    exit(1);
  }

  sin_size = sizeof their_addr;
  new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
  if (new_fd == -1)
  {
    perror("accept");
    return -1;
  }

  inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*)&their_addr), s, sizeof s);

  serve(self, new_fd);

  close(new_fd);

  return 0;
}
