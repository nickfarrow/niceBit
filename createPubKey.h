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

/*
char *coin_encode(const char *x, const char *y, char *out) {
*/
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

  /* Count the number of 1s at the beginning of the address */
  int n = 0;
  for (n = 0; out[n] == '1'; n++);

  /* Do we need to remove any 1s? */
  if (n > 1) {
  memmove(out, out + (n-1), 34-(n-1));

  out[34-(n-1)] = '\0';
  }

}
