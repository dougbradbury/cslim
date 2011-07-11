#include "Ethernet.h"

#include <stdlib.h>


#include "TcpComLink.h"

//static local variables
struct TcpComLink
{
  Client * client;
};

TcpComLink* TcpComLink_Create(Client * client)
{
 TcpComLink* self = (TcpComLink*)malloc(sizeof(TcpComLink));
 memset(self, 0, sizeof(TcpComLink));
 self->client = client;
 return self;
}

void TcpComLink_Destroy(TcpComLink* self)
{
    free(self);
}


int TcpComLink_send(void * voidSelf, char * msg, int length)
{
  TcpComLink * self = (TcpComLink *)voidSelf;
  self->client->write((uint8_t*)(msg), length);
  return length;
} 


int TcpComLink_recv(void * voidSelf, char * buffer, int length)
{
  TcpComLink * self = (TcpComLink *)voidSelf;
  int bytes_read = 0;

  Client * client = self->client;
  while(client->available() > 0 && bytes_read < length) {
    buffer[bytes_read] = client->read();
    bytes_read += 1;
  }

  return bytes_read;
}
