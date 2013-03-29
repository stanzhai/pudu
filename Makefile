CC= gcc
CFLAGS= -Ilibev
PUDUOBJ= 
HEADERS= 
ifndef TARGET
TARGET= pudu 
PUDUOBJ+= obj/pudu.o
endif

%.o: %.c $(HEADERS)
	@mkdir obj
	$(CC) -c -o obj/$@ $(CFLAGS) $(filter %.c, $^)

all: $(PUDUOBJ)
	@rm -rf bin
	@mkdir bin
	$(CC) $(LDFLAGS) $(PUDUOBJ) -o bin/$(TARGET)

install: all
	@rm -f /usr/bin/$(TARGET)
	@cp bin/$(TARGET) /usr/bin

clean:
	@rm -rf bin obj
