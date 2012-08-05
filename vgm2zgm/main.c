#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

unsigned char get(void) {
 unsigned char ret;
 if(fread(&ret, 1,1 ,stdin)!=1)
  if(feof(stdin))
   exit(0);
 return ret;
}

unsigned int globl_delay = 0;

void delay(unsigned int in) {
 if((in == -1)) {
  if(globl_delay == 0) 
   return;
  putchar(0x61);
  putchar(globl_delay&0xff);
  putchar((globl_delay>>8)&0xff);
  globl_delay = 0;
  return;
 }
 if((globl_delay+in)>0xffff) {
  putchar(0x61);
  putchar(globl_delay&0xff);
  putchar((globl_delay>>8)&0xff);
  globl_delay= in;
  return;
 }
 globl_delay+=in;
}

int main(void) {
 int i;
 unsigned int in;
 unsigned int u32;
 unsigned char opcode;

 for(i=0;i<0x40;i++) 
  get();

 for(;;) {
  opcode = get();
  if((opcode&0xf0)==0x70) {
   delay(opcode&0x0f);
  }
  if((opcode&0xf0)==0x80) {
   delay(opcode&0x0f);
  }

  switch(opcode) {
   case 0x50:
    delay(-1);
    putchar(0x50);
    putchar(get());
    putchar(0);
    break;
   case 0x52:
    delay(-1);
    putchar(0x52);
    putchar(get());
    putchar(get());
    break; 
   case 0x53:
    delay(-1);
    putchar(0x53);
    putchar(get());
    putchar(get());
    break; 
   case 0x61:
    in = (unsigned int)get();
    in|= (unsigned int)get() << 8;
    delay(in);
    break;
   case 0x62:
    delay(0x2df);
    break; 
   case 0x63:
    delay(0x372);
    break;
   case 0x66:
    putchar(0x66);
    putchar(0);
    putchar(0);
    exit(0);
    break;
   case 0x67:
    get();
    get();
    u32 = (unsigned int)get();
    u32|= (unsigned int)get()<<8;
    u32|= (unsigned int)get()<<16;
    u32|= (unsigned int)get()<<24;
    for(i=0;i<u32;i++)
     get();
    break;
   case 0xe0:
    u32 = (unsigned int)get();
    u32|= (unsigned int)get()<<8;
    u32|= (unsigned int)get()<<16;
    u32|= (unsigned int)get()<<24;
    break;

  }
 }

 
}
