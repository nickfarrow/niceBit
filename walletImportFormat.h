#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>

/* https://en.bitcoin.it/wiki/Wallet_import_format */

char *create_wif(const unsigned char *privatekey) {
	unsigned char newKey[65];
	unsigned char SHAkey[65];
	unsigned char SHAkey2[65];
	unsigned char checksum[11];
	unsigned char combinedKey[75];

	size_t combinedKeySize = 37;
	size_t wifSize = 51;
	char wif[51];

	/* Add 0x80 byte in front */
	newKey[0] = 128;
	for(int i=0; i<32; i++) {
		newKey[i+1] = privatekey[i];
	}

	/* Perform SHA-256 hash on the extended key */
	SHA256(newKey, 33, SHAkey);

	/* Perform SHA-256 hash again on the result */
	SHA256(SHAkey, 32, SHAkey2);

	/* Checksum is first 4 bytes of 2nd SHA*/
	for(int i=0; i<4; i++) {
		checksum[i] = SHAkey2[i];
	}

	/* Append checksum to end of 2nd SHA */
	for(int i=0; i<33; i++) {
		combinedKey[i] = newKey[i];
	}
	for(int i=0; i<4; i++) {
		combinedKey[33+i] = checksum[i];
	}

	/* Encode with base-58 */
	base58(combinedKey, combinedKeySize, wif, wifSize);
	puts(wif);

	return 0;
}
