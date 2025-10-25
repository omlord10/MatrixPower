#include "../include/matrix.h"
#include "../include/common.h"

int matrix_create(int rows, int cols, unsigned long long field_size, Matrix** result)
{
    if (rows <= 0 || cols <= 0)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }

    Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
    if (!matrix)
    {
        return MATRIX_ERROR_CREATION;
    }

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->field_size = field_size;

    matrix->data = (unsigned long long**)malloc(rows * sizeof(unsigned long long*));
    if (!matrix->data)
    {
        free(matrix);
        return MATRIX_ERROR_CREATION;
    }

    for (int i = 0; i < rows; i++)
    {
        matrix->data[i] = (unsigned long long*)calloc(cols, sizeof(unsigned long long));
        if (!matrix->data[i])
        {
            for (int j = 0; j < i; j++)
            {
                free(matrix->data[j]);
            }
            free(matrix->data);
            free(matrix);
            return MATRIX_ERROR_CREATION;
        }
    }

    *result = matrix;
    return MATRIX_SUCCESS;
}

int matrix_free(Matrix* matrix)
{
    if (!matrix)
    {
        return MATRIX_SUCCESS;
    }

    if (matrix->data)
    {
        for (int i = 0; i < matrix->rows; i++)
        {
            if (matrix->data[i])
            {
                free(matrix->data[i]);
            }
        }
        free(matrix->data);
    }
    free(matrix);
    return MATRIX_SUCCESS;
}

int matrix_copy(const Matrix* src, Matrix** result)
{
    if (!src)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }

    int error;
    Matrix* dest;
    error = matrix_create(src->rows, src->cols, src->field_size, &dest);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < src->rows; i++)
    {
        for (int j = 0; j < src->cols; j++)
        {
            dest->data[i][j] = src->data[i][j];
        }
    }

    *result = dest;
    return MATRIX_SUCCESS;
}

unsigned long long multiply_mod(unsigned long long a, unsigned long long b, unsigned long long mod)
{
    if (mod == 0) return a * b;

    unsigned long long result = 0;
    a %= mod;
    b %= mod;

    while (b > 0)
    {
        if (b & 1)
        {
            result = (result + a) % mod;
        }
        a = (a * 2) % mod;
        b >>= 1;
    }

    return result;
}

int matrix_sum(const Matrix* a, const Matrix* b, Matrix** result)
{
    if (!a || !b)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }
    if (a->rows != b->rows || a->cols != b->cols || a->field_size != b->field_size)
    {
        return MATRIX_ERROR_DIMENSION;
    }

    int error;
    Matrix* sum_matrix;
    error = matrix_create(a->rows, a->cols, a->field_size, &sum_matrix);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < a->cols; j++)
        {
            sum_matrix->data[i][j] = (a->data[i][j] + b->data[i][j]) % a->field_size;
        }
    }

    *result = sum_matrix;
    return MATRIX_SUCCESS;
}

int matrix_subtract(const Matrix* a, const Matrix* b, Matrix** result)
{
    if (!a || !b)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }
    if (a->rows != b->rows || a->cols != b->cols || a->field_size != b->field_size)
    {
        return MATRIX_ERROR_DIMENSION;
    }

    int error;
    Matrix* sub_matrix;
    error = matrix_create(a->rows, a->cols, a->field_size, &sub_matrix);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < a->cols; j++)
        {
            sub_matrix->data[i][j] = (a->data[i][j] - b->data[i][j] + a->field_size) % a->field_size;
        }
    }

    *result = sub_matrix;
    return MATRIX_SUCCESS;
}

int matrix_scalar_multiply(const Matrix* a, unsigned long long scalar, Matrix** result)
{
    if (!a)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }

    int error;
    Matrix* scaled_matrix;
    error = matrix_create(a->rows, a->cols, a->field_size, &scaled_matrix);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < a->cols; j++)
        {
            scaled_matrix->data[i][j] = multiply_mod(a->data[i][j], scalar, a->field_size);
        }
    }

    *result = scaled_matrix;
    return MATRIX_SUCCESS;
}

