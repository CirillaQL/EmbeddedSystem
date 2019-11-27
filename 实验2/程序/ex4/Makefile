CC = /opt/host/armv4l/bin/armv4l-unknown-linux-gcc


CFLAGS =  -I/arm830_linux2410tft/kernel/include -Wall -Wstrict-prototypes -Wno-trigraphs -Os -mapcs -fno-strict-aliasing -fno-common -fno-common -pipe -mapcs-32 
      
kbd: ex4.c
	$(CC) $(CFLAGS) -o ex4  ex4.c
	cp ex4 /home/nfs
clean:
	-rm -f *.o ex4
