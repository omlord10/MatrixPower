#include "../include/matrix.h"
#include "../include/inline_expansion.h"


int matrix_create(int rows, int cols, ULL modulo, Matrix** result)
{
    int error = matrix_create_check(rows, cols, result);
    if (error != MATRIX_SUCCESS) return error;

    Matrix* matrix = malloc(sizeof(Matrix));
    if (!matrix) return MATRIX_ERROR_CREATION;

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->modulo = modulo;
    matrix->data = NULL;

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

void matrix_free(Matrix** matrix_ptr)
{
    if (matrix_ptr == NULL) return;

    Matrix* matrix = *matrix_ptr;
    if (!matrix)
    {
        return;
    }

    if (matrix->data)
    {
        for (int i = 0; i < matrix->rows; i++)
        {
            if (matrix->data[i])
            {
                free(matrix->data[i]);
                matrix->data[i] = NULL;
            }
        }
        free(matrix->data);
        matrix->data = NULL;
    }

    free(matrix);
    *matrix_ptr = NULL;
}

int matrix_copy(const Matrix* src, Matrix** result)
{
    int error = matrix_copy_check(src, result);
    if (error != MATRIX_SUCCESS) return error;

    Matrix* dest = matrix_new(src->rows, src->cols, src->modulo,
        &error);
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

int multiply_mod(ULL a, ULL b, ULL mod, ULL* result)
{
    if (result == NULL) return MATRIX_ERROR_NULL_POINTER;

    if (mod == 0)
    {
        if (mul_overflow_check(a, b))
            return MATRIX_ERROR_OVERFLOW;
        *result = a * b;
        return MATRIX_SUCCESS;
    }

    a %= mod;
    b %= mod;
    ULL res = 0;

    while (b > 0)
    {
        if (b & 1) res = (res + a) % mod;
        a = (a * 2) % mod;
        b >>= 1;
    }

    *result = res;
    return MATRIX_SUCCESS;
}

int matrix_sum0(const Matrix* A, const Matrix* B, Matrix** R)
{
    Matrix* C;
    matrix_create(A->rows, A->cols, A->modulo, &C);

    for (int p = 0; p < A->rows; p++)
    {
        for (int q = 0; q < A->cols; q++)
        {
            ULL m = A->data[p][q];
            ULL n = B->data[p][q];
            ULL r;

            if (A->modulo == 0)
            {
                r = m + n;
            }
            else
            {
                r = (m % A->modulo + n % A->modulo)
                % A->modulo;
            }

            C->data[p][q] = r;
        }
    }

    *R = C;
    return 0;
}

int matrix_sum(const Matrix* a, const Matrix* b,Matrix** result)
{
    int error = matrix_sum_sub_check(a, b, result);
    if (error != MATRIX_SUCCESS) return error;
    if (*result != NULL) matrix_free(result);

    Matrix* sum_matrix = matrix_new(a->rows, a->cols, a->modulo,
        &error);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < a->cols; j++)
        {
            ULL x = a->data[i][j];
            ULL y = b->data[i][j];

            if (a->modulo == 0)
            {
                if (add_overflow_check(x, y))
                {
                    matrix_free(&sum_matrix);
                    return MATRIX_ERROR_OVERFLOW;
                }
                sum_matrix->data[i][j] = x + y;
            }
            else
            {
                sum_matrix->data[i][j] = (x + y) % a->modulo;
            }
        }
    }

    *result = sum_matrix;
    return MATRIX_SUCCESS;
}

int matrix_subtract0(const Matrix* A, const Matrix* B,
    Matrix** R)
{
    Matrix* C;
    matrix_create(A->rows, A->cols, A->modulo, &C);

    for (int p = 0; p < A->rows; p++)
    {
        for (int q = 0; q < A->cols; q++)
        {
            ULL m = A->data[p][q];
            ULL n = B->data[p][q];
            ULL r;

            if (A->modulo == 0)
            {
                r = m - n;
            }
            else
            {
                r = ((m % A->modulo) -
                    (n % A->modulo)) % A->modulo;
            }

            C->data[p][q] = r;
        }
    }

    *R = C;
    return 0;
}


