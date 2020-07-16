#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include "base58.h"

typedef unsigned char byte;

int is_hex(const char *s) {
	int i;
	for (i = 0; i < 64; i++)
		if (!isxdigit(s[i])) return 0;
	return 1;
}

void str_to_byte(const char *src, byte *dst, int n) {
	while (n--) sscanf(src + n * 2, "%2hhx", dst + n);
}

char *pubkey_to_P2PKH(const unsigned char *pubkey64, char *out) {
	byte s[65];
	byte rmd[5 + RIPEMD160_DIGEST_LENGTH];

	int j;
	for (j = 0; j < 65; j++) {
		s[j] = pubkey64[j];
	}

	rmd[0] = 0;
	RIPEMD160(SHA256(s, 65, 0), SHA256_DIGEST_LENGTH, rmd + 1);

	memcpy(rmd + 21, SHA256(SHA256(rmd, 21, 0), SHA256_DIGEST_LENGTH, 0), 4);

	base58(rmd, 25, out, 34);

	/* Count the number of extra 1s at the beginning of the address */
	int k;
	for (k = 1; out[k] == '1'; k++);

	/* Count the number of extra leading 0x00 bytes */
	int n;
	for (n = 1; rmd[n] == 0x00; n++);

	/* Remove k-n leading 1's from the address */
	memmove(out, out + (k-n), 34-(k-n));
	out[34-(k-n)] = '\0';
}
