 # makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../timerLib -I../lcdLib
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:wakedemo.elf

#additional rules for files
wakedemo.elf: ${COMMON_OBJECTS} wakedemo.o lcdutils.o lcddraw.o font5x7.o clockstimer.o sr.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^   

lcdutils.o: ../lcdLib/lcdutils.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

lcddraw.o: ../lcdLib/lcddraw.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

font5x7.o: ../lcdLib/font-5x7.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clockstimer.o: ../timerLib/clocksTimer.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

sr.o: ../timerLib/sr.s
	${AS} $< -o $@
load: wakedemo.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
