#include "../include/matrix.h"
#include "../include/common.h"

int matrix_create(int rows, int cols, ULL field_size,
    Matrix** result)
{
    if (rows < 1 || cols < 1)
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

    matrix->data = (ULL**)malloc(rows * sizeof(ULL*));
    if (!matrix->data)
    {
        free(matrix);
        return MATRIX_ERROR_CREATION;
    }

    for (int i = 0; i < rows; i++)
    {
        matrix->data[i] = (ULL*)calloc(cols, sizeof(ULL));
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
    if (!src || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }

    int error;
    Matrix* dest;
    error = matrix_create(src->rows, src->cols, src->field_size,
        &dest);
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

ULL multiply_mod(ULL a, ULL b, ULL mod)
{
    if (mod == 0) return a * b;

    ULL result = 0;
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

int matrix_sum(const Matrix* a, const Matrix* b,
    Matrix** result)
{
    if (!a || !b || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }
    if (a->rows < 1 || a->cols < 1 || b->rows < 1
        || b->cols < 1)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }
    if (a->rows != b->rows || a->cols != b->cols)
    {
        return MATRIX_ERROR_DIMENSION;
    }
    if (a->field_size != b->field_size)
    {
        return MATRIX_ERROR_INVALID_FIELD;
    }

    int error;
    Matrix* sum_matrix;
    error = matrix_create(a->rows, a->cols, a->field_size,
        &sum_matrix);
    if (error != MATRIX_SUCCESS) return error;

    if (a->field_size == 0)
    {
        for (int i = 0; i < a->rows; i++)
        {
            for (int j = 0; j < a->cols; j++)
            {
                sum_matrix->data[i][j] = a->data[i][j]
                + b->data[i][j];
            }
        }
    }
    else
    {
        for (int i = 0; i < a->rows; i++)
        {
            for (int j = 0; j < a->cols; j++)
            {
                sum_matrix->data[i][j] = (a->data[i][j]
                    + b->data[i][j]) % a->field_size;
            }
        }
    }

    *result = sum_matrix;
    return MATRIX_SUCCESS;
}

int matrix_subtract(const Matrix* a, const Matrix* b,
    Matrix** result)
{
    if (!a || !b || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }
    if (a->rows < 1 || a->cols < 1 || b->rows < 1
        || b->cols < 1)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }
    if (a->rows != b->rows || a->cols != b->cols)
    {
        return MATRIX_ERROR_DIMENSION;
    }
    if (a->field_size != b->field_size)
    {
        return MATRIX_ERROR_INVALID_FIELD;
    }

    int error;
    Matrix* sub_matrix;
    error = matrix_create(a->rows, a->cols, a->field_size,
        &sub_matrix);
    if (error != MATRIX_SUCCESS) return error;

    if (a->field_size == 0)
    {
        for (int i = 0; i < a->rows; i++)
        {
            for (int j = 0; j < a->cols; j++)
            {
                sub_matrix->data[i][j] = a->data[i][j]
                - b->data[i][j];
            }
        }
    }
    else
    {
        for (int i = 0; i < a->rows; i++)
        {
            for (int j = 0; j < a->cols; j++)
            {
                sub_matrix->data[i][j] =
                    (a->data[i][j] - b->data[i][j] +
                        a->field_size) % a->field_size;
            }
        }
    }

    *result = sub_matrix;
    return MATRIX_SUCCESS;
}

int matrix_scalar_multiply(const Matrix* a, ULL scalar,
    Matrix** result)
{
    if (!a || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }
    if (a->rows < 1 || a->cols < 1)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }

    int error;
    Matrix* scaled_matrix;
    error = matrix_create(a->rows, a->cols, a->field_size,
        &scaled_matrix);
    if (error != MATRIX_SUCCESS) return error;

    if (a->field_size == 0)
    {
        for (int i = 0; i < a->rows; i++)
        {
            for (int j = 0; j < a->cols; j++)
            {
                scaled_matrix->data[i][j] = a->data[i][j]
                * scalar;
            }
        }
    }
    else
    {
        for (int i = 0; i < a->rows; i++)
        {
            for (int j = 0; j < a->cols; j++)
            {
                scaled_matrix->data[i][j] =
                    multiply_mod(a->data[i][j], scalar,
                        a->field_size);
            }
        }
    }

    *result = scaled_matrix;
    return MATRIX_SUCCESS;
}

int matrix_transpose(const Matrix* a, Matrix** result)
{
    if (!a || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }
    if (a->rows < 1 || a->cols < 1)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }

    int error;
    Matrix* transposed;
    error = matrix_create(a->cols, a->rows, a->field_size,
        &transposed);
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

int matrix_multiply(const Matrix* a, const Matrix* b,
    Matrix** result)
{
    if (!a || !b || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }
    if (a->rows < 1 || a->cols < 1 || b->rows < 1
        || b->cols < 1)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }
    if (a->cols != b->rows)
    {
        return MATRIX_ERROR_DIMENSION;
    }
    if (a->field_size != b->field_size)
    {
        return MATRIX_ERROR_INVALID_FIELD;
    }

    int error;
    Matrix* product;
    error = matrix_create(a->rows, b->cols, a->field_size,
        &product);
    if (error != MATRIX_SUCCESS) return error;

    int i, j, k;
    if (a->field_size == 0)
    {
        for (i = 0; i < a->rows; i++)
        {
            for (j = 0; j < b->cols; j++)
            {
                ULL sum = 0;
                for (k = 0; k < a->cols; k++)
                {
                    sum += a->data[i][k] * b->data[k][j];
                }
                product->data[i][j] = sum;
            }
        }
    }
    else
    {
        for (i = 0; i < a->rows; i++)
        {
            for (j = 0; j < b->cols; j++)
            {
                ULL sum = 0;
                for (k = 0; k < a->cols; k++)
                {
                    sum = (sum + multiply_mod(a->data[i][k],
                        b->data[k][j], a->field_size)) %
                            a->field_size;
                }
                product->data[i][j] = sum;
            }
        }
    }

    *result = product;
    return MATRIX_SUCCESS;
}


