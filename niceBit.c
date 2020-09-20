#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <secp256k1.h>
#include "createPubKey.h"
#include "walletImportFormat.h"

static secp256k1_context *ctx = NULL;

/* Create private & public address pair */
int gen_keypair(unsigned char *seckey, char *pubaddress, secp256k1_context *ctx) {
    secp256k1_pubkey pubkey;
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
    pubkey_to_P2PKH(public_key64, pubaddress);

    return 1;
}

int check_nums(char *pubaddress, int searchlen) {
    unsigned char compstr[40];
    char possibleChars[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    int j;

    /* For each vanity length
    * ('len' digits in a row)
    */
    for (int len=10; len>=searchlen; len--) {

        /* For each digit 1-9 */
        for (int i=0; i<58; i++) {

            /* Comprise compstr of 'len' repeats
            * of digit 'i'
            */
            j = 0;
            while (j<len) {
                compstr[j] = possibleChars[i];
                j++;
            }

            /* End string with null char*/
            compstr[j] = '\0';

            /* Check if string in pubaddress */
            if (strstr(pubaddress, compstr) != NULL) {
                printf("Found : %s\n", compstr);
                return 1;
            }
        }
    }
    return 0;
}

int check_words(char *pubaddress, char (*words)[34], int nwords) {
    for (int i=0; i<nwords; i++) {
        /* printf("%s\n", words[i]);
        puts(pubaddress); */
        if (strstr(pubaddress, words[i]) != NULL) {
            printf("Found : %s\n", words[i]);
            return 1;
        }
    }
    return 0;
}

int check_vanity(char *pubaddress, int searchlen, char (*words)[34], int nwords) {
    if ((nwords > 0) && check_words(pubaddress, words, nwords)) {
        return 1;
    }
    if ((searchlen >0) && check_nums(pubaddress, searchlen)) {
        return 1;
    }
    return 0;
}

void all_substitutes(char (*words)[34], char word[34], int *n_words, int len, int start, int a){
    if (start == len){
        strcpy(words[*n_words], word);
        words[*n_words-1][len] = '\0';
        (*n_words)++;
        return;
    }
    char ch = word[start];
    char sub = '\0';
    switch (ch){
        case 'e':
            if (a == 1){
                sub = '3';
            }
            break;
        case '1':
            if (a == 1){
                sub = 'i';
            }
            break;
        case 'O':
            // No base58 sub
            break;
        case 'L':
            // No base58 sub
            break;
        case 'i':
            // No base58 sub
            break;
        default:
            if(ch >= 'a' && ch <= 'z'){
                sub = toupper(ch);
            }
            else if(ch >= 'A' && ch <= 'Z'){
                sub = tolower(ch);
            }
            break;
    }
    //Skip on special chars
    if (sub == '\0') {
        all_substitutes(words, word, n_words, len, start+1, a);
    }
    else {
        char word2[34];
        strcpy(word2, word);
        word2[start]=sub;
        all_substitutes(words, word, n_words, len, start+1, a);
        all_substitutes(words, word2, n_words, len, start+1, a);
    }
}

void alphanum_combinations(char (*word)[34], char search_list[1000][34], int *n_words, int a) {
    int initial_n_words = *n_words;
    *n_words = 0;
    //Generated alphanumeric substitutions
    for (int i=0; i<initial_n_words; i++) {
        all_substitutes(search_list, word[i], n_words, strlen(word[i]), 0, a);
    }
}

int main(int argc, char **argv) {
    unsigned char seckey[32];
    char pubaddress[34];

    char *p = pubaddress;
    char *n = "0";
    char *filename = "";
    int C = 0;
    int a = 0;

    int searchlen;
    int c;

    /* Get input arguments (length) */
    while ((c = getopt(argc, argv, "aCn:f:")) != -1) {
        switch (c) {
            case 'n':
            n = optarg;
            break;
            case 'f':
            filename = optarg;
            break;
            case 'a':
            a = 1;
            break;
            case 'C':
            C = 1;
            break;
            case '?':
            printf("Invalid argument: %c\n", optopt);
            return 1;
        }
    }

    if ((n == "0") && (filename == "")) {
        puts("Resorting to default search length of 6.");
        n = "6";
    }
    searchlen = atoi(n);

    ctx = secp256k1_context_create(
        SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

        int i = 1;

        time_t start_time, current_time;
        double time_spent;
        double rate;

        char words[100][34];

        //Generated alphanumeric substitutions
        char search_list[1000][34];
        int n_words = 0;
        /* Load Dictionary File */
        if (filename != "") {
            puts("Loading words...");
            FILE *fptr = fopen(filename, "r");

            while (fgets(words[n_words], 34, fptr)) {
                words[n_words][strlen(words[n_words]) - 1] = '\0';
                puts(words[n_words]);
                n_words++;
            }
            if (C){
                alphanum_combinations(words, search_list, &n_words, a);

                for (i=0; i<n_words; i++) {
                    printf("word %d is %s \n", i, search_list[i]);
                }

                //search_list[n_words][0] = '\0';
            }
            else {
                for (i=0; i<n_words; i++) {
                    strcpy(search_list[i], words[i]);
                }

            }
            printf("%d words have been loaded.\n", n_words);
        }

        puts("Beginning search...\n");
        time(&start_time);
        while (1) {
            if (!gen_keypair(seckey, pubaddress, ctx)) {
                printf("Failed to create keypair\n");
                return 1;
            }

            if (check_vanity(pubaddress, searchlen, search_list, n_words)) {
                printf("Seckey : ");
                for (int j=0; j<32; j++) {
                    printf("%02X", seckey[j]);
                }
                printf("\nWIF: ");
                create_wif(seckey);

                printf("Public Address: %s\n\n", pubaddress);
            }

            if (i % 100000 == 0) {
                time(&current_time);
                time_spent = difftime(current_time, start_time);

                rate = (double)(i / time_spent);
                printf("Generated %d addresses in %.1fs. Rate:%.1f/s \n", i, time_spent, rate);
            }

            i++;
        }

        /* Destroy context to free memory */
        secp256k1_context_destroy(ctx);
    }
