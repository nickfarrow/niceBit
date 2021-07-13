#include <stdio.h>

typedef unsigned char byte;

/* See https://en.wikipedia.org/wiki/Positional_notation#Base_conversion */
char *base58(byte *s, int s_size, char *out, int out_size) {
    static const char *base_chars = "123456789"
                                    "ABCDEFGHJKLMNPQRSTUVWXYZ"
                                    "abcdefghijkmnopqrstuvwxyz";

    byte s_cp[s_size];
    memcpy(s_cp, s, s_size);

    int c, i, n;

    out[n = out_size] = 0;
    while (n--) {
        for (c = i = 0; i < s_size; i++) {
            c = c * 256 + s_cp[i];
            s_cp[i] = c / 58;
            c %= 58;
        }
        out[n] = base_chars[c];
    }

    return out;
}
