// CS 3503 W04- Comp Org & Arch
// Created by paola on 9/08/2025.
// Assignment: A1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void div_convert(uint32_t n,int base, char* out);
void sub_convert(uint32_t n, int base,char* out);
void print_tables(uint32_t n);

int main(void) {
    FILE *file= fopen("a1_test_file.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file\n");
        return 1;
    }
    char line[256];
    unsigned test_num = 0;
    unsigned passed = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        //remove new line:
        line[strcspn(line, "\n")]= '\0';
        // skip comments and empty lines
        char *p= line;
        while (*p== ' ' || *p== '\t')++p;
        if (*p== '\0' || *p== '#') continue;

        char cmd[32];
        if (sscanf(p, "%31s",cmd)!= 1) continue;

        if (strcmp(cmd, "div_convert")==0 || strcmp(cmd, "sub_convert")==0) {
            unsigned long long ull=0;
            int base=0;
            char expected[65]= {0};

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
                   test_num, cmd, n, base, expected, got, ok ? "PASS" : "FAIL");
        } else if (strcmp(cmd, "print_tables")==0) {
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
