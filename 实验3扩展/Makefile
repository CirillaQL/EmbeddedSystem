CC = /opt/host/armv4l/bin/armv4l-unknown-linux-gcc

LD = /opt/host/armv4l/bin/armv4l-unknown-linux-ld

CFLAGS =  -I/arm830_linux2410tft/kernel/include -Wall -Wstrict-prototypes -Wno-trigraphs -Os -mapcs -fno-strict-aliasing 
			-fno-common -fno-common -pipe -mapcs-32 
			
			-D__KERNEL__ -I/arm830_linux2410tft/kernel/include -Wall -Wstrict-prototypes -Wno-trigraphs 
			-Os -mapcs -fno-strict-aliasing -fno-common -fno-common -pipe -mapcs-32 -march=armv4 -mtune=arm9tdmi 
			-mshort-load-bytes -msoft-float -DKBUILD_BASENAME=ex1 -I/opt/host/armv4l/src/linux/include -DMODULE
ex33: ex33.c
	$(CC) $(CFLAGS) -o kbd  kbd.c
	cp ex33 /home/nfs
clean:
	-rm -f *.o ex33