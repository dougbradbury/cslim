#include <stdlib.h>

#include "SerialComLink.h"
#include "WProgram.h"
//static local variables
struct SerialComLink
{
};

SerialComLink* SerialComLink_Create()
{
 // SerialComLink* self = (SerialComLink*)malloc(sizeof(SerialComLink));
 // memset(self, 0, sizeof(SerialComLink));
 // self->client = client;
 return 0;
}

void SerialComLink_Destroy(SerialComLink* self)
{
    // free(self);
}


int SerialComLink_send(void * voidSelf, char * msg, int length)
{
  Serial.write((uint8_t*)msg, length);
  return length;
} 



int SerialComLink_recv(void * voidSelf, char * buffer, int length)
{
  int bytes_read = 0;

  while(Serial.available() <= 0)
  {
    delay(100);
  }
  
  while(bytes_read < length) {
    while(Serial.available() <= 0);
    buffer[bytes_read] = (char) Serial.read();    
    bytes_read += 1;
  }

  return bytes_read;
}
