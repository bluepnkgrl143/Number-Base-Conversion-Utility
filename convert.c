// CS 3503 W04- Comp Org & Arch
// Created by Paola on 9/08/2025.
// Updated by Paola on 09/28/2025
// Assignment: A1 & A2

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
        out[0]= '0';
        out[1]= '\0';
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
            temp[pos++]='A'+(remainder-10);
        }
    //reverse the string
    for (int i=0; i<pos; ++i) {
        out[i]= temp[pos-1 -i];
    }
    out[pos]= '\0';
}

void sub_convert(uint32_t n, int base, char* out) {
    int pos=0;
    if (!out) return;
    if (base<2 || base> 36) {
        out[pos]= '\0';
        return;
    }
    if (n==0) {
        out[0]= '0';
        out[1]= '\0';
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
    out[pos]= '\0';
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
void oct_to_bin(const char *oct, char *out) {
    if (!oct || !out) return;
    // Build binary from octal using direct 3-bit extraction per digit
    int pos=0;
    for (const char *s= oct; *s; ++s) {
        char oct_char = *s;
        if (oct_char<'0' || oct_char >'7') { // invalid octal digit
            out[0]= '\0';
            return;
        }
        int octal_digit= oct_char- '0'; // '7' -> 7
        out[pos++] = ((octal_digit >> 2) & 1) ? '1' : '0';
        out[pos++] = ((octal_digit >> 1) & 1) ? '1' : '0';
        out[pos++] = ((octal_digit >> 0) & 1) ? '1' : '0';
    }
    out[pos] = '\0';
}
void oct_to_hex(const char *oct, char *out){
    if (!oct || !out) return;

    unsigned bitbuf= 0;  // holds pending bits
    int bits= 0;         // # of pending bits in bitbuf
    int started= 0;      // whether output is a non-zero hex digit
    char *p = out;

    // Align total bit length (3*len) to a multiple of 4 by pre-padding with zeros
    size_t len= strlen(oct);
    int pad = (int)((4 -((len *3) & 3)) & 3);
    if (pad > 0) {
        bitbuf <<= pad;
        bits += pad;
    }
    for (const char *s= oct; *s; ++s) {
        char c= *s;
        if (c< '0' || c> '7') { // invalid octal digit
            *p= '\0';
            return;
        }
        unsigned v= (unsigned)(c- '0'); //0 through 7
        bitbuf= (bitbuf << 3) | v;
        bits +=3;

        while (bits >= 4){
            unsigned nibble =(bitbuf >>(bits -4)) & 0xF;
            bits -= 4;
            if (!started){
                if (nibble == 0) continue; // drop leading zeros
                started= 1;
            }
            *p++= (char)(nibble< 10? ('0' +nibble) : ('A' +(nibble -10)));
        }
    }
    // Handle remaining bits (if any),
    if (bits> 0){
        unsigned nibble= (bitbuf & ((1u << bits)- 1u)) << (4-bits);
        if (nibble != 0 || started){
            started= 1;
            *p++= (char)(nibble< 10 ? ('0' +nibble): ('A' + (nibble-10)));
        }
    }
    // If nothing was output, the value is zero
    if (!started){
        *p++= '0';
    }
    *p= '\0';
}
void hex_to_bin(const char *hex, char *out) {
    if (!hex || !out) return;

    char *p = out;
    for (const char *s= hex; *s; ++s) {
        char c= *s;
        unsigned v;
        if (c>= '0' && c<= '9')       v= (unsigned)(c- '0');
        else if (c>= 'A' && c<= 'F')  v= (unsigned)(c- 'A' +10);
        else if (c>= 'a' && c<= 'f')  v= (unsigned)(c- 'a' +10);
        else { *p = '\0'; return; } // invalid hex char

        p[0]= (v & 0x8) ? '1' : '0';
        p[1]= (v & 0x4) ? '1' : '0';
        p[2]= (v & 0x2) ? '1' : '0';
        p[3]= (v & 0x1) ? '1' : '0';
        p += 4;
    }
    *p = '\0';
}

void to_sign_magnitude (int32_t value, char * out) {
    // - For non-negative: just the value (zero-padded to 32 bits)
    // - For negative: set MSB (bit 31) and place magnitude in bits 0..30
    uint32_t bits;
    if (value>= 0) {
        bits= (uint32_t)value; // regular binary
    } else {
        uint32_t mag= (uint32_t)(-(int64_t)value); // safe abs via wider type
        mag &= 0x7FFFFFFFu;   // keep only bits 0..30
        bits = 0x80000000u | mag;   // set sign bit
    }
    for (int i=31; i>=0; --i) {
        out[31-i] = ((bits >> i) & 1u)? '1':'0';
    }
    out[32] = '\0';
}

void to_ones_complement (int32_t value, char * out) {
    // For positive: output as-is with leading zeros
    // For negative: flip ALL bits of the positive magnitude
    uint32_t bits;
    if (value >= 0) {
        bits= (uint32_t)value;
    } else {
        uint32_t mag = (uint32_t)(-(int64_t)value); // safe abs via wider type
        bits= ~mag;    // one's complement: invert every bit
    }
    for (int i=31; i>=0; --i) {
        out[31-i] = ((bits >> i) & 1u)? '1':'0';
    }
    out[32]= '\0';
}
void to_2s_complement (int32_t value, char * out){
    // For positive: output as-is (32 bits, leading zeros)
    // For negative: flip all bits of magnitude and add 1 (two's complement)
    uint32_t bits;
    if (value >= 0){
        bits= (uint32_t)value;
    }
    else {
        uint32_t mag= (uint32_t)(-(int64_t)value); // safe abs via wider type
        bits= (~mag) + 1u;  // two's complement: invert then add 1
    }
    for (int i=31; i>=0; --i) {
        out[31-i] = ((bits >>i) & 1u)? '1':'0';
    }
    out[32]= '\0';
}
void to_32bit_binary ( uint32_t value , char * out) {
    for (int i = 31; i >= 0; i --) {
        out [31 - i] = (( value >> i) & 1) ? '1': '0';
    }
    out [32] = '\0'; // Don ’t forget !
}