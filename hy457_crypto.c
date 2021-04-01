#include "hy457_crypto.h"
uint8_t *otp_decrypt(uint8_t *ciphertext, uint8_t *key, unsigned int plain_size)
{
    unsigned int index = 0, length;
    uint8_t *plaintext = malloc(plain_size * sizeof(uint8_t));
    length = plain_size;
    while (length > 0)
    {
        plaintext[index] = ciphertext[index] ^ key[index];
        index++;
        length--;
    }
    return plaintext;
}

uint8_t *otp_encrypt(uint8_t *plaintext, uint8_t *key, unsigned int plain_size)
{
    unsigned int index = 0, length = 0;
    uint8_t *cipher_text;
    //uint8_t *cipher_text = malloc(length * sizeof(uint8_t));
    uint8_t *plain_sanitized = otp_sanitize_input(plaintext, plain_size);
    length = plain_size;
    cipher_text = malloc(sizeof(uint8_t) * plain_size);

    while (length > 0)
    {

        cipher_text[index] = plain_sanitized[index] ^ key[index];
        index++;
        length--;
    }

    return cipher_text;
}
uint8_t *otp_sanitize_input(uint8_t *plaintext, unsigned int plain_size)
{
    uint8_t *sanitized_plain;
    unsigned int index = 0, sanit_index = 0;
    int length = plain_size;

    sanitized_plain = malloc(sizeof(uint8_t) * length);
    memcpy(sanitized_plain, plaintext, length);
    while (index < length)
    {
        if (islower(plaintext[index]) || isupper(plaintext[index]) || isdigit(plaintext[index]))
        {
            sanitized_plain[sanit_index++] = plaintext[index];
        }

        index++;
    }
    if (sanit_index < length - 1)
    {
        memset(sanitized_plain + sanit_index, 0, length - sanit_index);
    }

    fprintf(stdout, "\n========SANITIZED PLAINTEXT=============  \n\n");
    fprintf(stdout, "========normal========================== \n");
    print_by_size(stdout, sanitized_plain, plain_size, 0);

    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, sanitized_plain, plain_size, 1);
    return sanitized_plain;
}
void print_by_size(FILE *fp, uint8_t *msg, unsigned int size, short AsBytes)
{
    if (msg == NULL)
    {
        return;
    }

    unsigned int index = 0;
    while (index < size)
    {
        if (AsBytes)
            fprintf(fp, "%x", msg[index]);
        else
            fprintf(fp, "%c", msg[index]);
        index++;
    }
    fprintf(fp, "%c", '\n');
}
uint8_t *generate__key(int length)
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

void print_with_space(FILE *fp, uint8_t *msg)
{
    if (msg == NULL)
    {
        return;
    }
    uint8_t *tmp_msg = msg;
    while (*tmp_msg != '\0')
    {
        if (*tmp_msg == '\0')
        {
            printf("lala\n");
        }
        if (isprint(*tmp_msg))
        {
            fprintf(fp, "%c ", *tmp_msg);
        }
        else
        {
            //fprintf(fp, "%c ", *tmp_msg);

            fprintf(fp, "%s ", "non_printable ");
        }
        tmp_msg++;
    }
    fprintf(fp, "%c", '\n');
}
void main_otp(unsigned int count_bytes, uint8_t *plaintext, unsigned int plain_size)
{
    uint8_t *key;
    key = generate__key(count_bytes);
    if (key == NULL)
    {
        return;
    }
    fprintf(stdout, "%c", '\n');

    uint8_t *ciphertext = otp_encrypt(plaintext, key, plain_size);
    fprintf(stdout, "\n========CIPHERTEXT=======================  \n\n");
    fprintf(stdout, "========normal========================== \n");
    print_by_size(stdout, ciphertext, count_bytes, 0);

    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, ciphertext, count_bytes, 1);

    plaintext = otp_decrypt(ciphertext, key, plain_size);
    fprintf(stdout, "\n========PLAINTEXT=======================  \n\n");

    fprintf(stdout, "========normal========================== \n");

    print_by_size(stdout, plaintext, count_bytes, 0);
    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, plaintext, count_bytes, 1);

    //close files
    //free memory
    free(key);
    free(plaintext);
}

