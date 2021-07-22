#
CFLAGS+= -lcrypto -g

all: mining-key
	./mining-key -b 30 -t 3 -n 5 secret-matthias-2022-05-07
	./mining-key -h

clean:
	rm -f mining-key mining-key.o
