#include "../include/string_utils.h"
#include "../include/inline_expansion.h"

int string_to_matrix0(const char* input_str, ULL field_size,
                      Matrix** matrix)
{
    if (!input_str || !matrix)
        return STRING_ERROR_NULL_POINTER;

    if (*matrix != NULL)
        matrix_free(matrix);

    int rows = 1, cols = 1;
    size_t len = strlen(input_str);
    for (size_t i = 1; i < len - 1; i++)
    {
        if (input_str[i] == ';') rows++;
        else if (input_str[i] == ',' && rows == 1) cols++;
    }

    int error = matrix_create(rows, cols, field_size, matrix);
    if (error != MATRIX_SUCCESS)
        return STRING_ERROR_CONVERSION;

    int row_index = 0, col_index = 0;
    char buffer[32];
    int buffer_pos = 0;
    int inside = 0;

    for (size_t i = 0; i < len; i++)
    {
        char c = input_str[i];
        if (c == '(') { inside = 1; continue; }
        else if (c == ')') { break; }
        if (!inside) continue;

        if (c == ',' || c == ';')
        {
            if (buffer_pos > 0)
            {
                buffer[buffer_pos] = '\0';
                ULL value = strtoull(buffer, NULL, 10);
                if (field_size != 0) value %= field_size;

                (*matrix)->data[row_index][col_index] = value;
                buffer_pos = 0;
                col_index++;
            }

            if (c == ';') { row_index++; col_index = 0; }
        }
        else if (c != ' ')
        {
            if (buffer_pos >= 31)
            {
                matrix_free(matrix);
                *matrix = NULL;
                return STRING_ERROR_BUFFER_OVERFLOW;
            }
            buffer[buffer_pos++] = c;
        }
    }

    if (buffer_pos > 0)
    {
        buffer[buffer_pos] = '\0';
        ULL value = strtoull(buffer, NULL, 10);
        if (field_size != 0) value %= field_size;
        (*matrix)->data[row_index][col_index] = value;
    }

    return STRING_SUCCESS;
}

int string_to_matrix(const char* str, ULL field_size,
    Matrix** result)
{
    int error = string_to_matrix_check(str, result);
    if (error != STRING_SUCCESS) return error;
    if (*result != NULL) matrix_free(result);
    size_t len = strlen(str);

    int rows = 1, cols = 1;
    for (size_t i = 1; i < len - 1; i++)
    {
        if (str[i] == ';') rows++;
        else if (str[i] == ',' && rows == 1) cols++;
    }

    int matrix_error = matrix_create(rows, cols, field_size,
        result);
    if (matrix_error != MATRIX_SUCCESS)
        return STRING_ERROR_CONVERSION;

    int row = 0, col = 0;
    char buffer[32];
    int buf_index = 0;
    int inside_parentheses = 0;

    for (size_t i = 0; i < len; i++)
    {
        char c = str[i];
        if (c == '(') { inside_parentheses = 1; continue; }
        else if (c == ')') { break; }
        if (!inside_parentheses) continue;

        if (c == ';' || c == ',')
        {
            if (buf_index > 0)
            {
                buffer[buf_index] = '\0';
                ULL value = strtoull(buffer, NULL, 10);
                if (field_size != 0) value %= field_size;

                (*result)->data[row][col] = value;
                buf_index = 0;
                col++;
            }

            if (c == ';') { row++; col = 0; }
        }
        else if (c != ' ')
        {
            if (buf_index >= 31)
            {
                matrix_free(result);
                *result = NULL;
                return STRING_ERROR_BUFFER_OVERFLOW;
            }
            buffer[buf_index++] = c;
        }
    }

    if (buf_index > 0)
    {
        buffer[buf_index] = '\0';
        ULL value = strtoull(buffer, NULL, 10);
        if (field_size != 0) value %= field_size;
        (*result)->data[row][col] = value;
    }

    return STRING_SUCCESS;
}