int matrix_submatrix(const Matrix* a, int start_row,
    int end_row, int start_col, int end_col,
    Matrix** result)
{
    if (!a || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }
    if (start_row < 0 || end_row >= a->rows || start_col < 0
        || end_col >= a->cols ||
        start_row > end_row || start_col > end_col)
    {
        return MATRIX_ERROR_DIMENSION;
    }

    int sub_rows = end_row - start_row + 1;
    int sub_cols = end_col - start_col + 1;

    int error;
    Matrix* submatrix;
    error = matrix_create(sub_rows, sub_cols, a->field_size,
        &submatrix);
    if (error != MATRIX_SUCCESS) return error;

    int i, j;
    for (i = 0; i < sub_rows; i++)
    {
        for (j = 0; j < sub_cols; j++)
        {
            submatrix->data[i][j] = a->data[start_row + i]
            [start_col + j];
        }
    }

    *result = submatrix;
    return MATRIX_SUCCESS;
}

int matrix_power(const Matrix* base, ULL exponent,
    Matrix** result)
{
    if (!base || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }
    if (base->rows != base->cols)
    {
        return MATRIX_ERROR_NOT_SQUARE;
    }

    int error;
    Matrix* result_matrix;
    Matrix* temp_power;

    if (exponent == 0)
    {
        error = matrix_create(base->rows, base->cols,
            base->field_size, &result_matrix);
        if (error != MATRIX_SUCCESS) return error;

        for (int i = 0; i < base->rows; i++)
        {
            result_matrix->data[i][i] = 1;
        }

        *result = result_matrix;
        return MATRIX_SUCCESS;
    }

    if (exponent == 1)
    {
        return matrix_copy(base, result);
    }

    error = matrix_create(base->rows, base->cols,
        base->field_size, &result_matrix);
    if (error != MATRIX_SUCCESS) return error;
    for (int i = 0; i < base->rows; i++)
    {
        result_matrix->data[i][i] = 1;
    }

    error = matrix_copy(base, &temp_power);
    if (error != MATRIX_SUCCESS)
    {
        matrix_free(result_matrix);
        return error;
    }

    ULL exp = exponent;


    // O(log(exp))
    while (exp > 0)
    {
        if (exp & 1)
        {
            Matrix* temp_result;
            // O(size^3)
            error = matrix_multiply(result_matrix,
                temp_power, &temp_result);
            if (error != MATRIX_SUCCESS)
            {
                matrix_free(result_matrix);
                matrix_free(temp_power);
                return error;
            }
            matrix_free(result_matrix);
            result_matrix = temp_result;
        }

        exp >>= 1;
        if (exp > 0)
        {
            Matrix* temp_square;
            error = matrix_multiply(temp_power,
                temp_power, &temp_square);
            if (error != MATRIX_SUCCESS)
            {
                matrix_free(result_matrix);
                matrix_free(temp_power);
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

    printf("Matrix %dx%d (field size: %llu):\n",
        matrix->rows, matrix->cols, matrix->field_size);
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

int matrix_power0(const Matrix* A, ULL n, Matrix** R)
{
    int err;
    if (n == 0)
    {
        err = matrix_create(A->rows, A->cols, A->field_size, R);
        if (err != MATRIX_SUCCESS) return err;
        for (int i = 0; i < A->rows; i++)
        {
            (*R)->data[i][i] = 1;
        }
        return MATRIX_SUCCESS;
    }
    if (n == 1)
    {
        return matrix_copy(A, R);
    }
    Matrix* result;
    err = matrix_create(A->rows, A->cols, A->field_size,
        &result);
    if (err != MATRIX_SUCCESS) return err;
    for (int i = 0; i < A->rows; i++)
    {
        result->data[i][i] = 1;
    }
    Matrix* temp;
    err = matrix_copy(A, &temp);
    if (err != MATRIX_SUCCESS)
    {
        matrix_free(result);
        return err;
    }
    ULL exp = n;
    while (exp > 0)
    {
        if (exp & 1)
        {
            Matrix* new_result;
            err = matrix_multiply(result, temp,
                &new_result);
            if (err != MATRIX_SUCCESS)
            {
                matrix_free(result);
                matrix_free(temp);
                return err;
            }
            matrix_free(result);
            result = new_result;
        }
        exp >>= 1;
        if (exp > 0)
        {
            Matrix* new_temp;
            err = matrix_multiply(temp, temp,
                &new_temp);
            if (err != MATRIX_SUCCESS)
            {
                matrix_free(result);
                matrix_free(temp);
                return err;
            }
            matrix_free(temp);
            temp = new_temp;
        }
    }
    matrix_free(temp);
    *R = result;
    return MATRIX_SUCCESS;
}
