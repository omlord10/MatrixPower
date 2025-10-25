#ifndef LAB2_TESTS_H
#define LAB2_TESTS_H

#include "common.h"
#include "matrix.h"
#include "string_utils.h"

/* Генерация случайной квадратной матрицы size x size (поле field_size).
   result — выходной параметр. */
int generate_random_matrix(int size, unsigned long long field_size, Matrix** result);

/* Генерация набора тестов и запись в CSV-файл.
   filename — имя выходного файла,
   min_size, max_size — диапазон размеров (включительно),
   num_tests — число тестов,
   min_exponent, max_exponent — диапазон степеней,
   field_size — модуль.
   Возвращает TEST_SUCCESS или код ошибки. */
int generate_test_cases(const char* filename, int min_size, int max_size, int num_tests,
                       unsigned long long min_exponent, unsigned long long max_exponent,
                       unsigned long long field_size);

/* UI-ориентированные тесты (ручной ввод / предопределённые тесты).
   Эти функции используются в main.c. */
int file_operations_test(void);   /* режим генерации тестов (взаимодействие с пользователем) */
int input_test(void);            /* ручной режим (ввод матрицы) */
int manual_test(void);        /* набор предопределённых тестов */

/* Запуск всех unit-тестов (если добавишь unit-тесты без UI). */
int run_unit_tests(void);

#endif //LAB2_TESTS_H