CC=gcc

makenicebit:
	$(CC) -o niceBit niceBit.c -lcrypto -lsecp256k1
