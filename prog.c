/* first edit sdcc source-code src/device/lib/z80/crt0.s and change the 
 * stack pointer to 0x1fff
 *
 */

//#define START	0x3270
#define START	0
#define END	256508

unsigned char *switch_bank_addr = (unsigned char *)0x6000;
unsigned int  *vgm_pos          = (unsigned int *)0x50;
unsigned char *fm1_register     = (unsigned char *)0x4000;
unsigned char *fm1_data         = (unsigned char *)0x4001;
unsigned char *fm2_register     = (unsigned char *)0x4002;
unsigned char *fm2_data         = (unsigned char *)0x4003;
unsigned char *psg_data	        = (unsigned char *)0x7f11;

void bank_switch(unsigned int in);
unsigned char getc(void);

unsigned int addr;
unsigned int addr_bank;

unsigned int data_addr;
unsigned int data_addr_bank;
unsigned int data_pos;

unsigned int store_addr;
unsigned int store_addr_bank;

#define STORE_POS \
 store_addr = addr; \
 store_addr_bank = addr_bank;

#define LOAD_POS \
 addr = store_addr; \
 addr_bank = store_addr_bank; \
 bank_switch(addr_bank);

#define DELAY \
 __asm \
  nop \
  nop \
  nop \
  nop \
  nop \
  nop \
  nop \
  nop \
  nop \
  nop \
 __endasm;

void nop(void) {
}
void main(void) {
 unsigned int i;
 unsigned int store_u16;
 unsigned char opcode, op1, op2, op3, op4;

 *vgm_pos = START;
 addr = 0;
 addr_bank = 1;
 bank_switch(1);

 for(;;) {
  opcode = getc();
  op1 = getc();
  op2 = getc();
  store_u16 = *vgm_pos;
  store_u16++;
  *vgm_pos = store_u16;
//  *vgm_pos++;
  switch(opcode) {
   case 0x50:
    *psg_data = op1;
    break;
   case 0x52:
    *fm1_register = op1;
    *fm1_data = op2;
    break;
   case 0x53:
    *fm2_register = op1;
    *fm2_data = op2;
    break;
   case 0x66:
    for(;;);
    break;
   case 0x61:
    store_u16 = op1;
    store_u16|= op2<<8;
    for(i=store_u16;i!=0;i--) {
     DELAY;
    }
    break;
   
  }
 }



}

void bank_switch(unsigned int in) {
 int i;
 for(i=0;i<9;i++)
  *switch_bank_addr = (in>>i)&1;
}


unsigned char getc(void) {
 unsigned char *base;
 unsigned char ret;


 base = (unsigned char *)addr + 0x8000;
 ret = *base;

 addr++;
 if(addr==0x8000) {
  addr = 0;
  bank_switch(++addr_bank);
 }

 return ret;
}
