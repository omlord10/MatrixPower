#include "../include/matrix.h"
#include "../include/common.h"

int matrix_create(int rows, int cols, ULL field_size, Matrix** result)
{
    if (!result)
        return MATRIX_ERROR_NULL_POINTER;

    if (rows < 1 || cols < 1)
        return MATRIX_ERROR_INVALID_SIZE;

    Matrix* matrix = malloc(sizeof(Matrix));
    if (!matrix)
        return MATRIX_ERROR_CREATION;

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->field_size = field_size;
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

int matrix_free(Matrix** matrix_ptr)
{
    if (!matrix_ptr)
    {
        return MATRIX_SUCCESS;
    }

    Matrix* matrix = *matrix_ptr;
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
                matrix->data[i] = NULL;
            }
        }
        free(matrix->data);
        matrix->data = NULL;
    }

    free(matrix);
    *matrix_ptr = NULL;

    return MATRIX_SUCCESS;
}

int matrix_copy(const Matrix* src, Matrix** result)
{
    if (!src || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }

    if (*result != NULL)
    {
        matrix_free(result);
    }

    if (src == *result)
    {
        return MATRIX_ERROR_INVALID_ARGUMENT;
    }

    int error;
    Matrix* dest;
    error = matrix_create(src->rows, src->cols, src->field_size,
        &dest);
    if (error != MATRIX_SUCCESS)
    {
        return error;
    }

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
    if (!result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }

    *result = 0;

    if (mod == 0)
    {
        if (a != 0 && b > ULLONG_MAX / a)
        {
            return MATRIX_ERROR_OVERFLOW;
        }
        *result = a * b;
        return MATRIX_SUCCESS;
    }

    ULL res = 0;
    a %= mod;
    b %= mod;

    while (b > 0)
    {
        if (b & 1)
        {
            res = (res + a) % mod;
        }
        a = (a * 2) % mod;
        b >>= 1;
    }

    *result = res;
    return MATRIX_SUCCESS;
}

int matrix_sum0(const Matrix* A, const Matrix* B, Matrix** R)
{
    Matrix* C;
    matrix_create(A->rows, A->cols, A->field_size, &C);

    for (int p = 0; p < A->rows; p++)
    {
        for (int q = 0; q < A->cols; q++)
        {
            ULL m = A->data[p][q];
            ULL n = B->data[p][q];
            ULL r;

            if (A->field_size == 0)
            {
                r = m + n;
            }
            else
            {
                r = (m % A->field_size + n % A->field_size)
                % A->field_size;
            }

            C->data[p][q] = r;
        }
    }

    *R = C;
    return 0;
}


