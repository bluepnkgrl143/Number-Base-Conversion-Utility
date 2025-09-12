// CS 3503 W04- Comp Org & Arch
// Created by paola on 9/08/2025.
// Assignment: A1

#include <stdint.h> // For uint32_t
#include <stdio.h>
#include <string.h>

void div_convert(uint32_t n, int base, char* out) {
    if (!out) return;

    char temp[65];
    int pos=0;

    if (base<2 || base> 36) {
        out[pos]= '\0';
        return;
    }
    //Handle zero cases
    if (n==0) {
        out[pos]= '\0';
        return;
        }
    //extracts digits from right to left
    while (n>0) {
        int remainder= n% base;
        n =n/ base;
        //convert digit to character
        if (remainder <10)
            temp[pos++]= '0'+remainder;
        else
            temp[pos++]='A'+(remainder - 10);
        }
    //reverse the string
    for (int i = 0; i<pos; ++i) {
        out[i] = temp[pos-1 -i];
    }
    out[pos] = '\0';
}

void sub_convert(uint32_t n, int base, char* out) {
    int pos=0;
    if (!out) return;
    if (base<2 || base> 36) {
        out[pos]= '\0';
        return;
    }
    if (n==0) {
        out[pos]= '\0';
        return;
    }
    // finding largest power of base that fits in n
    uint64_t power=1;
    while (power<=(uint64_t)n /(uint64_t)base) {
        power*= (uint64_t)base;
    }

    while (power>0) {
        unsigned digit =(unsigned)((uint64_t)n /power);
        if (digit<10) out[pos++]= (char)('0'+digit);
        else out[pos++] =(char)('A'+(digit -10));
        n-= (uint32_t)(digit* power);
        power/= (uint64_t)base;
    }
    out[pos] = '\0';
}

void print_tables(uint32_t n) {
    char bin[33], oct[12], hex[9];
    //original number
    div_convert(n, 2, bin);
    div_convert(n, 8, oct);
    div_convert(n, 16, hex);
    printf("Original: Binary=%s Octal=%s Decimal=%u Hex=%s\n", bin,oct,n,hex);
    //left shift by 3
    uint32_t shifted=n<<3;
    div_convert(shifted, 2, bin);
    div_convert(shifted, 8, oct);
    div_convert(shifted, 16, hex);
    printf("Left Shift by 3: Binary=%s Octal=%s Decimal=%u Hex=%s\n",bin,oct,shifted,hex);
    // AND with 0xFF
    uint32_t masked=n&0xFF;
    div_convert(masked, 2, bin);
    div_convert(masked, 8, oct);
    div_convert(masked, 16, hex);
    printf("AND with 0xFF: Binary=%s Octal=%s Decimal=%u Hex=%s\n",bin,oct,masked,hex);
}