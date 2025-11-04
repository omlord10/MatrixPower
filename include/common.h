#ifndef LAB2_COMMON_H
#define LAB2_COMMON_H


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* ---------- Коды ошибок (общие / для UI) ---------- */
/* MAIN_STATUS — общий код возврата для main/утилит */
enum MAIN_STATUS
{
    SUCCESS = 0,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_INPUT,
    ERROR_FILE_OPERATION,
    ERROR_USER_INPUT
};

/* MATRIX_STATUS — коды ошибок для операций с матрицами */
enum MATRIX_STATUS
{
    MATRIX_SUCCESS = 0,
    MATRIX_ERROR_DIMENSION,
    MATRIX_ERROR_INVALID_SIZE,
    MATRIX_ERROR_NOT_SQUARE,
    MATRIX_ERROR_CREATION,
    MATRIX_ERROR_NULL_POINTER,
    MATRIX_ERROR_INVALID_FIELD,
    MATRIX_ERROR_INVALID_NUMBER
};

/* STRING_STATUS — коды ошибок в работе со строками/парсингом */
enum STRING_STATUS
{
    STRING_SUCCESS = 0,
    STRING_ERROR_CONVERSION,
    STRING_ERROR_INVALID_FORMAT,
    STRING_ERROR_BUFFER_OVERFLOW,
    STRING_ERROR_NULL_POINTER
};

/* TEST_STATUS — коды ошибок модуля генерации тестов */
enum TEST_STATUS
{
    TEST_SUCCESS = 0,
    TEST_ERROR_GENERATION,
    TEST_ERROR_FILE_WRITE,
    TEST_ERROR_INVALID_PARAMS,
    TEST_ERROR_CLOCK,
    TEST_ERROR_OVERFLOW
};

/* UI_STATUS — коды ошибок интерфейса/печати */
enum UI_STATUS
{
    UI_SUCCESS = 0,
    UI_ERROR_INPUT,
    UI_ERROR_MENU,
    UI_ERROR_DISPLAY
};

/*
 * Получить текстовое сообщение для кода ошибки матрицы.
 * Возвращает строку с описанием ошибки.
 * Если код ошибки неизвестен, возвращает "Unknown matrix error"
 * [IN] error — числовой код ошибки из enum MATRIX_STATUS
 * [RETURN] const char* — указатель на строку с сообщением
 */
const char* get_matrix_error_message(int error);

/*
 * Получить текстовое сообщение для кода ошибки работы
 * со строками.
 * Возвращает строку с описанием ошибки.
 * Если код ошибки неизвестен, возвращает "Unknown string error"
 * [IN] error — числовой код ошибки из enum STRING_STATUS
 * [RETURN] const char* — указатель на строку с сообщением
 */
const char* get_string_error_message(int error);

#endif //LAB2_COMMON_H