int modulo(int left, int right)
{
    return ((left % right + right) % right);
}

uint8_t *create_alphabet()
{
    int i = 0;
    unsigned int index = 0;
    uint8_t *alphabet = malloc(sizeof(uint8_t) * ALPHABET_SIZE);

    for (i = 0; i < 10; i++)
    {
        alphabet[index++] = '0' + i;
    }
    for (i = 0; i < 26; i++)
    {
        alphabet[index++] = 'A' + i;
    }
    for (i = 0; i < 26; i++)
    {
        alphabet[index++] = 'a' + i;
    }

    //printf("lala %s\n",alphabet);
    //print_with_space(stdout, alphabet);
    return alphabet;
}
int *create_indexing_array()
{
    int i = 0;
    unsigned int index = 0;
    int *indexing = malloc(sizeof(int) * 123);

    for (i = 48; i <= 57; i++)
    {
        indexing[i] = index;
        index++;
    }
    for (i = 65; i <= 90; i++)
    {
        indexing[i] = index;
        index++;
    }
    for (i = 97; i <= 122; i++)
    {
        indexing[i] = index;
        index++;
    }
    //printf("index:%d\n", index);
    return indexing;
}
uint8_t *caesar_encrypt(uint8_t *plaintext, ushort N, unsigned int plain_size)
{
    unsigned int index = 0;
    unsigned int length = plain_size;
    uint8_t *cipher_text = malloc(length * sizeof(uint8_t));
    uint8_t *alphabet = create_alphabet();
    int *indexing_arr = create_indexing_array();
    while (length > 0)
    {
        if (isupper(plaintext[index]) || islower(plaintext[index]) || isdigit(plaintext[index]))
        {
            cipher_text[index] = alphabet[modulo(indexing_arr[plaintext[index]] + N, ALPHABET_SIZE + 1)];
        }
        else
        {
            cipher_text[index] = plaintext[index];
        }
        index++;
        length--;
    }
    //printf("lala: %c",cipher_text[0]);
    return cipher_text;
}
uint8_t *caesar_decrypt(uint8_t *ciphertext, ushort N, unsigned int plain_size)
{
    unsigned int index = 0;
    int length = plain_size;
    uint8_t *plaintext = malloc(length * sizeof(uint8_t));
    uint8_t *alphabet = create_alphabet();
    int *indexing_arr = create_indexing_array();
    while (length > 0)
    {
        if (isupper(ciphertext[index]) || islower(ciphertext[index]) || isdigit(ciphertext[index]))
        {
            plaintext[index] = alphabet[modulo(indexing_arr[ciphertext[index]] - N, ALPHABET_SIZE + 1)];
        }
        else
        {
            plaintext[index] = ciphertext[index];
        }
        index++;
        length--;
    }
    //printf("lala2: %c",plaintext[0]);

    return plaintext;
}

