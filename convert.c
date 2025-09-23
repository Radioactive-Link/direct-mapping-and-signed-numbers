#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void oct_to_bin(const char *oct, char *out);
void oct_to_hex(const char *oct, char *out);
void hex_to_bin(const char *hex, char *out);
void oct_to_bin_map(char oct, char *out);
char bin_to_hex_map(char *bin);
void hex_to_bin_map(char hex, char *out);
void to_sign_magnitude(int32_t n, char *out);
void to_ones_complement(int32_t n, char *out);
void to_twos_complement(int32_t n, char *out);
void positive_binary(int32_t n, char *out);

void div_convert(uint32_t n, int base, char *out);

void oct_to_bin(const char *oct, char *out) {
    int n = strlen(oct);
    for (int i = 0; i < n; i++) {
        char output[4];
        oct_to_bin_map(oct[i], output);
        // write to output in chunks of 3 (ignoring null terminator)
        for (int j = 0; j < 3; j++) {
            out[(i*3)+j] = output[j];
        }
    }
    out[n*3] = '\0';
}

void oct_to_hex(const char *oct, char *out) {
    // zero case
    if (*oct == '0') {
        out[0] = '0';
        out[1] = '\0';
        return;
    }

    char bin[100];
    oct_to_bin(oct, bin);
    // printf("bin: %s\n", bin);

    char temp[100];
    int pos = 0;

    // --- read from right to left in chunks of 4
    int n = strlen(bin);
    int start = n - 4;
    if (start < 0) start = 0;
    int end = start + 4;
    if (end > n) end = n;

    while (end > 0) {
        char bin_chunk[5];
        int offset = 0;
        // pad out zeros for small inputs
        if (end - start < 4) {
            for (int i = 0; i < 4 - (end - start); i++) {
                bin_chunk[i] = '0';
                offset++;
            }
        }
        // copy rest of digits
        for (int j = 0; j < end - start; j++) {
            // printf("at %c ", bin[start+j]);
            bin_chunk[j+offset] = bin[start+j];
        }
        bin_chunk[4] = '\0';
        // printf("[loop]: %s | start: %d, end: %d\n", bin_chunk, start, end);
        
        temp[pos++] = bin_to_hex_map(bin_chunk);

        end = start;
        start -= 4;
        if (start < 0) {
            start = 0;
        }
    }
    temp[pos] = '\0';

    // ignore (what will be) leading zeros
    while (temp[pos-1] == '0') pos--;
    // reverse to output
    for (int i = pos; i > 0; i--) {
        out[pos-i] = temp[i-1];
    }
    out[pos] = '\0';
}

void hex_to_bin(const char *hex, char *out) {
    int n = strlen(hex);
    for (int i = 0; i < n; i++) {
        char output[5];
        hex_to_bin_map(hex[i], output);
        // write to output in chunks of 4 (ignoring null terminator)
        for (int j = 0; j < 4; j++) {
            out[(i*4)+j] = output[j];
        }
    }
    out[n*4] = '\0';
}

void oct_to_bin_map(char oct, char *out) {
    switch (oct) {
    case '0': strcpy(out, "000"); break;
    case '1': strcpy(out, "001"); break;
    case '2': strcpy(out, "010"); break;
    case '3': strcpy(out, "011"); break;
    case '4': strcpy(out, "100"); break;
    case '5': strcpy(out, "101"); break;
    case '6': strcpy(out, "110"); break;
    case '7': strcpy(out, "111"); break;
    default:
        printf("[ERROR]: oct_to_bin_map: char [%c] is not a valid octal character\n", oct);
        strcpy(out, "???");
    }
}

char bin_to_hex_map(char *bin) {
    if (strcmp(bin, "0000") == 0) return '0';
    if (strcmp(bin, "0001") == 0) return '1';
    if (strcmp(bin, "0010") == 0) return '2';
    if (strcmp(bin, "0011") == 0) return '3';
    if (strcmp(bin, "0100") == 0) return '4';
    if (strcmp(bin, "0101") == 0) return '5';
    if (strcmp(bin, "0110") == 0) return '6';
    if (strcmp(bin, "0111") == 0) return '7';
    if (strcmp(bin, "1000") == 0) return '8';
    if (strcmp(bin, "1001") == 0) return '9';
    if (strcmp(bin, "1010") == 0) return 'A';
    if (strcmp(bin, "1011") == 0) return 'B';
    if (strcmp(bin, "1100") == 0) return 'C';
    if (strcmp(bin, "1101") == 0) return 'D';
    if (strcmp(bin, "1110") == 0) return 'E';
    if (strcmp(bin, "1111") == 0) return 'F';
    
    printf("[ERROR] bin_to_hex_map: string [%s] does not have a hex equivalnet\n", bin);
    return '?';
}

