#ifndef LAB2_MATRIX_INLINE_EXPANSION_H
#define LAB2_MATRIX_INLINE_EXPANSION_H

#include "matrix.h"

/* ---------- Базовые проверки и операции ---------- */

/*
 * Проверить переполнение при сложении беззнаковых чисел.
 * [IN] a — первое слагаемое
 * [IN] b — второе слагаемое
 * [RETURN] 1 если a + b > ULLONG_MAX, иначе 0
 */
static inline int add_overflow_check(ULL a, ULL b)
{
    return (a > ULLONG_MAX - b);
}

/*
 * Проверить переполнение при умножении беззнаковых чисел.
 * [IN] a — первый множитель
 * [IN] b — второй множитель
 * [RETURN] 1 если a * b > ULLONG_MAX, иначе 0
 */
static inline int mul_overflow_check(ULL a, ULL b)
{
    return (a != 0 && b > ULLONG_MAX / a);
}

/*
 * Выполнить вычитание по модулю: (x - y) mod modulo.
 * [IN] x — уменьшаемое
 * [IN] y — вычитаемое
 * [IN] modulo — модуль для операции
 * [RETURN] результат (x - y) mod modulo
 */
static inline ULL mod_sub(ULL x, ULL y, ULL modulo)
{
    // return (x + modulo - (y % modulo)) % modulo;
    return (x % modulo - y % modulo + modulo) % modulo;
}


/* Специфичные проверки и обёртки для matrix.c/string_utils.c */


/*
 * Проверить условия для операций сложение, вычитание.
 * [IN] a - первая матрица
 * [IN] b - вторая матрица
 * [IN] result - выходной параметр
 * [RETURN] код ошибки или MATRIX_SUCCESS
 */
static inline int matrix_sum_sub_check(const Matrix* a,
    const Matrix* b, Matrix** result)
{
    if (!a || !b || !result)
        return MATRIX_ERROR_NULL_POINTER;
    if (a->rows < 1 || a->cols < 1 ||
        b->rows < 1 || b->cols < 1)
        return MATRIX_ERROR_INVALID_SIZE;
    if (a->rows != b->rows || a->cols != b->cols)
        return MATRIX_ERROR_DIMENSION;
    if (a->modulo != b->modulo)
        return MATRIX_ERROR_INVALID_MODULO;
    return MATRIX_SUCCESS;
}

/*
 * Проверить условия для умножения матриц.
 * [IN] a - первая матрица
 * [IN] b - вторая матрица
 * [IN] result - выходной параметр
 * [RETURN] код ошибки или MATRIX_SUCCESS
 */
static inline int matrix_multiply_check(const Matrix* a,
    const Matrix* b, Matrix** result)
{
    if (!a || !b || !result)
        return MATRIX_ERROR_NULL_POINTER;
    if(a->rows < 1 || a->cols < 1 ||
        b->rows < 1 || b->cols < 1)
        return MATRIX_ERROR_INVALID_SIZE;
    if (a->cols != b->rows)
        return MATRIX_ERROR_DIMENSION;
    if (a->modulo != b->modulo)
        return MATRIX_ERROR_INVALID_MODULO;
    return MATRIX_SUCCESS;
}

/*
 * Проверить условия для возведения в степень.
 * [IN] base - базисная матрица
 * [IN] result - выходной параметр
 * [RETURN] код ошибки или MATRIX_SUCCESS
 */
static inline int matrix_power_check(const Matrix* base,
    Matrix** result)
{
    if (!base || !result)
        return MATRIX_ERROR_NULL_POINTER;
    if (base->rows != base->cols)
        return MATRIX_ERROR_NOT_SQUARE;
    return MATRIX_SUCCESS;
}

/*
 * Проверить условия для создания подматрицы.
 * [IN] a - исходная матрица
 * [IN] start_row - начальная строка
 * [IN] end_row - конечная строка
 * [IN] start_col - начальный столбец
 * [IN] end_col - конечный столбец
 * [IN] result - выходной параметр
 * [RETURN] код ошибки или MATRIX_SUCCESS
 */
