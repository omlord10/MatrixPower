#ifndef LAB2_TESTS_H
#define LAB2_TESTS_H

#include "string_utils.h"

/*
 * Сгенерировать случайную квадратную матрицу size x size.
 * Все элементы — случайные числа по модулю field_size.
 * [IN] size — размер матрицы (количество строк и столбцов)
 * [IN] field_size — размер конечного поля для модульной арифметики
 * [OUT] result — указатель на созданную матрицу
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int generate_random_matrix(int size, unsigned long long field_size, Matrix** result);

/*
 * Сгенерировать набор тестов и сохранить в CSV-файл.
 * CSV содержит: размер матрицы, степень, поле, матрица, результат, время выполнения.
 * Особенности:
 * - num_tests = общее число экспериментов (например 10000)
 * - распределяем эксп. по диапазону степеней min_exponent..max_exponent (включительно)
 *   равномерно: base_count = num_tests / num_degrees; остаток добавляем к max_exponent.
 * - Для каждого эксперимента:
 *     - размер матрицы выбирается случайно между min_size и max_size (включительно)
 *       (если тебе нужно, чтобы размер = 2^k, замени логику выбора размера ниже).
 *     - exponent для matrix_power выбирается случайно в диапазоне [min_exponent, max_exponent]
 *       (если хочешь фиксировать exponent == текущая степень — можно изменить).
 * - Измерение времени делается через clock_gettime(CLOCK_MONOTONIC).
 * - Формируется два файла:
 *     output-short.txt   (matrix_size exponent field_size computation_time_ns)
 *     filename (CSV)     (matrix_size,exponent,field_size,matrix_data,result_data,computation_time_ns)

 * [IN] filename — имя выходного CSV-файла
 * [IN] min_size, max_size — диапазон размеров матриц (включительно)
 * [IN] num_tests — количество тестов
 * [IN] min_exponent, max_exponent — диапазон степеней для возведения матрицы
 * [IN] field_size — размер конечного поля
 * [RETURN] TEST_SUCCESS или код ошибки TEST_STATUS
 */

int generate_test_cases(const char* filename, int min_size, int max_size, int num_tests,
                       unsigned long long min_exponent, unsigned long long max_exponent,
                       unsigned long long field_size);

/*
 * Генерация тестов с взаимодействием с пользователем (CSV-файл).
 * Выполняется ввод параметров через консоль и генерация тестов.
 * [RETURN] UI_SUCCESS или код ошибки UI_STATUS
 */
int file_operations_test(void);

/*
 * Ручной ввод матрицы и параметров для тестирования функций.
 * Позволяет пользователю ввести матрицу и степень, выводит результат.
 * [RETURN] UI_SUCCESS или код ошибки UI_STATUS
 */
int input_test(void);

/*
 * Предопределённые тесты для проверки функций работы с матрицами.
 * Используется для быстрого тестирования без пользовательского ввода.
 * [RETURN] UI_SUCCESS или код ошибки UI_STATUS
 */
int manual_test(void);

#endif //LAB2_TESTS_H