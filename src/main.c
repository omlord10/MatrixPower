#include "../include/common.h"
#include "../include/tests.h"

int main()
{
    printf("БЫСТРОЕ ВОЗВЕДЕНИЕ КВАДРАТНОЙ МАТРИЦЫ В СТЕПЕНЬ\n");
    printf("================================================\n\n");

    printf("СПРАВКА:\n");
    printf("1. Ручное тестирование - ввод матрицы и параметров вручную\n");
    printf("2. Тестирование с известными данными - предопределенные тесты\n");
    printf("3. Генерация тестовых данных - создание CSV файла с результатами\n");
    printf("4. Выход - завершение программы\n\n");

    int choice;
    int ui_error;

    do
    {
        printf("Меню:\n");
        printf("1. Ручное тестирование\n");
        printf("2. Тестирование с известными данными\n");
        printf("3. Генерация тестовых данных\n");
        printf("4. Выход\n");
        printf("Выберите опцию:");

        if (scanf("%d", &choice) != 1)
        {
            printf("Ошибка ввода\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice)
        {
            case 1:
                ui_error = input_test();
                if (ui_error != UI_SUCCESS)
                {
                    printf("Ошибка ручного тестирования: %d\n", ui_error);
                }
                break;
            case 2:
                ui_error = manual_test();
                if (ui_error != UI_SUCCESS)
                {
                    printf("Ошибка предопределенного тестирования: %d\n", ui_error);
                }
                break;
            case 3:
                ui_error = file_operations_test();
                if (ui_error != UI_SUCCESS)
                {
                    printf("Ошибка работы с файлами: %d\n", ui_error);
                }
                break;
            case 4:
                printf("Выход...\n");
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 4);

    return SUCCESS;
}