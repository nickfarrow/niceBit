#include <stdio.h>
#include <time.h>
#include <secp256k1.h>
#include "create_pubkey.h"

static secp256k1_context *ctx = NULL;

int gen_keypair(unsigned char *seckey, unsigned char *pubaddress, secp256k1_context *ctx) {
	secp256k1_pubkey pubkey;
	/*unsigned char seckey[32];*/
	unsigned char public_key64[65];
	
	size_t pk_len = 65;

	int i = 0;

	/* Load private key (seckey) from random bytes */
	FILE *frand = fopen("/dev/urandom", "r");
	fread(seckey, 32, 1, frand);
	fclose(frand);
	if (frand == NULL) {
		printf("Failed to read /dev/urandom\n");
		return 0;
	}

	/* Print private key,
	printf("Seckey : ");
	for(int i=0; i<32; i++) {
		printf("%02X", seckey[i]);
	}
	printf("\n\n");
	*/
	
	/* Apparently there is a 2^-128 chance of
	 * a secret key being invalid.
	 * https://en.bitcoin.it/wiki/Private_key
	 */
	/* Verify secret key is valid */
	if (!secp256k1_ec_seckey_verify(ctx, seckey)) {
		printf("Invalid secret key\n");
	}

	
	/* Create Public Key */
	if (!secp256k1_ec_pubkey_create(ctx, &pubkey, seckey)) {
		printf("Failed to create public key\n");
		return 0;
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

	return 1;
}

int check_vanity(unsigned char *pubaddress) {
	unsigned char compstr[40];
	int j;

	/* For each vanity length
	 * ('len' digits in a row)
	 */
	for(int len=10; len>5; len--) {
		
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
			
			/* Check if string in pubaddress */
			if(strstr(pubaddress, compstr) != NULL) {
				printf("Found : %s\n", compstr);
				return 1;
			}
		}
	}

	return 0;
}

int main() {
	unsigned char seckey[32];
	unsigned char pubaddress[40];
	
	ctx = secp256k1_context_create(
			SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

	int i = 1;
	
	clock_t starttime = clock();
	double timespent;
	double rate;
	while(1) {
		if(!gen_keypair(seckey, pubaddress, ctx)) {
			printf("Failed to create keypair\n");
			return 1;
		}

		if(check_vanity(pubaddress)) {
			printf("Seckey : ");
			for(int i=0; i<32; i++) {
				printf("%02X", seckey[i]);
			}
			printf("\n");
			printf("Public Address: 1%s\n\n", pubaddress);
		}
		else {
			; /*printf("nothing...\n\n");*/
		}

		if(i % 10000 == 0) {
			clock_t currenttime = clock();
			timespent = 
				(double)((currenttime - starttime) 
				/ CLOCKS_PER_SEC);
			rate = (double)(i / timespent);
			printf("Generated %d addresses in %.1fs. Rate:%.1f/s \n", i, timespent, rate);
		}

		i++;
	}

	/* Destroy context to free memory */
	/* TODO make take context out of this function
	 * and use the same context for all generation?
	 */
	secp256k1_context_destroy(ctx);	
}


