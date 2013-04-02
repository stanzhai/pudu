CC= gcc
CFLAGS= 
LDFLAGS= -Wl,-Bstatic -lev -Wl,-Bdynamic
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
	$(CC) $(PUDUOBJ) $(LDFLAGS) -o bin/$(TARGET)

install: all
	@rm -f /usr/bin/$(TARGET)
	@cp bin/$(TARGET) /usr/bin

clean:
	@rm -rf bin obj
