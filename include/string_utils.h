#ifndef LAB2_STRING_UTILS_H
#define LAB2_STRING_UTILS_H

#include "matrix.h"

/* Преобразование матрицы в строковый формат:
   формат: (a11,a12; a21,a22) — строка выделяется malloc'ом, нужно free.
   Возвращает STRING_SUCCESS или код ошибки. */
int matrix_to_string(const Matrix* matrix, char** result);

/* Парсинг строки вида "(...)" в матрицу. field_size — модуль.
   result — выходной Matrix*, нужно освобождать через matrix_free.
   Возвращает STRING_SUCCESS или код ошибки. */
int string_to_matrix(const char* str, unsigned long long field_size, Matrix** result);

/* Опциональные вспомогательные функции (CSV):
   Сериализация матрицы в CSV-строку/файл и парсинг CSV (если нужен).
   Определить/реализовать по необходимости. */
int matrix_to_csv(const Matrix* matrix, const char* filename);
int csv_to_matrix(const char* filename, Matrix** result);

#endif //LAB2_STRING_UTILS_H
