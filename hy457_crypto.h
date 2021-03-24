#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define PLAYFAIR_ROW 5
#define PLAYFAIR_COL 5

//otp
uint8_t *otp_encrypt(uint8_t *plaintext, uint8_t *key);
uint8_t *otp_decrypt(uint8_t *ciphertext, uint8_t *key);

//ceasar
#define OFFSET 4
#define ALPHABET_SIZE 61
uint8_t *caesar_encrypt(uint8_t *plaintext, ushort N);
uint8_t *caesar_decrypt(uint8_t *ciphertext, ushort N);
static int modulo(int left, int right);

//playfair
typedef struct letter_position
{
     int row;
     int col;
} letter_position;
short is_even_v = 0;
unsigned int *replaced_with_X;
short int *replaced_with_I;

unsigned char *playfair_encrypt(unsigned char *plaintext, unsigned char **key);
unsigned char *playfair_decrypt(unsigned char *ciphertext, unsigned char **key);
unsigned char **playfair_keymatrix(unsigned char *key);
static void find_row_col(letter_position *let1_pos, letter_position *let2_pos, char let1, char let2, unsigned char **matrix_key);
static unsigned char *eliminate_duplicates_from_key(unsigned char *key);
static unsigned char *generate_upper_letters_array();
static unsigned char **allocate_matrix(unsigned int row, unsigned int col);
static void print_matrix(unsigned char **matrix);
static short is_even(unsigned char *plaintext);
static void make_plaintext_even(unsigned char *plaintext);
static void add_X_on_duplicates(unsigned char *plaintext);
static short is_letters_on_same_row(char *letter1, char *letter2, unsigned char **matrix_key);
static short is_letters_on_same_col(char *letter1, char *letter2, unsigned char **matrix_key);
static void rectangle_encrypt(char *letter1, char *letter2, unsigned char **matrix_key);

static short is_letters_on_same_row_decrypt(char *letter1, char *letter2, unsigned char **matrix_key);
static short is_letters_on_same_col_decrypt(char *letter1, char *letter2, unsigned char **matrix_key);
static void rectangle_decrypt(char *letter1, char *letter2, unsigned char **matrix_key);
static void sanitize_upper(unsigned char *plaintext);

//Affine
#define a 11
#define b 19
#define m 26
uint8_t *affine_encrypt(uint8_t *plaintext);
uint8_t *affine_decrypt(uint8_t *ciphertext);

//feister cipher
#define n 8
#define S 8 //(64 bits)
uint8_t *feistel_round(uint8_t *block, uint8_t *key);
uint8_t *feistel_encrypt(uint8_t *plaintext, uint8_t keys[][S/2]);
uint8_t *feistel_decrypt(uint8_t *ciphertext, uint8_t keys[][S/2]);

//general

static void print_bytes(FILE *fp, uint8_t *msg);
static int isslash(uint8_t *text, unsigned int index);
static uint8_t *generate__key(int length);