static inline int matrix_submatrix_check(const Matrix* a,
    int start_row, int end_row, int start_col, int end_col,
    Matrix** result)
{
    if (!a || !result)
        return MATRIX_ERROR_NULL_POINTER;
    if (start_row < 0 || end_row >= a->rows ||
        start_col < 0 || end_col >= a->cols ||
        start_row > end_row || start_col > end_col)
        return MATRIX_ERROR_DIMENSION;
    return MATRIX_SUCCESS;
}

/*
 * Проверка параметров для matrix_create.
 * [IN] rows - количество строк
 * [IN] cols - количество столбцов
 * [IN] result - выходной параметр
 * [RETURN] код ошибки или MATRIX_SUCCESS
 */
static inline int matrix_create_check(int rows, int cols,
    Matrix** result)
{
    if (!result)
        return MATRIX_ERROR_NULL_POINTER;
    if (rows < 1 || cols < 1)
        return MATRIX_ERROR_INVALID_SIZE;
    return MATRIX_SUCCESS;
}

/*
 * Проверка параметров для matrix_copy.
 * [IN] src - исходная матрица
 * [IN] result - выходная матрица
 * [RETURN] код ошибки или MATRIX_SUCCESS
 */
static inline int matrix_copy_check(const Matrix* src,
    Matrix** result)
{
    if (!src || !result)
        return MATRIX_ERROR_NULL_POINTER;
    if (src->rows < 1 || src->cols < 1)
        return MATRIX_ERROR_INVALID_SIZE;
    if (*result == src)
        return MATRIX_ERROR_INVALID_ARGUMENT;
    return MATRIX_SUCCESS;
}

/*
 * Проверка параметров для matrix_scalar_multiply.
 * [IN] a - матрица
 * [IN] result - выходная матрица
 * [RETURN] код ошибки или MATRIX_SUCCESS
 */
static inline int matrix_scalar_multiply_check(const Matrix* a,
    Matrix** result)
{
    if (!a || !result)
        return MATRIX_ERROR_NULL_POINTER;
    if (a->rows < 1 || a->cols < 1)
        return MATRIX_ERROR_INVALID_SIZE;
    return MATRIX_SUCCESS;
}

/*
 * Проверка параметров для matrix_transpose.
 * [IN] a - матрица
 * [IN] result - выходная матрица
 * [RETURN] код ошибки или MATRIX_SUCCESS
 */
static inline int matrix_transpose_check(const Matrix* a,
    Matrix** result)
{
    if (!a || !result)
        return MATRIX_ERROR_NULL_POINTER;
    if (a->rows < 1 || a->cols < 1)
        return MATRIX_ERROR_INVALID_SIZE;
    return MATRIX_SUCCESS;
}

/*
 * Проверка параметров для matrix_print.
 * [IN] matrix - матрица
 * [RETURN] код ошибки или UI_SUCCESS
 */
static inline int matrix_print_check(const Matrix* matrix)
{
    if (!matrix)
        return UI_ERROR_DISPLAY;
    return UI_SUCCESS;
}

/*
 * Проверка параметров для string_to_matrix.
 * [IN] str - входная строка
 * [IN] result - указатель на матрицу
 * [RETURN] код ошибки или STRING_SUCCESS
 */
static inline int string_to_matrix_check(const char* str,
    Matrix** result)
{
    if (!str || !result)
        return STRING_ERROR_NULL_POINTER;

    size_t len = strlen(str);
    if (len < 3) // минимальная строка вида "(0)"
        return STRING_ERROR_INVALID_FORMAT;

    return STRING_SUCCESS;
}

/*
 * Проверка параметров для matrix_to_string.
 * [IN] matrix - исходная матрица
 * [IN] result - указатель на строку
 * [RETURN] код ошибки или STRING_SUCCESS
 */
static inline int matrix_to_string_check(const Matrix* matrix,
    char** result)
{
    if (!matrix || !result)
        return STRING_ERROR_NULL_POINTER;

    if (matrix->rows <= 0 || matrix->cols <= 0)
        return STRING_ERROR_INVALID_FORMAT;

    return STRING_SUCCESS;
}

// Создание новой матрицы
static inline Matrix* matrix_new(int rows, int cols, ULL modulo,
    int* error)
{
    Matrix* m = NULL;
    *error = matrix_create(rows, cols, modulo, &m);
    return (*error == MATRIX_SUCCESS) ? m : NULL;
}

#endif //LAB2_MATRIX_INLINE_EXPANSION_H