int matrix_subtract(const Matrix* a, const Matrix* b,
    Matrix** result)
{
    int error = matrix_sum_sub_check(a, b, result);
    if (error != MATRIX_SUCCESS) return error;
    if (*result != NULL) matrix_free(result);

    Matrix* sub_matrix = matrix_new(a->rows, a->cols, a->modulo,
        &error);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < a->cols; j++)
        {
            ULL x = a->data[i][j];
            ULL y = b->data[i][j];
            ULL r;

            if (a->modulo == 0)
            {
                if (x < y)
                {
                    matrix_free(&sub_matrix);
                    return MATRIX_ERROR_OVERFLOW;
                }
                r = x - y;
            }
            else
            {
                r = mod_sub(x, y, a->modulo);
            }

            sub_matrix->data[i][j] = r;
        }
    }

    *result = sub_matrix;
    return MATRIX_SUCCESS;
}


int matrix_scalar_multiply0(const Matrix* A, ULL s, Matrix** R)
{
    Matrix* C;
    matrix_create(A->rows, A->cols, A->modulo, &C);

    for (int i = 0; i < A->rows; i++)
    {
        for (int j = 0; j < A->cols; j++)
        {
            ULL x = A->data[i][j];
            ULL r;

            if (A->modulo == 0)
            {
                r = x * s;
            }
            else
            {
                r = (x % A->modulo * s % A->modulo)
                % A->modulo;
            }

            C->data[i][j] = r;
        }
    }

    *R = C;
    return 0;
}

int matrix_scalar_multiply(const Matrix* a, ULL scalar,
    Matrix** result)
{
    int error = matrix_scalar_multiply_check(a, result);
    if (error != MATRIX_SUCCESS) return error;

    Matrix* scaled_matrix = matrix_new(a->rows, a->cols,
        a->modulo, &error);
    if (error != MATRIX_SUCCESS)
    {
        return error;
    }

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < a->cols; j++)
        {
            ULL x = a->data[i][j];
            ULL res;

            if (a->modulo == 0)
            {
                if (mul_overflow_check(x, scalar))
                {
                    matrix_free(&scaled_matrix);
                    return MATRIX_ERROR_OVERFLOW;
                }
                res = x * scalar;
            }
            else
            {
                multiply_mod(x, scalar, a->modulo, &res);
            }

            scaled_matrix->data[i][j] = res;
        }
    }

    *result = scaled_matrix;
    return MATRIX_SUCCESS;
}

int matrix_transpose(const Matrix* a, Matrix** result)
{
    int error = matrix_transpose_check(a, result);
    if (error != MATRIX_SUCCESS) return error;

    Matrix* transposed = matrix_new(a->cols, a->rows, a->modulo,
        &error);
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

int matrix_multiply0(const Matrix* A, const Matrix* B,
    Matrix** R)
{
    Matrix* C;
    matrix_create(A->rows, B->cols, A->modulo, &C);

    for (int i = 0; i < A->rows; i++)
    {
        for (int j = 0; j < B->cols; j++)
        {
            ULL sum = 0;

            for (int k = 0; k < A->cols; k++)
            {
                ULL term;
                if (A->modulo == 0)
                {
                    term = A->data[i][k] * B->data[k][j];
                    sum += term;
                }
                else
                {
                    term = (A->data[i][k] % A->modulo *
                            B->data[k][j] % A->modulo)
                            % A->modulo;
                    sum = (sum + term) % A->modulo;
                }
            }

            C->data[i][j] = sum;
        }
    }

    *R = C;
    return 0;
}

int matrix_multiply(const Matrix* a, const Matrix* b,
    Matrix** result)
{
    int error = matrix_multiply_check(a, b, result);
    if (error != MATRIX_SUCCESS) return error;
    if (*result != NULL) matrix_free(result);

    Matrix* product = matrix_new(a->rows, a->cols, a->modulo,
        &error);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < b->cols; j++)
        {
            ULL sum = 0;

            for (int k = 0; k < a->cols; k++)
            {
                ULL x = a->data[i][k];
                ULL y = b->data[k][j];
                ULL term;

                if (a->modulo == 0)
                {
                    if (mul_overflow_check(x, y) ||
                        add_overflow_check(sum, x*y))
                    {
                        matrix_free(&product);
                        return MATRIX_ERROR_OVERFLOW;
                    }
                    term = x * y;
                    sum += term;
                }
                else
                {
                    error = multiply_mod(x, y, a->modulo,&term);
                    if (error != MATRIX_SUCCESS)
                    {
                        matrix_free(&product);
                        return error;
                    }
                    sum = (sum + term) % a->modulo;
                }
            }

            product->data[i][j] = sum;
        }
    }

    *result = product;
    return MATRIX_SUCCESS;
}

