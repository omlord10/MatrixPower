#ifndef LAB2_ERRORS_H
#define LAB2_ERRORS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    MATRIX_ERROR_MULTIPLICATION,
    MATRIX_ERROR_POWER
};

/* STRING_STATUS — коды ошибок при работе со строками/парсингом */
enum STRING_STATUS
{
    STRING_SUCCESS = 0,
    STRING_ERROR_CONVERSION,
    STRING_ERROR_INVALID_FORMAT,
    STRING_ERROR_BUFFER_OVERFLOW
};

/* TEST_STATUS — коды ошибок модуля генерации тестов */
enum TEST_STATUS
{
    TEST_SUCCESS = 0,
    TEST_ERROR_GENERATION,
    TEST_ERROR_FILE_WRITE,
    TEST_ERROR_INVALID_PARAMS
};

/* UI_STATUS — коды ошибок пользовательского интерфейса/печати */
enum UI_STATUS
{
    UI_SUCCESS = 0,
    UI_ERROR_INPUT,
    UI_ERROR_MENU,
    UI_ERROR_DISPLAY
};


const char* get_matrix_error_message(int error);
const char* get_string_error_message(int error);

#endif //LAB2_ERRORS_H