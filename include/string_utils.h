#ifndef LAB2_STRING_UTILS_H
#define LAB2_STRING_UTILS_H

#include "matrix.h"

/*
 * Преобразовать матрицу в строковый формат.
 * Формат: (a11,a12;a21,a22,...)
 * Строка выделяется через malloc, необходимо освободить
 * через free.
 * [IN] matrix — указатель на исходную матрицу
 * [OUT] result — указатель на строку с результатом
 * [RETURN] STRING_SUCCESS или код ошибки STRING_STATUS
 */
int matrix_to_string(const Matrix* matrix, char** result);

/*
 * Преобразовать строку вида "(...)" в матрицу.
 * Строка должна содержать элементы через ',' и строки
 * через ';'.
 * [IN] str — входная строка с данными матрицы
 * [IN] modulo — размер поля для модульной арифметики
 * [OUT] result — указатель на созданную матрицу
 * [RETURN] STRING_SUCCESS или код ошибки STRING_STATUS
 */
int string_to_matrix(const char* str, unsigned long long
    field_size, Matrix** result);

#endif //LAB2_STRING_UTILS_H