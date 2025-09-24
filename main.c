#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void oct_to_bin(const char *oct, char *out);
void hex_to_bin(const char *hex, char *out);
void oct_to_hex(const char *oct, char *out);
void to_sign_magnitude(int32_t n, char *out);
void to_ones_complement(int32_t n, char *out);
void to_twos_complement(int32_t n, char *out);

int main() {
    FILE *file = fopen("A2_tests.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file\n");
        return 1;
    }

    // what test is being ran
    int test = 0;
    // # of tests passed
    int passed = 0;

    char line[100];
    while (fgets(line, 100, file)) {
        // skip empty lines
        if (strlen(line) <= 1) continue;
        // printf("line: %s\n", line);

        // remove newline character
        line[strcspn(line, "\n")] = '\0';

        // --- begin parsing for all functions
        // expected format: char*, uint32_t, ...
        // keeps track of the current token in the line
        char* token = strtok(line, " "); // space as delimiter
        char func_name[256];
        int32_t n;
        char number_string[33];
        char output_arg[33];

        strcpy(func_name, token);

        // ignore lines that do not start with one of the following function names
        if (strcmp(func_name, "oct_to_bin") != 0 &&
            strcmp(func_name, "oct_to_hex") != 0 &&
            strcmp(func_name, "hex_to_bin") != 0 &&
            strcmp(func_name, "to_sign_magnitude") != 0 &&
            strcmp(func_name, "to_ones_complement") != 0 &&
            strcmp(func_name, "to_twos_complement") != 0) continue;

        token = strtok(NULL, " "); // go to next token

        if (strcmp(func_name, "to_sign_magnitude") == 0 ||
            strcmp(func_name, "to_ones_complement") == 0 ||
            strcmp(func_name, "to_twos_complement") == 0)
        {
            // parse as base 10
            n = strtoul(token, NULL, 10);
            sprintf(output_arg, "%d", n);
        } else {
            // as string
            strcpy(number_string, token);
            sprintf(output_arg, "\"%s\"", number_string);
        }

        char expected[256];
        token = strtok(NULL, " ");
        strcpy(expected, token);

        char result[100];
        char* result_category = "FAIL";
        if (strcmp(func_name, "oct_to_bin") == 0) oct_to_bin(number_string, result);
        else if (strcmp(func_name, "oct_to_hex") == 0) oct_to_hex(number_string, result);
        else if (strcmp(func_name, "hex_to_bin") == 0) hex_to_bin(number_string, result);
        else if (strcmp(func_name, "to_sign_magnitude") == 0) to_sign_magnitude(n, result);
        else if (strcmp(func_name, "to_ones_complement") == 0) to_ones_complement(n, result);
        else if (strcmp(func_name, "to_twos_complement") == 0) to_twos_complement(n, result);

        if (strcmp(result, expected) == 0) {
            result_category = "PASS";
            passed++;
        }
        printf("Test %i: %s(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
            ++test, func_name, output_arg, expected, result, result_category);
    }
    printf("Summary: %i/%i tests passed", passed, test);
    fclose(file);
}