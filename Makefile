CC= gcc
CFLAGS= -Ilibev
PUDUOBJ= 
HEADERS= 
ifndef TARGET
TARGET= pudu 
PUDUOBJ+= obj/pudu.o obj/log.o
endif

obj/%.o: %.c $(HEADERS)
	-@mkdir obj
	$(CC) -c -o $@ $(CFLAGS) $(filter %.c, $^)

all: $(PUDUOBJ)
	-@mkdir bin
	$(CC) $(LDFLAGS) $(PUDUOBJ) -o bin/$(TARGET)

install: all
	@rm -f /usr/bin/$(TARGET)
	@cp bin/$(TARGET) /usr/bin

clean:
	@rm -rf bin obj
