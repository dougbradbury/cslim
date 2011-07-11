#ifndef SERIAL_COM_LINK_H
#define SERIAL_COM_LINK_H

typedef struct SerialComLink SerialComLink;

SerialComLink* SerialComLink_Create();

void SerialComLink_Destroy(SerialComLink* self);
int SerialComLink_send(void * voidSelf, char * msg, int length);
int SerialComLink_recv(void * voidSelf, char * buffer, int length);

#endif