int matrix_transpose(const Matrix* a, Matrix** result)
{
    if (!a)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }

    int error;
    Matrix* transposed;
    error = matrix_create(a->cols, a->rows, a->field_size, &transposed);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < a->cols; j++)
        {
            transposed->data[j][i] = a->data[i][j];
        }
    }

    *result = transposed;
    return MATRIX_SUCCESS;
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix** result)
{
    if (!a || !b)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }
    if (a->cols != b->rows || a->field_size != b->field_size)
    {
        return MATRIX_ERROR_DIMENSION;
    }

    int error;
    Matrix* product;
    error = matrix_create(a->rows, b->cols, a->field_size, &product);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < b->cols; j++)
        {
            unsigned long long sum = 0;
            for (int k = 0; k < a->cols; k++)
            {
                sum = (sum + multiply_mod(a->data[i][k], b->data[k][j], a->field_size)) % a->field_size;
            }
            product->data[i][j] = sum;
        }
    }

    *result = product;
    return MATRIX_SUCCESS;
}

int matrix_submatrix(const Matrix* a, int start_row, int end_row,
                                int start_col, int end_col, Matrix** result)
{
    if (!a)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }
    if (start_row < 0 || end_row >= a->rows || start_col < 0 || end_col >= a->cols ||
        start_row > end_row || start_col > end_col)
    {
        return MATRIX_ERROR_DIMENSION;
    }

    int sub_rows = end_row - start_row + 1;
    int sub_cols = end_col - start_col + 1;

    int error;
    Matrix* submatrix;
    error = matrix_create(sub_rows, sub_cols, a->field_size, &submatrix);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < sub_rows; i++)
    {
        for (int j = 0; j < sub_cols; j++)
        {
            submatrix->data[i][j] = a->data[start_row + i][start_col + j];
        }
    }

    *result = submatrix;
    return MATRIX_SUCCESS;
}

int matrix_power(const Matrix* base, unsigned long long exponent, Matrix** result)
{
    if (!base)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }
    if (base->rows != base->cols)
    {
        return MATRIX_ERROR_NOT_SQUARE;
    }

    if (exponent == 0)
    {
        int error;
        Matrix* identity;
        error = matrix_create(base->rows, base->cols, base->field_size, &identity);
        if (error != MATRIX_SUCCESS) return error;

        for (int i = 0; i < base->rows; i++)
        {
            identity->data[i][i] = 1;
        }
        *result = identity;
        return MATRIX_SUCCESS;
    }

    if (exponent == 1)
    {
        return matrix_copy(base, result);
    }

    int error;
    Matrix* result_matrix;
    error = matrix_create(base->rows, base->cols, base->field_size, &result_matrix);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < base->rows; i++)
    {
        result_matrix->data[i][i] = 1;
    }

    Matrix* temp_power;
    error = matrix_copy(base, &temp_power);
    if (error != MATRIX_SUCCESS)
    {
        matrix_free(result_matrix);
        return error;
    }

    unsigned long long exp = exponent;

    while (exp > 0)
    {
        if (exp & 1)
        {
            Matrix* temp_result;
            error = matrix_multiply(result_matrix, temp_power, &temp_result);
            if (error != MATRIX_SUCCESS)
            {
                matrix_free(temp_power);
                matrix_free(result_matrix);
                return error;
            }
            matrix_free(result_matrix);
            result_matrix = temp_result;
        }

        exp >>= 1;
        if (exp > 0)
        {
            Matrix* temp_square;
            error = matrix_multiply(temp_power, temp_power, &temp_square);
            if (error != MATRIX_SUCCESS)
            {
                matrix_free(temp_power);
                matrix_free(result_matrix);
                return error;
            }
            matrix_free(temp_power);
            temp_power = temp_square;
        }
    }

    matrix_free(temp_power);
    *result = result_matrix;
    return MATRIX_SUCCESS;
}

int matrix_print(const Matrix* matrix)
{
    if (!matrix)
    {
        printf("NULL matrix\n");
        return UI_ERROR_DISPLAY;
    }

    printf("Matrix %dx%d (field size: %llu):\n", matrix->rows, matrix->cols, matrix->field_size);
    for (int i = 0; i < matrix->rows; i++)
    {
        printf("  ");
        for (int j = 0; j < matrix->cols; j++)
        {
            printf("%llu ", matrix->data[i][j]);
        }
        printf("\n");
    }
    return UI_SUCCESS;
}

// Остальные функции matrix_* ...
