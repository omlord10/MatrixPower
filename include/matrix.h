#ifndef LAB2_MATRIX_H
#define LAB2_MATRIX_H

#include "common.h"

typedef unsigned long long ULL;

/* Структура данных для матрицы */
typedef struct Matrix
{
    ULL** data;      /* указатель на массив строк */
    ULL field_size;  /* модуль (размер конечного поля) */
    int rows;                       /* число строк */
    int cols;                       /* число столбцов */
} Matrix;

/* ---------- Функции (матрицы) ---------- */

/*
 * Создать матрицу rows x cols, поле field_size.
 * result — выходной параметр (адрес указателя на Matrix).
 * [IN] rows - количество строк матрицы
 * [IN] cols - количество столбцов матрицы
 * [IN] field_size - размер данных (по умолчанию, 0)
 * [OUT] result - указатель на созданную матрицу
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_create(int rows, int cols, ULL field_size,
    Matrix** result);

/*
 * Освободить память, выделенную под матрицу.
 * Уничтожает структуру и внутренние данные, предотвращая
 * утечку памяти.
 * Безопасно при передаче NULL (не вызывает разыменования NULL).
 * [IN] matrix — указатель на созданную матрицу для удаления
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_free(Matrix** matrix);

/*
 * Создать копию матрицы src и вернуть её через result.
 * Полностью дублирует размеры, поле field_size и данные.
 * [IN] src — исходная матрица
 * [OUT] result — указатель на указатель, куда будет записана
 * новая копия
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_copy(const Matrix* src, Matrix** result);

/*
 * Сложить две матрицы одинакового размера: a + b.
 * Операция выполняется в поле field_size, если оно задано.
 * [IN] a — первая матрица
 * [IN] b — вторая матрица
 * [OUT] result — указатель на новую матрицу, содержащую сумму
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_sum(const Matrix* a, const Matrix* b,
    Matrix** result);

/*
 * Вычесть матрицу b из матрицы a: a - b.
 * Операция выполняется в поле field_size, если оно задано.
 * [IN] a — уменьшаемое
 * [IN] b — вычитаемое
 * [OUT] result — указатель на матрицу результата
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_subtract(const Matrix* a, const Matrix* b,
    Matrix** result);

/*
 * Умножить матрицу a на скаляр scalar в поле field_size.
 * Каждое значение элемента матрицы умножается на scalar.
 * [IN] a — исходная матрица
 * [IN] scalar — множитель
 * [OUT] result — указатель на матрицу результата
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_scalar_multiply(const Matrix* a, ULL scalar,
    Matrix** result);

/*
 * Транспонировать матрицу a (перевернуть строки и столбцы).
 * Результирующая матрица имеет размеры cols x rows.
 * [IN] a — исходная матрица
 * [OUT] result — указатель на транспонированную матрицу
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_transpose(const Matrix* a, Matrix** result);

/*
 * Вырезать подматрицу из матрицы a по указанным индексам.
 * Диапазоны [start_row, end_row) и [start_col, end_col)
 * должны быть корректными.
 * [IN] a — исходная матрица
 * [IN] start_row — начальный индекс строки
 * [IN] end_row — конечный индекс строки (не включая)
 * [IN] start_col — начальный индекс столбца
 * [IN] end_col — конечный индекс столбца (не включая)
 * [OUT] result — указатель на вырезанную подматрицу
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_submatrix(const Matrix* a, int start_row, int
    end_row, int start_col, int end_col, Matrix** result);

/*
 * Перемножить матрицы a и b: a × b.
 * Количество столбцов в a должно совпадать с количеством
 * строк в b.
 * [IN] a — первая матрица (левый множитель)
 * [IN] b — вторая матрица (правый множитель)
 * [OUT] result — указатель на новую матрицу с произведением
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_multiply(const Matrix* a, const Matrix* b,
    Matrix** result);

/*
 * Умножить два числа по модулю (a * b) % mod.
 * Используется в арифметике поля field_size, предотвращая
 * переполнение.
 * [IN] a — первый множитель
 * [IN] b — второй множитель
 * [IN] mod — модуль (если 0, операция выполняется без модуля)
 * [OUT] result - указатель на результат умножения
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int multiply_mod(ULL a, ULL b, ULL mod, ULL* result);

/*
 * Возвести квадратную матрицу base в степень exponent.
 * Используется метод бинарного возведения для эффективности.
 * Операции выполняются в поле field_size.
 * [IN] base — квадратная матрица (n x n)
 * [IN] exponent — показатель степени
 * [OUT] result — указатель на результирующую матрицу
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_power(const Matrix* base, ULL exponent,
    Matrix** result);

/*
 * Напечатать матрицу в стандартный поток вывода.
 * Формат вывода зависит от field_size (по модулю или
 * обычные значения).
 * Используется для отладки и проверки корректности.
 * [IN] matrix — указатель на матрицу для печати
 * [RETURN] MATRIX_SUCCESS или код ошибки MATRIX_STATUS
 */
int matrix_print(const Matrix* matrix);

#endif //LAB2_MATRIX_H