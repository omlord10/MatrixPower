#include "../include/common.h"

const char* get_matrix_error_message(int error)
{
    const char* messages[] = {
        "\nEN: Operation completed successfully \n"
        "RU: Операция выполнена успешно\n",
        "\nEN: Matrix dimensions mismatch \n"
        "RU: Несовпадение размеров матриц\n",
        "\nEN: Invalid matrix size \n"
        "RU: Недопустимое количество строк или столбцов\n",
        "\nEN: Matrix is not square \n"
        "RU: Матрица не является квадратной\n",
        "\nEN: Memory allocation failed \n"
        "RU: Ошибка выделения памяти\n",
        "\nEN: Null pointer passed to function \n"
        "RU: Передан NULL указатель\n",
        "\nEN: Field/modulus mismatch or invalid \n"
        "RU: Несовпадение поля/модуля или"
        " недопустимое значение\n"
    };
    return ( (error >= 0) && (error < sizeof(messages)/
        sizeof(messages[0])) ) ? messages[error] :
    "EN: Unknown matrix error \n"
    "RU: Неизвестная ошибка матрицы";
}

const char* get_string_error_message(int error)
{
    const char* messages[] = {
        "\nEN: Operation completed successfully \n"
        "RU: Операция выполнена успешно\n",
        "\nEN: String conversion failed \n"
        "RU: Ошибка преобразования строки\n",
        "\nEN: Invalid string format \n"
        "RU: Недопустимый формат строки\n",
        "\nEN: String buffer overflow \n"
        "RU: Переполнение буфера строки\n",
        "\nEN: Null pointer passed to function \n"
        "RU: Передан NULL указатель\n"
    };
    return ((error >= 0) && (error < sizeof(messages)/
        sizeof(messages[0]))) ?
            messages[error] : "\nEN: Unknown string error \n"
                              "RU: Неизвестная ошибка строки\n";
}