void main_ceasar(unsigned int count_bytes, uint8_t *plaintext)
{
    uint8_t *ciphertext;
    int offset = 3;

    fprintf(stdout, "insert offset :");
    scanf("%d", &offset);
    ciphertext = caesar_encrypt(plaintext, offset, count_bytes);
    plaintext = caesar_decrypt(ciphertext, offset, count_bytes);

    fprintf(stdout, "\n========CIPHERTEXT=======================  \n\n");
    fprintf(stdout, "========normal========================== \n");
    print_by_size(stdout, ciphertext, count_bytes, 0);

    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, ciphertext, count_bytes, 1);
    fprintf(stdout, "\n========PLAINTEXT=======================  \n\n");

    fprintf(stdout, "========normal========================== \n");

    print_by_size(stdout, plaintext, count_bytes, 0);
    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, plaintext, count_bytes, 1);

    free(plaintext);
    free(ciphertext);
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
unsigned char **playfair_keymatrix(unsigned char *key, unsigned int size)
{
    unsigned char **key_matrix = allocate_matrix(PLAYFAIR_ROW, PLAYFAIR_COL);
    unsigned short row = 0, col = 0, index = 0;
    int ascii_upper_let[91] = {0};
    unsigned int pos = 0;

    //sanitize_upper(key, size);
    print_by_size(stdout, key, size, 0);
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

void make_plaintext_even(unsigned char *plaintext, unsigned int size)
{
    printf("size : %u\n", size);
    plaintext = (char *)realloc(plaintext, size + 1);
    printf("lala\n");
    memcpy(plaintext + size, "X", 1);
}

void make_J_to_I(unsigned char *plaintext, unsigned int size)
{
    unsigned int index = 0;
    while (index < size)
    {
        if (plaintext[index] == 'J')
        {
            plaintext[index] = 'I';
        }
        index++;
    }
}

void add_X_on_duplicates(unsigned char *plaintext, unsigned int size)
{
    unsigned int index = 0, x_index = 0;
    while (index < size)
    {
        if (plaintext[index] == plaintext[index + 1])
        {
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

void sanitize_upper(unsigned char *plaintext, unsigned int size)
{
    unsigned int valid_input_index = 0, index = 0;

    while (index < size)
    {

        if (isupper(plaintext[index]))
        {
            plaintext[valid_input_index++] = plaintext[index];
        }
        else
        {
            printf("warning plaintext conntains non upper characters\n");
        }
        index++;
    }
    memset(plaintext + valid_input_index, 0, size - valid_input_index);
}
unsigned char *playfair_encrypt(unsigned char *plaintext, unsigned char **key, unsigned int size)
{
    unsigned char *tmp_plaintext;
    unsigned int index = 0;
    if (size % 2 != 0)
    {
        make_plaintext_even(plaintext, size);
        size += 1;
    }
    tmp_plaintext = malloc(sizeof(char) * size);
    memcpy(tmp_plaintext, plaintext, size);
    print_by_size(stdout, tmp_plaintext, size, 1);

    add_X_on_duplicates(tmp_plaintext, size);
    make_J_to_I(tmp_plaintext, size);
    while (index < size)
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

    //printf("cipher : ");

    //print_with_space(stdout, tmp_plaintext);

    return tmp_plaintext;
}

unsigned char *playfair_decrypt(unsigned char *ciphertext, unsigned char **key, unsigned int size)
{
    unsigned char *tmp_plaintext;
    unsigned int index = 0;
    if (size % 2 != 0)
    {
        size += 1;
    }

    tmp_plaintext = malloc(sizeof(char) * size);

    memcpy(tmp_plaintext, ciphertext, size);

    while (index < size)
    {
        //printf("index %u\n",index);
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
    return tmp_plaintext;
}
void main_playfair(unsigned char *key, unsigned int size)
{
    unsigned char **key_matrix;
    unsigned char *ciphertext, *plaintext;
    fprintf(stdout, "matrix : \n");
    key_matrix = playfair_keymatrix(key, size);
    print_matrix(key_matrix);

    ciphertext = playfair_encrypt(key, key_matrix, size);
    plaintext = playfair_decrypt(ciphertext, key_matrix, size);

    fprintf(stdout, "\n========CIPHERTEXT=======================  \n\n");
    fprintf(stdout, "========normal========================== \n");
    print_by_size(stdout, ciphertext, size, 0);

    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, ciphertext, size, 1);
    fprintf(stdout, "\n========PLAINTEXT=======================  \n\n");

    fprintf(stdout, "========normal========================== \n");

    print_by_size(stdout, plaintext, size, 0);
    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, plaintext, size, 1);

    //free(plaintext);
    //free(ciphertext);
}

uint8_t *affine_encrypt(uint8_t *plaintext, unsigned int size)
{
    uint8_t *ciphertext;
    unsigned int index = 0, cipher_index = 0;
    unsigned int x;
    unsigned int res;
    //uint8_t *plain_sanitized = affine_sanitize_input(plaintext, size);
    ciphertext = malloc(sizeof(uint8_t) * size);
    while (index < size)
    {
        if (isupper(plaintext[index]))
        {
            x = plaintext[index] - 65;
            res = modulo((a * x + b), m);
            ciphertext[cipher_index++] = res + 'A';
        }
        else
        {
            ciphertext[cipher_index++] = plaintext[index];
        }
        index++;
    }
    return ciphertext;
}
uint8_t *affine_decrypt(uint8_t *ciphertext, unsigned int size)
{
    int x, a_inverse;
    uint8_t *plaitext_tmp = malloc(sizeof(uint8_t) * size);
    unsigned int i = 0, index = 0;
    for (i = 0; i < m; i++)
    {
        if (modulo((a * i), m) == 1)
        {
            a_inverse = i;
        }
    }
    while (index < size)
    {

        if (isupper(ciphertext[index]))
        {
            plaitext_tmp[index] = modulo(a_inverse * (ciphertext[index] - 65 - b), m) + 65;
        }
        else
        {
            plaitext_tmp[index] = ciphertext[index];
        }
        index++;
    }
    return plaitext_tmp;
}

void affine_main(unsigned char *plaintext, unsigned int size)
{
    uint8_t *ciphertext, *plain_text;
    ciphertext = affine_encrypt(plaintext, size);
    plain_text = affine_decrypt(ciphertext, size);

    fprintf(stdout, "\n========CIPHERTEXT=======================  \n\n");
    fprintf(stdout, "========normal========================== \n");
    print_by_size(stdout, ciphertext, size, 0);

    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, ciphertext, size, 1);
    fprintf(stdout, "\n========PLAINTEXT=======================  \n\n");

    fprintf(stdout, "========normal========================== \n");

    print_by_size(stdout, plain_text, size, 0);
    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, plain_text, size, 1);
}

uint8_t *feistel_round(uint8_t *block, uint8_t *key)
{

    int i = 0;
    uint8_t *cipher_block = malloc(sizeof(uint8_t) * (S / 2));
    for (i = 0; i < S / 2; i++)
    {
        cipher_block[i] = modulo((block[i] * key[i]), pow(2, (S / 2)));
    }
    return cipher_block;
}
uint8_t *feistel_encrypt(uint8_t *plaintext, uint8_t keys[][S / 2], unsigned int size)
{
    int round = 0;
    unsigned int index = 0, j = 0;
    uint8_t *tmp, *cipher;
    uint8_t swap_tmp[S / 2];
    int padding_size;
    int plain_size = size;
    if (plain_size % S != 0)
    {

        //add padding
        padding_size = S - (plain_size % S);
        plaintext = (uint8_t *)realloc(plaintext, plain_size + padding_size);
        for (j = 0; j < padding_size; j++)
        {
            plaintext[j + plain_size] = '\0';
        }
        plain_size += padding_size;
    }

    cipher = malloc(sizeof(uint8_t) * (plain_size + 1));
    int blocks_amount = plain_size / S;
    int block_counter = 0, i = 0;
    memcpy(cipher, plaintext, plain_size);

    while (1)
    {
        int left_block = block_counter * S;

        for (round = 0; round < n; round++)
        {
            index = 0;
            tmp = feistel_round(cipher + left_block + S / 2, keys[round]);
            for (i = left_block; i < left_block + (S / 2); i++) //iterate leftt block
            {
                cipher[i] = cipher[i] ^ tmp[index++];
            }
            //swap
            memcpy(swap_tmp, cipher + left_block, S / 2);
            memcpy(cipher + left_block, (cipher + left_block + S / 2), S / 2);
            memcpy(cipher + left_block + S / 2, swap_tmp, S / 2);
        }
        block_counter += 1;
        if (block_counter >= blocks_amount)
            break;
    }
    return cipher;
}
uint8_t *feistel_decrypt(uint8_t *ciphertext, uint8_t keys[][S / 2], unsigned int size)
{
    int round = 0;
    unsigned int index = 0, j;
    uint8_t *tmp, *plain;
    uint8_t swap_tmp[S / 2];
    int plain_size = size;
    if (plain_size % S != 0)
        plain_size += S - plain_size % S;

    int blocks_amount = (plain_size + S - (plain_size % S)) / S;
    int block_counter = 0, i = 0;
    plain = malloc(sizeof(uint8_t) * (plain_size + 1));
    memcpy(plain, ciphertext, plain_size);

    while (1)
    {
        int left_block = block_counter * S;

        for (round = n - 1; round >= 0; round--)
        {
            //swap
            memcpy(swap_tmp, plain + left_block, S / 2);
            memcpy(plain + left_block, (plain + left_block + S / 2), S / 2);
            memcpy(plain + left_block + S / 2, swap_tmp, S / 2);

            index = 0;
            tmp = feistel_round(plain + left_block + S / 2, keys[round]);
            for (i = left_block; i < left_block + (S / 2); i++) //iterate leftt block
            {
                plain[i] = plain[i] ^ tmp[index++];
            }
        }
        block_counter += 1;
        if (block_counter >= blocks_amount)
            break;
    }
    plain[plain_size] = '\0';

    return plain;
}

void feistel_main(uint8_t *plaintext, unsigned int size)
{
    uint8_t keys[n][S / 2];
    uint8_t *cipher, *plain;
    unsigned int i = 0;
    for (i = 0; i < n; i++)
    {
        memcpy(keys[i], generate__key(S / 2), S / 2); //4 bytes key (32bits)
    }
    cipher = feistel_encrypt(plaintext, keys, size);
    plain = feistel_decrypt(cipher, keys, size);
    fprintf(stdout, "\n========CIPHERTEXT=======================  \n\n");
    fprintf(stdout, "========normal========================== \n");
    print_by_size(stdout, cipher, size, 0);

    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, cipher, size, 1);
    fprintf(stdout, "\n========PLAINTEXT=======================  \n\n");

    fprintf(stdout, "========normal========================== \n");

    print_by_size(stdout, plain, size, 0);
    fprintf(stdout, "========hex============================= \n");

    print_by_size(stdout, plain, size, 1);
}

