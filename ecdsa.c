#include <stdio.h>
#include <secp256k1.h>
#include <create_pubkey.h>

/* y^2 = x^3 + 7 */


/* we want to return an array of unsigned bytes */
char unsigned * gen_private_key() {
	int byte_count = 32;
	static char unsigned data[32];
	FILE *random_file;

	random_file = fopen("/dev/urandom", "r");
	fread(&data, 1, byte_count, random_file);
	fclose(random_file);
	
	printf("Private Key : ");
	for(int i=0; i<byte_count; i++) {
		printf("%02X", data[i]);
	}

	return data;
}


int main() {
	/*
	 pointer to a char byte 
	char unsigned *priv_key;

        priv_key = gen_private_key(32);
	
	printf("\n\nPrivate Key : ");
	for(int i=0; i<32; i++) {
                printf("%02X", *(i + priv_key));
        }
	*/

	char unsigned priv_key[32];
        FILE *random_file;

        random_file = fopen("/dev/urandom", "r");
        fread(&priv_key, 1, 32, random_file);
        fclose(random_file);
	
	printf("\n\nPrivate Key : ");
        for(int i=0; i<32; i++) {
                printf("%02X", priv_key[i]);
        }
	int verify_ret;

	secp256k1_context* ctx;
	ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

	verify_ret = secp256k1_ec_seckey_verify(ctx, priv_key);
        printf("\nverify priv_key: %d\n", verify_ret);


	secp256k1_pubkey pubkey;
	int ret;
	ret = secp256k1_ec_pubkey_create(ctx, &pubkey, priv_key); 
	printf("%d", ret);	

	char unsigned public_key64[65];
	size_t pk_len = sizeof(public_key64);

	secp256k1_ec_pubkey_serialize(ctx, public_key64, &pk_len, &pubkey, SECP256K1_EC_UNCOMPRESSED);

	printf("\n\nLong Public Key : ");
        for(int i=0; i<65; i++) {
                printf("%02X", public_key64[i]);
        }
	
	printf("\nPublic Address:1");
	puts(coin_encode(public_key64, 0));

	/*
	&pubkeys[i]
	
	struct secp256k1_context_struct secp256k1_context; 
	
	struct secp256k1_context_struct secp256k1_context;  
	
	int op;
	op = secp256k1_ec_pubkey_create(ctx, priv_key, &pubkey);
	
	printf("%d", op);
	*/
	printf("\n\n\n");
}



