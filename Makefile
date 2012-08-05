
prog.h: prog.bin
	printf "#define Z80_PROG_LEN %s\n\n" `wc -c < prog.bin` > prog.h
	bin2head const\ u8\ z80_prog\[\] < prog.bin >> prog.h

prog.bin: prog.ihx
	hex2bin prog.ihx

prog.ihx: prog.c 
	sdcc -mz80 prog.c --stack-loc 0x1fff --data-loc 0xfff 

clean:
	rm -f prog.asm prog.bin prog.ihx prog.lnk prog.lst prog.map prog.noi prog.rel prog.sym crt0.rel prog.h