void hex_to_bin_map(char hex, char *out) {
    switch (hex) {
        case '0': strcpy(out, "0000"); break;
        case '1': strcpy(out, "0001"); break;
        case '2': strcpy(out, "0010"); break;
        case '3': strcpy(out, "0011"); break;
        case '4': strcpy(out, "0100"); break;
        case '5': strcpy(out, "0101"); break;
        case '6': strcpy(out, "0110"); break;
        case '7': strcpy(out, "0111"); break;
        case '8': strcpy(out, "1000"); break;
        case '9': strcpy(out, "1001"); break;
        case 'A': case 'a': strcpy(out, "1010"); break;
        case 'B': case 'b': strcpy(out, "1011"); break;
        case 'C': case 'c': strcpy(out, "1100"); break;
        case 'D': case 'd': strcpy(out, "1101"); break;
        case 'E': case 'e': strcpy(out, "1110"); break;
        case 'F': case 'f': strcpy(out, "1111"); break;
        default:
            printf("[ERROR]: hex_to_bin_map: char [%c] is not a valid hex character\n", hex);
            strcpy(out, "????");
    }
}

void to_sign_magnitude(int32_t n, char *out) {
    if (n >= 0) {
        positive_binary(n, out);
    } else {
        // flip it minus one to prepare for div_convert using twos complement
        n = ~(n-1);
        char temp[33];
        div_convert(n, 2, temp);

        int len = strlen(temp);
        // set the sign bit
        out[0] = '1';
        // padding zeros
        for (int i = 1; i < 32 - len; i++) out[i] = '0';
        // put in the actual number
        for (int i = 0; i < len; i++) out[(32-len)+i] = temp[i];
        out[33] = '\0';
    }
}

void to_ones_complement(int32_t n, char *out) {
    if (n >= 0) {
        positive_binary(n, out);
    } else {
        // calling div_convert with n (a signed int) converts it to twos
        // complement, so subtract one
        div_convert(n-1, 2, out);
    }
}

void to_twos_complement(int32_t n, char *out) {
    if (n >= 0) {
        positive_binary(n, out);
    } else {
        div_convert(n, 2, out);
    }
}

/** Returns a positive binary representation of the number `n` into `out`. */
void positive_binary(int32_t n, char *out) {
    char temp[33];
    div_convert(n, 2, temp);
    // printf("temp: %s\n", temp);
    int len = strlen(temp);
    int diff = 32 - len;
    // printf("length: %d\n", len);
    // add leading zeros
    for (int i = 0; i < diff; i++) {
        out[i] = '0';
    }
    // rest
    int pos = 0;
    for (int i = diff; i < 32; i++) {
        out[i] = temp[i-diff];
        pos++;
    }
    out[pos+diff] = '\0';
}

void div_convert(uint32_t n, int base, char *out) {
    char temp[65]; // temporary buffer
    int pos = 0;

    // handle zero case
    if (n == 0) {
        strcpy(out, "0");
        return;
    }

    // extract digits from right to left
    while (n > 0) {
        int remainder = n % base;
        n /= base;

        // convert digit to character
        if (remainder < 10) {
            temp[pos++] = '0' + remainder;
        } else {
            temp[pos++] = 'A' + (remainder - 10);
        }
    }
    temp[pos] = '\0';

    // reverse the result
    size_t l = strlen(temp);
    if (l == 1) {
        out[0] = temp[0];
        out[1] = '\0';
        return;
    }
    int j = 0;
    for (int i=l-1; i >= 0; i--) {
        out[j++] = temp[i];
    }
    out[j] = '\0';
}

void sub_convert(uint32_t n, int base, char *out) {
    // handle zero case
    if (n == 0) {
        strcpy(out, "0");
        return;
    }
    
    uint32_t remainder = n;
    int pos = 0;
    
    // figure out the highest power + 1
    uint64_t exponent = 1; // start at 0th power
    while (exponent <= n) {
        exponent *= base;
    }

    while (remainder > 0) {
        // go to the next lowest power
        exponent /= base;
        // then, find the highest multiple
        int multiple = 0;
        while ((multiple + 1) * exponent <= remainder) {
            multiple++;
        }
        // store the the resulting digit in the output
        if (multiple < 10) {
            out[pos++] = '0' + multiple;
        } else {
            out[pos++] = 'A' + (multiple - 10);
        }
        // finally, perform the subtraction
        remainder -= exponent * multiple;
    }
    // pad out remaining zeros, if needed
    while (exponent != 1) {
        exponent /= base;
        out[pos++] = '0';
    }
    out[pos] = '\0';
}

void print_tables(uint32_t n) {
    // output buffers to use for each base representation
    char binary[100];
    char octal[100];
    char hex[100];
    
    div_convert(n, 2, binary);
    div_convert(n, 8, octal);
    div_convert(n, 16, hex);
    printf(
        "Original: binary=%s Octal=%s Decimal=%u Hex=%s\n",
        binary,
        octal,
        n,
        hex
    );

    uint32_t shifted = n << 3;
    div_convert(shifted, 2, binary);
    div_convert(shifted, 8, octal);
    div_convert(shifted, 16, hex);
    printf(
        "Left Shift by 3: binary=%s Octal=%s Decimal=%u Hex=%s\n",
        binary,
        octal,
        shifted,
        hex
    );

    uint32_t masked = n & 0xFF;
    div_convert(masked, 2, binary);
    div_convert(masked, 8, octal);
    div_convert(masked, 16, hex);
    printf(
        "AND with 0xFF: binary=%s Octal=%s Decimal=%u Hex=%s\n",
        binary,
        octal,
        masked,
        hex
    );
}
