INCL := -I ./device/inc/ -I ./driver/inc/
HSRC := $(shell ls src/*.h)
CSRC := $(shell ls src/*.c)
ASRC := $(shell ls src/*.s)

OBJS := obj/dispatch.o $(subst .c,.o,$(subst src/,obj/,$(CSRC))) $(subst .c,.o,$(subst src/,obj/,$(DSRC)))

DSRC := $(shell ls driver/src/*.c)
DOBJ := $(subst .c,.o,$(subst src/,obj/,$(DSRC)))

LNKR := arm-none-eabi-g++ --specs=nano.specs -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -nostartfiles -T ./md407-ram.x -z muldefs

CPLR := arm-none-eabi-gcc -g -O0 -Wall -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fverbose-asm -DSTM32F40_41xxx --specs=nano.specs

OBCP := arm-none-eabi-objcopy -S -O srec

STTY := /dev/ttyUSB0

.PHONY: clean new

bin.elf: $(HDRS) $(OBJS) $(DOBJ)
	$(LNKR) $(INCL) $(OBJS) $(DOBJ) -o bin.elf

bin.s19: bin.elf
	$(OBCP) bin.elf bin.s19

obj/dispatch.o: src/dispatch.s
	$(CPLR) $(INCL) -c $< -o $@

driver/obj/%.o: driver/src/%.c
	$(CPLR) $(INCL) -c $< -o $@

obj/%.o: src/%.c
	$(CPLR) $(INCL) -c $< -o $@


clean:
	rm -f obj/*.o
	rm -f bin.elf
	rm -f bin.s19

new:
	make clean
	make bin.s19

upload: bin.s19
	# Connect to serial tty
	# FIXME: onlcr is *supposed* to fix the line breaks, but it doesn't...
	screen -dmS md407 $(STTY) 115200,onlcr
	
	# load binary (in background)
	screen -S md407 -X stuff "load\n"
	screen -S md407 -X readreg p bin.s19
	screen -S md407 -X paste p &
	
	# attach to screen session
	screen -r md407