int matrix_to_string0(const Matrix* m, char** s)
{
    if (!m || !s)
        return STRING_ERROR_NULL_POINTER;

    if (*s != NULL)
    {
        free(*s);
        *s = NULL;
    }

    if (m->rows <= 0 || m->cols <= 0)
        return STRING_ERROR_INVALID_FORMAT;

    int total_chars = 2; // ( )

    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            ULL n = m->data[i][j];
            if (m->modulo != 0)
                n %= m->modulo;

            int digits = (n == 0) ? 1 : 0;
            ULL tmp = n;
            while (tmp > 0)
            {
                digits++;
                tmp /= 10;
            }

            total_chars += digits;
            if (j < m->cols - 1)
                total_chars++; // ,
        }
        if (i < m->rows - 1)
            total_chars++; // ;
    }

    total_chars++; // '\0'
    char* result_str = malloc(total_chars);
    if (!result_str)
        return STRING_ERROR_CONVERSION;

    int pos = 0;
    result_str[pos++] = '(';

    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            ULL n = m->data[i][j];
            if (m->modulo != 0)
                n %= m->modulo;

            char buffer[32];
            int buf_pos = 0;

            if (n == 0)
                buffer[buf_pos++] = '0';
            else
            {
                ULL tmp = n;
                char digits[32];
                int digit_count = 0;
                while (tmp > 0)
                {
                    digits[digit_count++] = (char)('0' + (tmp % 10));
                    tmp /= 10;
                }
                for (int k = digit_count - 1; k >= 0; k--)
                    buffer[buf_pos++] = digits[k];
            }

            for (int k = 0; k < buf_pos; k++)
                result_str[pos++] = buffer[k];

            if (j < m->cols - 1)
                result_str[pos++] = ',';
        }
        if (i < m->rows - 1)
            result_str[pos++] = ';';
    }

    result_str[pos++] = ')';
    result_str[pos] = '\0';
    *s = result_str;

    return STRING_SUCCESS;
}

int matrix_to_string(const Matrix* matrix, char** result)
{
    int error = matrix_to_string_check(matrix, result);
    if (error != STRING_SUCCESS) return error;
    if (*result != NULL)
    {
        free(*result);
        *result = NULL;
    }

    int total_chars = 2; // '(' и ')'

    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            ULL num = matrix->data[i][j];
            if (matrix->modulo != 0)
                num %= matrix->modulo;

            int digits = (num == 0) ? 1 : 0;
            ULL tmp = num;
            while (tmp > 0)
            {
                digits++;
                tmp /= 10;
            }

            total_chars += digits;
            if (j < matrix->cols - 1)
                total_chars++;
        }
        if (i < matrix->rows - 1)
            total_chars++;
    }

    total_chars++; // '\0'

    char* str_result = malloc(total_chars);
    if (!str_result)
        return STRING_ERROR_CONVERSION;

    int pos = 0;
    str_result[pos++] = '(';

    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            ULL num = matrix->data[i][j];
            if (matrix->modulo != 0)
                num %= matrix->modulo;

            char buffer[32];
            int buf_pos = 0;

            if (num == 0)
                buffer[buf_pos++] = '0';
            else
            {
                ULL tmp = num;
                char digits[32];
                int digit_count = 0;
                while (tmp > 0)
                {
                    digits[digit_count++] = (char)('0' + (tmp % 10));
                    tmp /= 10;
                }
                for (int k = digit_count - 1; k >= 0; k--)
                    buffer[buf_pos++] = digits[k];
            }

            for (int k = 0; k < buf_pos; k++)
                str_result[pos++] = buffer[k];

            if (j < matrix->cols - 1)
                str_result[pos++] = ',';
        }
        if (i < matrix->rows - 1)
            str_result[pos++] = ';';
    }

    str_result[pos++] = ')';
    str_result[pos] = '\0';
    *result = str_result;

    return STRING_SUCCESS;
}
