#include "hy457_crypto.h"
uint8_t *otp_decrypt(uint8_t *ciphertext, uint8_t *key)
{
    unsigned int index = 0;
    int length = strlen(ciphertext) + 1;
    uint8_t *plaintext = malloc(length * sizeof(uint8_t));
    while (length >= 0)
    {
        //if (isalpha(plaintext[index]) || isdigit(plaintext[index]) || !isslash(plaintext, index))
        plaintext[index] = ciphertext[index] ^ key[index];
        //else
        //plaintext[index] = ciphertext[index];

        index++;
        length--;
    }
    return plaintext;
}
int isslash(uint8_t *text, unsigned int index)
{
    unsigned int tmp_index = index;
    if (text[tmp_index] == '\\' || ((tmp_index - 1 > 0) && text[tmp_index - 1] == '\\'))
    {
        return 1;
    }
    return 0;
}
uint8_t *otp_encrypt(uint8_t *plaintext, uint8_t *key)
{
    unsigned int index = 0;
    int length = strlen(plaintext) + 1;
    uint8_t *cipher_text = malloc(length * sizeof(uint8_t));
    while (length >= 0)
    {

        //  if (isalpha(plaintext[index]) || isdigit(plaintext[index]) || !isslash(plaintext, index))
        cipher_text[index] = plaintext[index] ^ key[index];
        //else
        //  cipher_text[index] = plaintext[index];

        index++;
        length--;
    }
    //print_bytes(stdout, cipher_text);

    return cipher_text;
}
uint8_t *generate_otp_key(int length)
{
    FILE *fp;
    int ch;
    uint8_t *key = (uint8_t *)malloc(length * sizeof(uint8_t));
    fp = fopen("/dev/urandom", "r");
    unsigned int index = 0;
    while ((ch = fgetc(fp)) != EOF && index < length)
    {
        key[index++] = ch;
    }
    key[index] = '\0';
    return key;
}
void print_bytes(FILE *fp, uint8_t *msg)
{
    if (msg == NULL)
    {
        return;
    }
    uint8_t *tmp_msg = msg;
    while (*tmp_msg != '\0')
    {
        if (isprint(*tmp_msg))
        {
            fprintf(fp, "%c ", *tmp_msg);
        }
        else
        {
            fprintf(fp, "%c ", *tmp_msg);

            //fprintf(fp, "%s ", "non_printable ");
        }
        tmp_msg++;
    }
    fprintf(fp, "%c", '\n');
}
void main_otp(int argc, char **argv)
{
    FILE *fp;
    uint8_t *plaintext, *key;
    int ch;
    unsigned int count_bytes = 0, index = 0;
    if (argc > 1)
    {
        fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            fprintf(stderr, "error can't open file\n");
            return ;
        }
        while ((ch = fgetc(fp)) != EOF)
        {
            count_bytes++;
        }
        fseek(fp, 0, SEEK_SET);
        plaintext = (uint8_t *)malloc(count_bytes * sizeof(uint8_t));

        while ((ch = fgetc(fp)) != EOF)
        {
            plaintext[index++] = ch;
            fprintf(stdout, "%c", plaintext[index - 1]);
        }
        plaintext[index] = '\0';
        fprintf(stdout, "%c", '\n');
    }
    else
    {
        plaintext = malloc(sizeof(uint8_t) * 1024);
        scanf("%s", plaintext);
        printf("%s\n", plaintext);
        count_bytes = strlen(plaintext);
    }

    key = generate_otp_key(count_bytes);
    fprintf(stdout, "%c", '\n');

    uint8_t *ciphertext = otp_encrypt(plaintext, key);
    fprintf(stdout, "ciphertext: ");

    print_bytes(stdout, ciphertext);

    plaintext = otp_decrypt(ciphertext, key);
    fprintf(stdout, "plaintext: ");
    print_bytes(stdout, plaintext);

    //close files
    //free memory
    free(key);
    free(plaintext);
}





int main(int argc, char **argv)
{
    main_otp(argc,argv);
    return 0;
}