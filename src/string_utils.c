#include "../include/string_utils.h"

int string_to_matrix(const char* str, ULL field_size,
    Matrix** result)
{
    if (!str)
    {
        return STRING_ERROR_NULL_POINTER;
    }
    if (strlen(str) < 3)
    {
        return STRING_ERROR_INVALID_FORMAT;
    }

    int rows = 1;
    int cols = 1;

    for (int i = 1; i < (int)strlen(str) - 1; i++)
    {
        if (str[i] == ';')
        {
            rows++;
        }
        else if (str[i] == ',' && rows == 1)
        {
            cols++;
        }
    }

    int matrix_error;
    matrix_error = matrix_create(rows, cols, field_size,
        result);
    if (matrix_error != MATRIX_SUCCESS)
    {
        return STRING_ERROR_CONVERSION;
    }

    int row = 0, col = 0;
    char buffer[32];
    int buf_index = 0;
    int inside_parentheses = 0;

    for (int i = 0; i < (int)strlen(str); i++)
    {
        if (str[i] == '(')
        {
            inside_parentheses = 1;
            continue;
        }
        else if (str[i] == ')')
        {
            inside_parentheses = 0;
            break;
        }

        if (!inside_parentheses) continue;

        if (str[i] == ';' || str[i] == ',' || str[i] == ')')
        {
            if (buf_index > 0)
            {
                buffer[buf_index] = '\0';
                ULL value = strtoull(buffer, NULL, 10);
                if (field_size != 0)
                {
                    value %= field_size;
                }
                (*result)->data[row][col] = value;

                buf_index = 0;
                col++;
            }

            if (str[i] == ';')
            {
                row++;
                col = 0;
            }
        }
        else if (str[i] != ' ')
        {
            if (buf_index >= 31)
            {
                matrix_free(*result);
                *result = NULL;
                return STRING_ERROR_BUFFER_OVERFLOW;
            }
            buffer[buf_index++] = str[i];
        }
    }

    if (buf_index > 0)
    {
        buffer[buf_index] = '\0';
        ULL value = strtoull(buffer, NULL, 10);
        if (field_size != 0)
        {
            value %= field_size;
        }
        (*result)->data[row][col] = value;
    }

    return STRING_SUCCESS;
}

int matrix_to_string(const Matrix* matrix, char** result)
{
    if (!matrix || !result)
    {
        return STRING_ERROR_NULL_POINTER;
    }

    if (matrix->rows <= 0 || matrix->cols <= 0)
    {
        return STRING_ERROR_INVALID_FORMAT;
    }

    int total_chars = 2;

    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            ULL num = matrix->data[i][j];
            if (matrix->field_size != 0)
            {
                num %= matrix->field_size;
            }

            int digit_count = (num == 0) ? 1 : 0;
            ULL temp = num;
            while (temp > 0)
            {
                digit_count++;
                temp /= 10;
            }

            total_chars += digit_count;

            if (j < matrix->cols - 1)
            {
                total_chars += 1;
            }
        }

        if (i < matrix->rows - 1)
        {
            total_chars += 1;
        }
    }

    total_chars += 1;

    char* str_result = (char*)malloc(total_chars);
    if (!str_result)
    {
        return STRING_ERROR_CONVERSION;
    }

    int pos = 0;
    str_result[pos++] = '(';

    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            ULL num = matrix->data[i][j];
            if (matrix->field_size != 0)
            {
                num %= matrix->field_size;
            }

            char buffer[32];
            int buf_pos = 0;

            if (num == 0)
            {
                buffer[buf_pos++] = '0';
            }
            else
            {
                ULL temp = num;
                char digits[32];
                int digit_count = 0;

                while (temp > 0)
                {
                    digits[digit_count++] = '0' + (temp % 10);
                    temp /= 10;
                }

                for (int k = digit_count - 1; k >= 0; k--)
                {
                    buffer[buf_pos++] = digits[k];
                }
            }

            buffer[buf_pos] = '\0';
            for (int k = 0; k < buf_pos; k++)
            {
                str_result[pos++] = buffer[k];
            }

            if (j < matrix->cols - 1)
            {
                str_result[pos++] = ',';
            }
        }

        if (i < matrix->rows - 1)
        {
            str_result[pos++] = ';';
        }
    }

    str_result[pos++] = ')';
    str_result[pos] = '\0';

    *result = str_result;
    return STRING_SUCCESS;
}