int matrix_submatrix0(const Matrix* A, int sr, int er, int sc,
    int ec, Matrix** R)
{
    int sub_rows = er - sr + 1;
    int sub_cols = ec - sc + 1;
    Matrix* C;
    matrix_create(sub_rows, sub_cols, A->modulo, &C);

    for (int i = 0; i < sub_rows; i++)
    {
        for (int j = 0; j < sub_cols; j++)
        {
            C->data[i][j] = A->data[sr + i][sc + j];
        }
    }

    *R = C;
    return 0;
}

int matrix_submatrix(const Matrix* a, int start_row,
    int end_row, int start_col, int end_col,
    Matrix** result)
{
    int error = matrix_submatrix_check(a, start_row, end_row,
        start_col, end_col, result);
    if (error != MATRIX_SUCCESS) return error;

    int sub_rows = end_row - start_row + 1;
    int sub_cols = end_col - start_col + 1;

    Matrix* submatrix = matrix_new(sub_rows, sub_cols,
        a->modulo, &error);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < sub_rows; i++)
    {
        for (int j = 0; j < sub_cols; j++)
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
    int error = matrix_power_check(base, result);
    if (error != MATRIX_SUCCESS) return error;
    if (*result != NULL) matrix_free(result);

    if (exponent == 1)
        return matrix_copy(base, result);

    Matrix* res_matrix = matrix_new(base->rows, base->cols,
        base->modulo, &error);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < base->rows; i++)
        res_matrix->data[i][i] = 1;

    if (exponent == 0)
    {
        *result = res_matrix;
        return MATRIX_SUCCESS;
    }

    Matrix* temp_power = matrix_new(base->rows, base->cols,
        base->modulo, &error);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < base->rows; i++)
        res_matrix->data[i][i] = 1;

    error = matrix_copy(base, &temp_power);
    if (error != MATRIX_SUCCESS)
    {
        matrix_free(&res_matrix);
        return error;
    }

    ULL exp = exponent;
    while (exp > 0)
    {
        if (exp & 1)
        {
            Matrix* temp_result = matrix_new(base->rows,
                base->cols, base->modulo, &error);
            if (error != MATRIX_SUCCESS)
            {
                matrix_free(&res_matrix);
                matrix_free(&temp_power);
                return error;
            }
            matrix_free(&res_matrix);
            res_matrix = temp_result;
        }

        exp >>= 1;

        if (exp > 0)
        {
            Matrix* temp_square = NULL;
            error = matrix_multiply(temp_power, temp_power,
                &temp_square);
            if (error != MATRIX_SUCCESS)
            {
                matrix_free(&res_matrix);
                matrix_free(&temp_power);
                return error;
            }
            matrix_free(&temp_power);
            temp_power = temp_square;
        }
    }

    matrix_free(&temp_power);
    *result = res_matrix;
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
        matrix->rows, matrix->cols, matrix->modulo);
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
    if (!A || !R)
        return MATRIX_ERROR_NULL_POINTER;

    if (A->rows != A->cols)
        return MATRIX_ERROR_NOT_SQUARE;

    if (*R != NULL)
        matrix_free(R);

    int err;
    Matrix* result = NULL;
    Matrix* temp = NULL;

    if (n == 0)
    {
        err = matrix_create(A->rows, A->cols, A->modulo,
            &result);
        if (err != MATRIX_SUCCESS) return err;
        for (int i = 0; i < A->rows; i++)
            result->data[i][i] = 1;

        *R = result;
        return MATRIX_SUCCESS;
    }

    if (n == 1)
        return matrix_copy(A, R);

    err = matrix_create(A->rows, A->cols, A->modulo,
        &result);
    if (err != MATRIX_SUCCESS) return err;
    for (int i = 0; i < A->rows; i++)
        result->data[i][i] = 1;

    err = matrix_copy(A, &temp);
    if (err != MATRIX_SUCCESS)
    {
        matrix_free(&result);
        return err;
    }

    ULL exp = n;
    while (exp > 0)
    {
        if (exp & 1)
        {
            Matrix* new_result = NULL;
            err = matrix_multiply(result, temp, &new_result);
            if (err != MATRIX_SUCCESS)
            {
                matrix_free(&result);
                matrix_free(&temp);
                return err;
            }
            matrix_free(&result);
            result = new_result;
        }

        exp >>= 1;

        if (exp > 0)
        {
            Matrix* new_temp = NULL;
            err = matrix_multiply(temp, temp, &new_temp);
            if (err != MATRIX_SUCCESS)
            {
                matrix_free(&result);
                matrix_free(&temp);
                return err;
            }
            matrix_free(&temp);
            temp = new_temp;
        }
    }

    matrix_free(&temp);
    *R = result;
    return MATRIX_SUCCESS;
}
