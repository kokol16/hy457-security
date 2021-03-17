#include "hy457_crypto.h"
#define OFFSET 34
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
    if (fp == NULL)
    {
        fprintf(stderr, "cannot open file \n");
        return NULL;
    }
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
void main_otp(unsigned int count_bytes, uint8_t *plaintext)
{
    uint8_t *key;
    key = generate_otp_key(count_bytes);
    if (key == NULL)
    {
        return;
    }
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

int modulo(int left, int right)
{
    return ((left % right + right) % right);
}

uint8_t *caesar_encrypt(uint8_t *plaintext, ushort N)
{
    unsigned int index = 0;
    int length = strlen(plaintext) + 1;
    uint8_t *cipher_text = malloc(length * sizeof(uint8_t));
    while (length >= 0)
    {
        if (isupper(plaintext[index]))
        {
            cipher_text[index] = ((plaintext[index] + N - 65) % 26) + 65;
        }
        else if (islower(plaintext[index]))
        {

            cipher_text[index] = ((plaintext[index] + N - 97) % 26) + 97;
        }
        else if (isdigit(plaintext[index]))
        {
            cipher_text[index] = ((plaintext[index] + N - 48) % 10) + 48;
        }
        else
        {
            cipher_text[index] = plaintext[index];
        }
        index++;
        length--;
    }
    //print_bytes(stdout, cipher_text);

    return cipher_text;
}
uint8_t *caesar_decrypt(uint8_t *ciphertext, ushort N)
{
    unsigned int index = 0;
    int length = strlen(ciphertext) + 1;
    uint8_t *plaintext = malloc(length * sizeof(uint8_t));
    while (length >= 0)
    {
        if (isupper(ciphertext[index]))
        {
            plaintext[index] = (modulo((ciphertext[index] - N - 65), 26)) + 65;
        }
        else if (islower(ciphertext[index]))
        {
            plaintext[index] = (modulo((ciphertext[index] - N - 97), 26)) + 97;
        }
        else if (isdigit(ciphertext[index]))
        {
            plaintext[index] = (modulo((ciphertext[index] - N - 48), 10)) + 48;
        }
        else
        {
            plaintext[index] = ciphertext[index];
        }
        index++;
        length--;
    }
    return plaintext;
}

void main_ceasar(unsigned int count_bytes, uint8_t *plaintext)
{
    uint8_t *ciphertext;
    ciphertext = caesar_encrypt(plaintext, OFFSET);
    plaintext = caesar_decrypt(ciphertext, OFFSET);

    print_bytes(stdout, ciphertext);
    print_bytes(stdout, plaintext);
}

unsigned char *eliminate_duplicates_from_key(unsigned char *key)
{
    short ascii_upper_let[91] = {0};
    unsigned int index = 0, key_index = 0, pos = 0;
    int key_length = strlen(key);
    unsigned char *clean_key = malloc(sizeof(char) * key_length + 1);
    memcpy(clean_key, key, key_length + 1);
    while (clean_key[index] != '\0')
    {
        if (ascii_upper_let[clean_key[index]] != 1)
        {
            if (clean_key[index] == 'J' || clean_key[index] == 'I')
            {
                ascii_upper_let['J'] = 1;
                ascii_upper_let['I'] = 1;
                clean_key[pos++] = 'I';
            }
            else
            {
                ascii_upper_let[clean_key[index]] = 1;
                clean_key[pos++] = clean_key[index];
            }
        }
        index++;
    }

    clean_key[pos] = '\0';
    return clean_key;
}
unsigned char *generate_upper_letters_array()
{
    int i = 0;
    unsigned char *upper_letters = malloc(sizeof(unsigned char) * 26);
    for (i = 0; i < 26; i++)
    {
        upper_letters[i] = 'A' + i;
    }
    return upper_letters;
}

unsigned char **allocate_matrix(unsigned int row, unsigned int col)
{
    unsigned char **matrix;
    unsigned int r = 0;
    matrix = (unsigned char **)malloc(row * sizeof(unsigned char *));
    for (r = 0; r < row; r++)
    {
        matrix[r] = (unsigned char *)malloc(col * sizeof(unsigned char));
    }
    return matrix;
}
void print_matrix(unsigned char **matrix)
{
    unsigned int row, col;
    for (row = 0; row < PLAYFAIR_ROW; row++)
    {
        for (col = 0; col < PLAYFAIR_COL; col++)
        {
            fprintf(stdout, "%c", matrix[row][col]);
        }
        fprintf(stdout, "%s", "\n-----\n");
    }
}
unsigned char **playfair_keymatrix(unsigned char *key)
{
    unsigned char **key_matrix = allocate_matrix(PLAYFAIR_ROW, PLAYFAIR_COL);
    unsigned short row = 0, col = 0, index = 0;
    int ascii_upper_let[91] = {0};
    unsigned int pos = 0;
    sanitize_upper(key);

    unsigned char *clean_key = eliminate_duplicates_from_key(key);
    
    int key_length = strlen(clean_key);
    unsigned char *upper_letters = generate_upper_letters_array();
    //memset(ascii_upper_let, 0, 90);
    for (row = 0; row < PLAYFAIR_ROW; row++)
    {
        for (col = 0; col < PLAYFAIR_COL; col++)
        {
            if (index < key_length)
            {
                if (clean_key[index] == 'I')
                {
                    key_matrix[row][col] = 'I';
                    ascii_upper_let['I'] = 1;
                    ascii_upper_let['J'] = 1;
                }
                else
                {
                    key_matrix[row][col] = clean_key[index];
                    ascii_upper_let[clean_key[index]] = 1;
                }

                index++;
            }
            else
            {
                //dietrexe upper letters kai bale mesa opoio boreis
                while (1)
                {
                    if (pos > 25)
                    {
                        break;
                    }
                    if (ascii_upper_let[upper_letters[pos]] == 0)
                    {
                        if (upper_letters[pos] == 'J' || upper_letters[pos] == 'I')
                        {
                            key_matrix[row][col] = 'I';
                            ascii_upper_let['J'] = 1;
                            ascii_upper_let['I'] = 1;
                        }
                        else if (ascii_upper_let[upper_letters[pos]] == 0)
                        {
                            key_matrix[row][col] = upper_letters[pos];
                            ascii_upper_let[upper_letters[pos]] = 1;
                        }

                        pos++;

                        break;
                    }
                    pos++;
                }
            }
        }
    }
    return key_matrix;
}

short is_even(unsigned char *plaintext)
{
    return strlen(plaintext) % 2 == 0;
}
void make_plaintext_even(unsigned char *plaintext)
{
    int plaintext_length = 0;
    plaintext_length = strlen(plaintext);
    if (!is_even(plaintext))
    {

        plaintext = (char *)realloc(plaintext, plaintext_length + 2);
        memcpy(plaintext + plaintext_length, "X", 1);
        memcpy(plaintext + plaintext_length + 1, "\0", 1);
        plaintext_length += 1;
    }
    else
    {
        is_even_v = 1;
    }
}
void make_J_to_I(unsigned char *plaintext)
{
    unsigned int index = 0;
    replaced_with_I = malloc(sizeof(short) * strlen(plaintext));
    while (plaintext[index] != 0)
    {
        if (plaintext[index] == 'J')
        {
            replaced_with_I[index] = 1;
            plaintext[index] = 'I';
        }
        index++;
    }
}
void add_X_on_duplicates(unsigned char *plaintext)
{
    unsigned int index = 0, x_index = 0;
    replaced_with_X = malloc(sizeof(unsigned int) * (strlen(plaintext) + 1));
    while (plaintext[index] != '\0')
    {
        if (plaintext[index] == plaintext[index + 1])
        {
            replaced_with_X[index + 1] = 1;
            plaintext[index + 1] = 'X';
        }
        index += 2;
    }
}

short is_letters_on_same_row(char *letter1, char *letter2, unsigned char **matrix_key)
{
    letter_position let1_pos, let2_pos;
    char let1 = *letter1;
    char let2 = *letter2;
    find_row_col(&let1_pos, &let2_pos, let1, let2, matrix_key);

    if (let1_pos.row == let2_pos.row && let1_pos.row != -1 && let2_pos.row != -1)
    {
        short wrapped_1 = 0, wrapped_2 = 0;
        if (let1_pos.col + 1 >= PLAYFAIR_COL) //wrap around
        {
            wrapped_1 = 1;
            let1 = matrix_key[let1_pos.row][0];
        }
        if (let2_pos.col + 1 >= PLAYFAIR_COL)
        {
            wrapped_2 = 1;
            let2 = matrix_key[let2_pos.row][0];
        }
        if (wrapped_2 == 0)
        {
            let2 = matrix_key[let2_pos.row][let2_pos.col + 1];
        }
        if (wrapped_1 == 0)
        {
            let1 = matrix_key[let1_pos.row][let1_pos.col + 1];
        }

        *letter1 = let1;
        *letter2 = let2;
        return 1;
    }
    else
    {
        return 0;
    }
}
void find_row_col(letter_position *let1_pos, letter_position *let2_pos, char let1, char let2, unsigned char **matrix_key)
{
    unsigned int row, col;
    let1_pos->col = -1;
    let2_pos->col = -1;
    for (row = 0; row < PLAYFAIR_ROW; row++)
    {
        for (col = 0; col < PLAYFAIR_COL; col++)
        {
            if (let1_pos->col != -1 && let2_pos->col != -1)
            {
                break;
            }
            if (matrix_key[row][col] == let1)
            {
                let1_pos->row = row;
                let1_pos->col = col;
            }
            if (matrix_key[row][col] == let2)
            {
                let2_pos->row = row;
                let2_pos->col = col;
            }
        }
    }
}

short is_letters_on_same_col(char *letter1, char *letter2, unsigned char **matrix_key)
{
    letter_position let1_pos, let2_pos;
    char let1 = *letter1;
    char let2 = *letter2;

    find_row_col(&let1_pos, &let2_pos, let1, let2, matrix_key);
    if (let1_pos.col == let2_pos.col && let1_pos.col != -1 && let2_pos.col != -1)
    {
        short wrapped_1 = 0, wrapped_2 = 0;
        if (let1_pos.row + 1 >= PLAYFAIR_ROW) //wrap around
        {
            wrapped_1 = 1;
            let1 = matrix_key[0][let1_pos.col];
        }
        if (let2_pos.row + 1 >= PLAYFAIR_ROW)
        {
            wrapped_2 = 1;
            let2 = matrix_key[0][let2_pos.col];
        }
        if (wrapped_2 == 0)
        {
            let2 = matrix_key[let2_pos.row + 1][let2_pos.col];
        }
        if (wrapped_1 == 0)
        {
            let1 = matrix_key[let1_pos.row + 1][let1_pos.col];
        }

        *letter1 = let1;
        *letter2 = let2;
        return 1;
    }
    else
    {
        return 0;
    }
}

void rectangle_encrypt(char *letter1, char *letter2, unsigned char **matrix_key)
{
    letter_position let1_pos, let2_pos;
    char let1 = *letter1;
    char let2 = *letter2;
    unsigned int from_row, to_row, from_col, to_col;
    find_row_col(&let1_pos, &let2_pos, let1, let2, matrix_key);

    *letter1 = matrix_key[let1_pos.row][let2_pos.col];
    *letter2 = matrix_key[let2_pos.row][let1_pos.col];
}

short is_letters_on_same_row_decrypt(char *letter1, char *letter2, unsigned char **matrix_key)
{
    letter_position let1_pos, let2_pos;
    char let1 = *letter1;
    char let2 = *letter2;
    find_row_col(&let1_pos, &let2_pos, let1, let2, matrix_key);

    if (let1_pos.row == let2_pos.row && let1_pos.row != -1 && let2_pos.row != -1)
    {
        short wrapped_1 = 0, wrapped_2 = 0;
        if ((let1_pos.col - 1) < 0) //wrap around
        {
            wrapped_1 = 1;
            let1 = matrix_key[let1_pos.row][4];
        }
        if ((let2_pos.col - 1) < 0)
        {
            wrapped_2 = 1;
            let2 = matrix_key[let2_pos.row][4];
        }
        if (wrapped_2 == 0)
        {

            let2 = matrix_key[let2_pos.row][(let2_pos.col - 1)];
        }
        if (wrapped_1 == 0)
        {

            let1 = matrix_key[let1_pos.row][(let1_pos.col - 1)];
        }

        *letter1 = let1;
        *letter2 = let2;

        return 1;
    }
    else
    {
        return 0;
    }
}
short is_letters_on_same_col_decrypt(char *letter1, char *letter2, unsigned char **matrix_key)
{

    letter_position let1_pos, let2_pos;
    char let1 = *letter1;
    char let2 = *letter2;

    find_row_col(&let1_pos, &let2_pos, let1, let2, matrix_key);
    if (let1_pos.col == let2_pos.col && let1_pos.col != -1 && let2_pos.col != -1)
    {
        short wrapped_1 = 0, wrapped_2 = 0;
        if (let1_pos.row - 1 < 0) //wrap around
        {
            wrapped_1 = 1;
            let1 = matrix_key[4][let1_pos.col];
        }
        if (let2_pos.row - 1 < 0)
        {
            wrapped_2 = 1;
            let2 = matrix_key[4][let2_pos.col];
        }
        if (wrapped_2 == 0)
        {
            let2 = matrix_key[let2_pos.row - 1][let2_pos.col];
        }
        if (wrapped_1 == 0)
        {
            let1 = matrix_key[let1_pos.row - 1][let1_pos.col];
        }

        *letter1 = let1;
        *letter2 = let2;
        return 1;
    }
    else
    {
        return 0;
    }
}
void rectangle_decrypt(char *letter1, char *letter2, unsigned char **matrix_key)
{
    letter_position let1_pos, let2_pos;
    char let1 = *letter1;
    char let2 = *letter2;
    unsigned int from_row, to_row, from_col, to_col;
    find_row_col(&let1_pos, &let2_pos, let1, let2, matrix_key);

    *letter1 = matrix_key[let1_pos.row][let2_pos.col];
    *letter2 = matrix_key[let2_pos.row][let1_pos.col];
}

void sanitize_upper(unsigned char *plaintext)
{
    unsigned int valid_input_index=0 , index=0;
    while (plaintext[index] != '\0')
    {
        if (!isupper(plaintext[index]))
        {
        }
        else
        {
            plaintext[valid_input_index++] = plaintext[index];
        }
        index++;
    }
    plaintext[valid_input_index] = '\0';
}
unsigned char *playfair_encrypt(unsigned char *plaintext, unsigned char **key)
{
    unsigned char *tmp_plaintext = malloc(sizeof(char) * strlen(plaintext) + 1);
    unsigned int index = 0;
    memcpy(tmp_plaintext, plaintext, strlen(plaintext) + 1);
    make_plaintext_even(tmp_plaintext);
    add_X_on_duplicates(tmp_plaintext);
    make_J_to_I(tmp_plaintext);
    while (tmp_plaintext[index] != '\0')
    {
        if (is_letters_on_same_row(&tmp_plaintext[index], &tmp_plaintext[index + 1], key))
        {
        }
        else if (is_letters_on_same_col(&tmp_plaintext[index], &tmp_plaintext[index + 1], key))
        {
        }
        else
        {
            rectangle_encrypt(&tmp_plaintext[index], &tmp_plaintext[index + 1], key);
        }
        index += 2;
    }

    printf("cipher : ");

    print_bytes(stdout, tmp_plaintext);

    return tmp_plaintext;
}

unsigned char *playfair_decrypt(unsigned char *ciphertext, unsigned char **key)
{
    unsigned char *tmp_plaintext = malloc(sizeof(char) * strlen(ciphertext) + 1);
    unsigned int index = 0;
    memcpy(tmp_plaintext, ciphertext, strlen(ciphertext) + 1);
    printf("%c\n", tmp_plaintext[8]);
    printf("%c\n", tmp_plaintext[9]);
    printf("%c\n", tmp_plaintext[10]);

    while (tmp_plaintext[index] != '\0')
    {
        if (is_letters_on_same_row_decrypt(&tmp_plaintext[index], &tmp_plaintext[index + 1], key))
        {
        }
        else if (is_letters_on_same_col_decrypt(&tmp_plaintext[index], &tmp_plaintext[index + 1], key))
        {
        }
        else
        {
            rectangle_decrypt(&tmp_plaintext[index], &tmp_plaintext[index + 1], key);
        }
        index += 2;
    }
    size_t plain_len = strlen(tmp_plaintext);

    //make it a function
    index = 0;
    while (tmp_plaintext[index] != '\0')
    {
        if (replaced_with_X[index] == 1)
        {
            tmp_plaintext[index] = tmp_plaintext[index - 1];
        }
        else if (replaced_with_I[index])
        {
            tmp_plaintext[index] = 'J';
        }
        index++;
    }
    if (is_even_v == 0)
    {
        tmp_plaintext[plain_len - 1] = '\0';
    }

    printf("plain : ");

    print_bytes(stdout, tmp_plaintext);

    return tmp_plaintext;
}
void main_playfair(unsigned char *key)
{
    unsigned char **key_matrix;
    unsigned char *cipher;
    fprintf(stdout, "matrix : \n");
    key_matrix = playfair_keymatrix(key);
    print_matrix(key_matrix);
    cipher = playfair_encrypt(key, key_matrix);
    playfair_decrypt(cipher, key_matrix);
}
int main(int argc, char **argv)
{
    FILE *fp;
    uint8_t *plaintext;
    int ch;
    unsigned int count_bytes = 0, index = 0;
    if (argc > 1)
    {
        fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            fprintf(stderr, "error can't open file\n");
            return -1;
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

    //main_otp(count_bytes, plaintext);
    //main_ceasar(count_bytes, plaintext);
    main_playfair(plaintext);
    return 0;
}