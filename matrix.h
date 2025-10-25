#ifndef LAB2_MATRIX_H
#define LAB2_MATRIX_H

#include "common.h"

/* Тип данных для матрицы */
typedef struct Matrix
{
    unsigned long long** data;      /* указатель на массив строк */
    int rows;                       /* число строк */
    int cols;                       /* число столбцов */
    unsigned long long field_size;  /* модуль (размер конечного поля) */
} Matrix;

/* ---------- Прототипы функций (матрицы) ---------- */

/* Возвращает строку-описание кода ошибки MATRIX_STATUS
   (реализовать в matrix.c). */
const char* get_matrix_error_message(int error);

/* Создать матрицу rows x cols, поле field_size.
   result — выходной параметр (адрес указателя на Matrix).
   Возвращает MATRIX_SUCCESS или код ошибки. */
int matrix_create(int rows, int cols, unsigned long long field_size, Matrix** result);

/* Освободить память матрицы (без NULL-deref). */
int matrix_free(Matrix* matrix);

/* Скопировать src в новый объект (через result). */
int matrix_copy(const Matrix* src, Matrix** result);

/* Арифметические операции (в поле field_size) */
int matrix_sum(const Matrix* a, const Matrix* b, Matrix** result);
int matrix_subtract(const Matrix* a, const Matrix* b, Matrix** result);
int matrix_scalar_multiply(const Matrix* a, unsigned long long scalar, Matrix** result);

/* Транспонирование, вырезание подматрицы, умножение */
int matrix_transpose(const Matrix* a, Matrix** result);
int matrix_submatrix(const Matrix* a, int start_row, int end_row, int start_col, int end_col, Matrix** result);
int matrix_multiply(const Matrix* a, const Matrix* b, Matrix** result);

/* Умножение по модулю (полезно экспортировать для тестов) */
unsigned long long multiply_mod(unsigned long long a, unsigned long long b, unsigned long long mod);

/* Быстрое возведение квадратной матрицы в степень (бинарное возведение).
   base должен быть квадратной матрицей. */
int matrix_power(const Matrix* base, unsigned long long exponent, Matrix** result);

/* Печать матрицы (удобно для отладки). */
int matrix_print(const Matrix* matrix);

#endif //LAB2_MATRIX_H