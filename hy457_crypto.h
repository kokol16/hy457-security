#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include<ctype.h>
#include<string.h>
uint8_t * otp_encrypt(uint8_t *plaintext, uint8_t *key);
uint8_t * otp_decrypt(uint8_t *ciphertext, uint8_t *key); 
extern void print_bytes(FILE *fp, uint8_t *msg);
extern int isslash(uint8_t *text, unsigned int index);
