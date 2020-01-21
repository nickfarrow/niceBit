#include <stdio.h>
#include <secp256k1.h>
#include "create_pubkey.h"

static secp256k1_context *ctx = NULL;

void gen_keypair(unsigned char *seckey, unsigned char *pubaddress) {
	secp256k1_pubkey pubkey;
	/*unsigned char seckey[32];*/
	unsigned char public_key64[65];
	
	size_t pk_len = 65;

	int i = 0;

	ctx = secp256k1_context_create(
			SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

	/* Load secret key */
	FILE *frand = fopen("/dev/urandom", "r");
	fread(seckey, 32, 1, frand);
	fclose(frand);
	
	/* 
	printf("Seckey : ");
	for(int i=0; i<32; i++) {
		printf("%02X", seckey[i]);
	}
	printf("\n\n");
	*/

	/* Verify secret key is valid */
	if (!secp256k1_ec_seckey_verify(ctx, seckey)) {
		printf("Invalid secret key");
	}

	
	/* Create Public Key */
	if (!secp256k1_ec_pubkey_create(ctx, &pubkey, seckey)) {
		printf("Failed to create public key");
	}
	
	/* Serialize Public Key */
	secp256k1_ec_pubkey_serialize(
			ctx,
			public_key64,
			&pk_len,
			&pubkey,
			SECP256K1_EC_UNCOMPRESSED
			);

	/* Print public key */
	/*
	printf("Long Public Key : ");
	for(int i=0; i<65; i++) {
		printf("%02X", public_key64[i]);
	}
	printf("\n\n");
	*/

	/* Generate Public Address
	 * (from create_pubkey.h)
	 */
	coin_encode(public_key64, pubaddress);	
}

int check_vanity(unsigned char *pubaddress) {
	unsigned char compstr[40];
	int j;

	/* For each vanity length
	 * ('len' digits in a row)
	 */
	for(int len=10; len>3; len--) {
		
		/* For each digit 1-9 */
		for(int i=0; i<10; i++) {
			
			/* Comprise compstr of 'len' repeats
			 * of digit 'i'
			 */
			j = 0;
			while(j<len) {
				/* Offset digit i by 48
				 * as 48 is 0 in ASCII*/
				compstr[j] = i + 48;
				j++;
			}
			
			/* End string with null char*/
			compstr[j] = '\0';
			
			/*printf("Here is that string... %s\n", compstr);*/

			/* Check if string in pubaddress */
			if(strstr(pubaddress, compstr) != NULL) {
				return 1;
			}
		}
	}

	return 0;
}

int main() {
	unsigned char seckey[32];
	unsigned char pubaddress[40];
	
	while(1) {
		gen_keypair(seckey, pubaddress);
	
		if(check_vanity(pubaddress)) {
			printf("FOUND!!!\n\n");
			printf("Seckey : ");
			for(int i=0; i<32; i++) {
				printf("%02X", seckey[i]);
			}
			printf("\n");
			printf("Public Address: 1%s\n", pubaddress);
		}
		else {
			; /*printf("nothing...\n\n");*/
		}
	}
}