int main(int argc, char **argv)
{
    FILE *fp;
    uint8_t *plaintext;
    int ch;
    int choice = 0;
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
            //fprintf(stdout, "%c", plaintext[index - 1]);
        }
        plaintext[index] = '\0';
        //fprintf(stdout, "%c", '\n');
    }
    else
    {
        plaintext = malloc(sizeof(uint8_t) * 1024);
        scanf("%s", plaintext);
        printf("%s\n", plaintext);
        count_bytes = strlen(plaintext);
    }

    fprintf(stdout, "press 1. for one time pad \npress 2. for ceasar \npress 3. for playfair \npress 4. for affine  \npress 5. for feistel\n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        main_otp(count_bytes, plaintext, count_bytes);
        break;

    case 2:
        main_ceasar(count_bytes, plaintext);
        break;

    case 3:
        main_playfair(plaintext, count_bytes);
        break;

    case 4:
        affine_main(plaintext, count_bytes);
        break;

    case 5:
        feistel_main(plaintext, count_bytes);
        break;

    default:
        fprintf(stderr, "wrong input\n");
        break;
    }
    return 0;
}

static void print_bytes(FILE *fp, uint8_t *msg)
{
    if (msg == NULL)
    {
        return;
    }
    uint8_t *tmp_msg = msg;
    while (*tmp_msg != '\0')
    {

        fprintf(fp, "%x ", *tmp_msg);

        tmp_msg++;
    }
    fprintf(fp, "%c", '\n');
}