int matrix_sum(const Matrix* a,const Matrix* b,Matrix** result)
{
    if (!a || !b || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }

    if (*result != NULL)
    {
        matrix_free(result);
    }

    if (a->rows < 1 || a->cols < 1 || b->rows < 1 ||
        b->cols < 1)
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
    if (error != MATRIX_SUCCESS)
    {
        return error;
    }

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < a->cols; j++)
        {
            ULL x = a->data[i][j];
            ULL y = b->data[i][j];

            if (a->field_size == 0)
            {
                if (x > ULLONG_MAX - y)
                {
                    matrix_free(&sum_matrix);
                    return MATRIX_ERROR_OVERFLOW;
                }
                sum_matrix->data[i][j] = x + y;
            }
            else
            {
                x %= a->field_size;
                y %= a->field_size;

                if (x > ULLONG_MAX - y)
                {
                    matrix_free(&sum_matrix);
                    return MATRIX_ERROR_OVERFLOW;
                }

                sum_matrix->data[i][j] = (x+y) % a->field_size;
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
    matrix_create(A->rows, A->cols, A->field_size, &C);

    for (int p = 0; p < A->rows; p++)
    {
        for (int q = 0; q < A->cols; q++)
        {
            ULL m = A->data[p][q];
            ULL n = B->data[p][q];
            ULL r;

            if (A->field_size == 0)
            {
                r = m - n;
            }
            else
            {
                r = ((m % A->field_size) -
                    (n % A->field_size)) % A->field_size;
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
    if (!a || !b || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }

    if (*result != NULL)
    {
        matrix_free(result);
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
    if (error != MATRIX_SUCCESS)
    {
        return error;
    }

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < a->cols; j++)
        {
            ULL x = a->data[i][j];
            ULL y = b->data[i][j];

            if (a->field_size == 0)
            {
                if (x < y)
                {
                    matrix_free(&sub_matrix);
                    return MATRIX_ERROR_OVERFLOW;
                }
                sub_matrix->data[i][j] = x - y;
            }
            else
            {
                x %= a->field_size;
                y %= a->field_size;
                sub_matrix->data[i][j] =
                    (x + a->field_size - y) % a->field_size;
            }
        }
    }

    *result = sub_matrix;
    return MATRIX_SUCCESS;
}

int matrix_scalar_multiply0(const Matrix* A, ULL s, Matrix** R)
{
    Matrix* C;
    matrix_create(A->rows, A->cols, A->field_size, &C);

    for (int i = 0; i < A->rows; i++)
    {
        for (int j = 0; j < A->cols; j++)
        {
            ULL x = A->data[i][j];
            ULL r;

            if (A->field_size == 0)
            {
                r = x * s;
            }
            else
            {
                r = (x % A->field_size * s % A->field_size)
                % A->field_size;
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
    if (!a || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }

    if (*result != NULL)
    {
        matrix_free(result);
    }

    if (a->rows < 1 || a->cols < 1)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }

    int error;
    Matrix* scaled_matrix;
    error = matrix_create(a->rows, a->cols, a->field_size,
        &scaled_matrix);
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

            if (a->field_size == 0)
            {
                if (x != 0 && scalar > ULLONG_MAX / x)
                {
                    matrix_free(&scaled_matrix);
                    return MATRIX_ERROR_OVERFLOW;
                }
                res = x * scalar;
            }
            else
            {
                error = multiply_mod(x, scalar,
                    a->field_size, &res);
                if (error != MATRIX_SUCCESS)
                {
                    matrix_free(&scaled_matrix);
                    return error;
                }
            }

            scaled_matrix->data[i][j] = res;
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

    if (*result != NULL)
    {
        matrix_free(result);
    }

    if (a->rows < 1 || a->cols < 1)
    {
        return MATRIX_ERROR_INVALID_SIZE;
    }

    int error;
    Matrix* transposed;
    error = matrix_create(a->cols, a->rows, a->field_size,
        &transposed);
    if (error != MATRIX_SUCCESS)
    {
        return error;
    }

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
    matrix_create(A->rows, B->cols, A->field_size, &C);

    for (int i = 0; i < A->rows; i++)
    {
        for (int j = 0; j < B->cols; j++)
        {
            ULL sum = 0;

            for (int k = 0; k < A->cols; k++)
            {
                ULL term;
                if (A->field_size == 0)
                {
                    term = A->data[i][k] * B->data[k][j];
                    sum += term;
                }
                else
                {
                    term = (A->data[i][k] % A->field_size *
                            B->data[k][j] % A->field_size)
                            % A->field_size;
                    sum = (sum + term) % A->field_size;
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
    if (!a || !b || !result)
    {
        return MATRIX_ERROR_NULL_POINTER;
    }

    if (*result != NULL)
    {
        matrix_free(result);
    }

    if(a->rows < 1 || a->cols < 1 || b->rows < 1 || b->cols < 1)
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
    if (error != MATRIX_SUCCESS)
    {
        return error;
    }

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < b->cols; j++)
        {
            ULL sum = 0;

            for (int k = 0; k < a->cols; k++)
            {
                ULL term;

                if (a->field_size == 0)
                {
                    if ((a->data[i][k] != 0) && (
                        b->data[k][j] > ULLONG_MAX /
                        a->data[i][k]))
                    {
                        matrix_free(&product);
                        return MATRIX_ERROR_OVERFLOW;
                    }

                    term = a->data[i][k] * b->data[k][j];

                    if (sum > ULLONG_MAX - term)
                    {
                        matrix_free(&product);
                        return MATRIX_ERROR_OVERFLOW;
                    }

                    sum += term;
                }
                else
                {
                    error = multiply_mod(a->data[i][k],
                        b->data[k][j],a->field_size, &term);
                    if (error != MATRIX_SUCCESS)
                    {
                        matrix_free(&product);
                        return error;
                    }

                    sum = (sum + term) % a->field_size;
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
    matrix_create(sub_rows, sub_cols, A->field_size, &C);

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
        return MATRIX_ERROR_NULL_POINTER;

    if (base->rows != base->cols)
        return MATRIX_ERROR_NOT_SQUARE;

    if (*result != NULL)
    {
        matrix_free(result);
    }

    int error;
    Matrix* res_matrix = NULL;
    Matrix* temp_power = NULL;

    if (exponent == 0)
    {
        error = matrix_create(base->rows, base->cols,
            base->field_size, &res_matrix);
        if (error != MATRIX_SUCCESS)
            return error;

        for (int i = 0; i < base->rows; i++)
            res_matrix->data[i][i] = 1;

        *result = res_matrix;
        return MATRIX_SUCCESS;
    }

    if (exponent == 1)
        return matrix_copy(base, result);

    error = matrix_create(base->rows, base->cols, base->field_size,
        &res_matrix);
    if (error != MATRIX_SUCCESS)
        return error;

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
            Matrix* temp_result = NULL;
            error = matrix_multiply(res_matrix, temp_power,
                &temp_result);
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
        err = matrix_create(A->rows, A->cols, A->field_size,
            &result);
        if (err != MATRIX_SUCCESS) return err;
        for (int i = 0; i < A->rows; i++)
            result->data[i][i] = 1;

        *R = result;
        return MATRIX_SUCCESS;
    }

    if (n == 1)
        return matrix_copy(A, R);

    err = matrix_create(A->rows, A->cols, A->field_size,
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
