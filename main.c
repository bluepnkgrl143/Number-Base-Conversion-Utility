// CS 3503 W04- Comp Org & Arch
// Created by Paola on 9/08/2025.
// Updated on 9/28/2025
// Assignment: A1 & A2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void div_convert(uint32_t n,int base, char* out);
void sub_convert(uint32_t n, int base,char* out);
void print_tables(uint32_t n);

//Updated A2
void oct_to_bin(const char *oct, char *out);
void oct_to_hex(const char *oct, char *out);
void hex_to_bin(const char *hex, char *out);
void to_sign_magnitude(int32_t value, char *out);
void to_ones_complement(int32_t value, char *out);
void to_2s_complement(int32_t value, char *out);

// Process a single test file
static void process_test_file(const char* filename) {
    FILE *file = fopen(filename, "r"); // open file
    if (file == NULL) {
        printf("Error: Could not open file: %s\n", filename);
        return; // if it can't open it will report error
    }
    printf("=== Running tests from: %s ===\n", filename);

    char line[256];
    unsigned test_num= 0;
    unsigned passed= 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        //remove new line:
        line[strcspn(line, "\n")]= '\0';
        // skip comments and empty lines
        char *p= line; //assigns lines and reads it as tokens
        while (*p== ' ' || *p== '\t')++p;
        if (*p== '\0' || *p== '#') continue;

        char cmd[32];
        if (sscanf(p, "%31s",cmd)!= 1) continue;

        if (strcmp(cmd, "div_convert")==0 || strcmp(cmd, "sub_convert")==0) {
            unsigned long long ull=0;
            int base=0;
            char expected[65]= {0}; //if command is division or subtraction it parses

            // Expect: <cmd> <n> <base> <expected>
            if (sscanf(p, "%31s %llu %d %64s",cmd, &ull, &base, expected)!= 4) {
                continue;
            }

            uint32_t n = (uint32_t)ull;
            char got[65];

            if (strcmp(cmd,"div_convert")== 0) {
                div_convert(n,base,got);
            } else {
                sub_convert(n,base,got);
            }

            ++test_num;
            int ok= (strcmp(got,expected)== 0);
            if (ok) ++passed;

            printf("Test %u: %s(%u, %d) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                   test_num, cmd, n, base, expected, got, ok? "PASS":"FAIL");
        }
        else if (strcmp(cmd, "oct_to_bin")==0 || strcmp(cmd, "oct_to_hex")==0) {
            char input[128]={0}, expected[128]={0};
            // Expect: <cmd> <input_string> <expected_string>
            if (sscanf(p, "%31s %127s %127s", cmd, input, expected) != 3) {
                continue;
            }
            char got[256];
            if (strcmp(cmd, "oct_to_bin")==0) {
                oct_to_bin(input, got);
            } else {
                oct_to_hex(input, got);
            }
            ++test_num;
            int ok= (strcmp(got, expected) == 0);
            if (ok) ++passed;
            printf("Test %u: %s(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                   test_num, cmd, input, expected, got, ok ? "PASS" : "FAIL");
        }
        else if (strcmp(cmd, "hex_to_bin")==0) {
            char input[128]={0}, expected[256]={0};
            // Expect: <cmd> <hex_string> <expected_binary>
            if (sscanf(p, "%31s %127s %255s", cmd, input, expected) != 3) {
                continue;
            }
            char got[256];
            hex_to_bin(input, got);
            ++test_num;
            int ok= (strcmp(got, expected) == 0);
            if (ok) ++passed;
            printf("Test %u: %s(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                   test_num, cmd, input, expected, got, ok ? "PASS" : "FAIL");
        }
        else if (strcmp(cmd, "to_sign_magnitude")==0 || strcmp(cmd, "to_ones_complement")==0 || strcmp(cmd, "to_2s_complement")==0) {
            long long ll=0;
            char expected[128]={0};
            // Expect: <cmd> <n> <expected>
            if (sscanf(p, "%31s %lld %127s", cmd, &ll, expected) != 3) {
                continue;
            }
            int32_t n = (int32_t)ll;
            char got[128];
            if (strcmp(cmd, "to_sign_magnitude")==0) {
                to_sign_magnitude(n, got);
            } else if (strcmp(cmd, "to_ones_complement")==0) {
                to_ones_complement(n, got);
            } else { // Supports both names for two's complement
                to_2s_complement(n, got);
            }
            ++test_num;
            int ok = (strcmp(got, expected) == 0);
            if (ok) ++passed;
            printf("Test %u: %s(%d) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                   test_num, cmd, n, expected, got, ok ? "PASS": "FAIL");
        }
        else if (strcmp(cmd, "print_tables")==0) {
            unsigned long long ull = 0;
            // Accept: print_tables <n> [anything...]
            if (sscanf(p, "%*s %llu", &ull)==1) {
                print_tables((uint32_t)ull);
            }
        }
    }

    printf("Summary: %u/%u tests passed\n", passed, test_num);
    fclose(file);
}

int main(void) {
    // runs both test files now.
    const char* files[]= {"a1_test_file.txt", "a2_test.txt" };
    const size_t count= sizeof(files)/ sizeof(files[0]);
    for (size_t i=0; i<count; ++i) {
        process_test_file(files[i]);
    }
    return 0;
}

