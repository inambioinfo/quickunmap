CFLAGS=-W -Wall -finline-functions -fPIC -std=gnu99 -Wno-unused-result -O2

main: quickunmap

LSAM0119_DIR = libsamtools0119
LSAM0119 = $(LSAM0119_DIR)/libsam.a
$(LSAM0119) :
	make -C $(LSAM0119_DIR) libsam.a

quickunmap : $(LSAM0119)
	gcc $(CFLAGS) -o $@ -I$(LSAM0119_DIR) quickunmap.c $(LSAM0119) -lz -lpthread

clean:
	rm -f quickunmap
