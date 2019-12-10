#include <stdio.h>

/* y^2 = x^3 + 7 */

char unsigned gen_private_key(int byte_count) {
	char unsigned data[byte_count];
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
	char unsigned priv_key[32];
        priv_key[32] = gen_private_key(32);
	
	printf("\n\nPrivate Key : ");
	for(int i=0; i<32; i++) {
                printf("%02X", priv_key[i]);
        }
}
