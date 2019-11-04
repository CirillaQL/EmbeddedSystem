CC = /opt/host/armv4l/bin/armv4l-unknown-linux-gcc


CFLAGS =  -I/arm830_linux2410tft/kernel/include -Wall -Wstrict-prototypes -Wno-trigraphs -Os -mapcs -fno-strict-aliasing -fno-common -fno-common -pipe -mapcs-32 


EXTRA_LIBS += -lpthread

EXEC = ./httpd
OBJS = httpd.o copy.o

#HTTPD_DOCUMENT_ROOT = /mnt/yaffs
HTTPD_DOCUMENT_ROOT = /root
CFLAGS += -DHTTPD_DOCUMENT_ROOT=\"$(HTTPD_DOCUMENT_ROOT)\"

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(EXTRA_LIBS)



clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

