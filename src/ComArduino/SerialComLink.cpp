#include <stdlib.h>

#include "SerialComLink.h"
#include "WProgram.h"
//static local variables
struct SerialComLink
{
};

SerialComLink* SerialComLink_Create()
{
 return 0;
}

void SerialComLink_Destroy(SerialComLink* self)
{
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
