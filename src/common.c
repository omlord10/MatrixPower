#include "../include/common.h"

const char* get_matrix_error_message(int error)
{
    const char* messages[] = {
        "\nEN: Operation completed successfully \nRU: Операция выполнена успешно\n",
        "\nEN: Matrix dimensions mismatch \nRU: Несовпадение размеров матриц\n",
        "\nEN: Invalid matrix size \nRU: Недопустимое количество строк или столбцов\n",
        "\nEN: Matrix is not square \nRU: Матрица не является квадратной\n",
        "\nEN: Memory allocation failed \nRU: Ошибка выделения памяти\n",
        "\nEN: Null pointer passed to function \nRU: Передан NULL указатель\n",
        "\nEN: Field/modulus mismatch or invalid \nRU: Несовпадение поля/модуля или недопустимое значение\n"
    };
    return ( (error >= 0) && (error < sizeof(messages)/sizeof(messages[0])) ) ? messages[error] : "EN: Unknown matrix error \nRU: Неизвестная ошибка матрицы";
}

const char* get_string_error_message(int error)
{
    const char* messages[] = {
        "\nEN: Operation completed successfully \nRU: Операция выполнена успешно\n",
        "\nEN: String conversion failed \nRU: Ошибка преобразования строки\n",
        "\nEN: Invalid string format \nRU: Недопустимый формат строки\n",
        "\nEN: String buffer overflow \nRU: Переполнение буфера строки\n",
        "\nEN: Null pointer passed to function \nRU: Передан NULL указатель\n"
    };
    return ((error >= 0) && (error < sizeof(messages)/sizeof(messages[0]))) ?
            messages[error] : "\nEN: Unknown string error \nRU: Неизвестная ошибка строки\n";
}
