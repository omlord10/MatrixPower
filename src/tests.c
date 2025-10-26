#include "../include/tests.h"

int generate_random_matrix(int size, unsigned long long field_size, Matrix** result)
{
    int error = matrix_create(size, size, field_size, result);
    if (error != MATRIX_SUCCESS) return error;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            (*result)->data[i][j] = rand() % field_size;
        }
    }

    return MATRIX_SUCCESS;
}

int generate_test_cases(const char* filename, int min_size, int max_size, int num_tests,
                       unsigned long long min_exponent, unsigned long long max_exponent,
                       unsigned long long field_size)
{
    FILE* file = fopen(filename, "w");
    if (!file)
    {
        return TEST_ERROR_FILE_WRITE;
    }

    if (min_size <= 0 || max_size <= 0 || min_size > max_size || num_tests <= 0 ||
        min_exponent > max_exponent || field_size == 0)
    {
        fclose(file);
        return TEST_ERROR_INVALID_PARAMS;
    }

    fprintf(file, "matrix_size,exponent,field_size,matrix_data,result_data,computation_time_ns\n");

    srand((unsigned int)time(NULL));

    int successful_tests = 0;

    for (int test = 0; test < num_tests; test++)
    {
        int size = min_size + (rand() % (max_size - min_size + 1));
        unsigned long long exponent = min_exponent + (rand() % (max_exponent - min_exponent + 1));

        printf("Генерация теста %d/%d: матрица %dx%d, степень %llu... ",
               test + 1, num_tests, size, size, exponent);
        while (getchar() != '\n');

        Matrix* matrix;
        int matrix_error = generate_random_matrix(size, field_size, &matrix);
        if (matrix_error != MATRIX_SUCCESS)
        {
            printf("ОШИБКА: не удалось создать матрицу\n");
            fclose(file);
            return TEST_ERROR_GENERATION;
        }

        char* matrix_str;
        int string_error = matrix_to_string(matrix, &matrix_str);
        if (string_error != STRING_SUCCESS)
        {
            printf("ОШИБКА: не удалось преобразовать матрицу в строку\n");
            matrix_free(matrix);
            fclose(file);
            return TEST_ERROR_GENERATION;
        }

        clock_t start = clock();
        Matrix* result;
        matrix_error = matrix_power(matrix, exponent, &result);
        clock_t end = clock();

        double computation_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1e9;

        if (matrix_error == MATRIX_SUCCESS)
        {
            char* result_str;
            string_error = matrix_to_string(result, &result_str);
            if (string_error == STRING_SUCCESS)
            {
                fprintf(file, "%d,%llu,%llu,\"%s\",\"%s\",%.0f\n",
                       size, exponent, field_size, matrix_str, result_str, computation_time);
                free(result_str);
                successful_tests++;
                printf("УСПЕХ\n");
                while (getchar() != '\n');
            }
            else
            {
                printf("ОШИБКА: не удалось преобразовать результат в строку\n");
            }
            matrix_free(result);
        }
        else
        {
            printf("ОШИБКА: %s\n", get_matrix_error_message(matrix_error));
        }

        free(matrix_str);
        matrix_free(matrix);
    }

    fclose(file);

    if (successful_tests == 0)
    {
        return TEST_ERROR_GENERATION;
    }

    printf("Успешно сгенерировано тестов: %d/%d\n", successful_tests, num_tests);
    while (getchar() != '\n');
    return TEST_SUCCESS;
}

int file_operations_test()
{
    printf("=== ГЕНЕРАЦИЯ ТЕСТОВЫХ ДАННЫХ ===\n\n");

    printf("СПРАВКА:\n");
    printf("- Размер матрицы: квадратная матрица NxN\n");
    printf("- Степень: показатель для возведения матрицы в степень\n");
    printf("- Размер поля: модуль для арифметических операций\n");
    printf("- Файл результатов: CSV с временем выполнения в наносекундах\n\n");

    int min_size, max_size, num_tests;
    unsigned long long min_exp, max_exp, field_size;

    printf("Введите минимальный размер матрицы [1-10000]:");
    if (scanf("%d", &min_size) != 1 || min_size < 1 || min_size > 10000)
    {
        printf("Ошибка: неверный минимальный размер\n");
        return UI_ERROR_INPUT;
    }
    while (getchar() != '\n');

    printf("Введите максимальный размер матрицы [%d-10000]:", min_size);
    while (getchar() != '\n');
    if (scanf("%d", &max_size) != 1 || max_size < min_size || max_size > 10000)
    {
        printf("Ошибка: неверный максимальный размер\n");
        return UI_ERROR_INPUT;
    }
    while (getchar() != '\n');

    printf("Введите количество тестов [1000-10000]:");
    if (scanf("%d", &num_tests) != 1 || num_tests < 1000 || num_tests > 10000)
    {
        printf("Ошибка: неверное количество тестов\n");
        return UI_ERROR_INPUT;
    }
    while (getchar() != '\n');

    printf("Введите минимальную степень [1-1000]:");
    if (scanf("%llu", &min_exp) != 1 || min_exp < 1 || min_exp > 1000)
    {
        printf("Ошибка: неверная минимальная степень\n");
        return UI_ERROR_INPUT;
    }
    while (getchar() != '\n');

    printf("Введите максимальную степень [%llu-1000]:", min_exp);
    if (scanf("%llu", &max_exp) != 1 || max_exp < min_exp || max_exp > 1000)
    {
        printf("Ошибка: неверная максимальная степень\n");
        return UI_ERROR_INPUT;
    }
    while (getchar() != '\n');

    printf("Введите размер конечного поля [0-100000]:");
    if (scanf("%llu", &field_size) != 1 || field_size <= 0 || field_size > 100000)
    {
        printf("Ошибка: неверный размер поля\n");
        return UI_ERROR_INPUT;
    }
    while (getchar() != '\n');

    printf("\nПараметры генерации:\n");
    printf("- Размер матрицы: от %dx%d до %dx%d\n", min_size, min_size, max_size, max_size);
    printf("- Степень: от %llu до %llu\n", min_exp, max_exp);
    printf("- Количество тестов: %d\n", num_tests);
    printf("- Размер поля: %llu\n\n", field_size);

    printf("Начало генерации...\n");
    int test_error = generate_test_cases("matrix_power_tests.csv", min_size, max_size, num_tests,
                                       min_exp, max_exp, field_size);

    if (test_error == TEST_SUCCESS)
    {
        printf("\nТестовые данные успешно сохранены в matrix_power_tests.csv\n");
    }
    else
    {
        printf("\nОшибка при генерации тестов: %d\n", test_error);
    }

    return UI_SUCCESS;
}

int input_test()
{
    printf("=== РУЧНОЕ ТЕСТИРОВАНИЕ ===\n");

    char buffer[256];
    int size;
    unsigned long long exponent, field_size;

    printf("Введите размер матрицы:");
    if (scanf("%d", &size) != 1 || size <= 0)
    {
        printf("Ошибка ввода размера матрицы\n");
        return UI_ERROR_INPUT;
    }

    printf("Введите размер конечного поля:");
    if (scanf("%llu", &field_size) != 1 || field_size == 0)
    {
        printf("Ошибка ввода размера поля\n");
        return UI_ERROR_INPUT;
    }

    printf("Введите степень:");
    if (scanf("%llu", &exponent) != 1)
    {
        printf("Ошибка ввода степени\n");
        return UI_ERROR_INPUT;
    }

    getchar();
    printf("Введите матрицу в формате (a11,a12,...;a21,a22,...):");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        printf("Ошибка ввода матрицы\n");
        return UI_ERROR_INPUT;
    }
    buffer[strcspn(buffer, "\n")] = 0;

    Matrix* matrix;
    int string_error = string_to_matrix(buffer, field_size, &matrix);
    if (string_error != STRING_SUCCESS)
    {
        printf("Ошибка преобразования строки в матрицу: %s\n", get_string_error_message(string_error));
        return UI_ERROR_INPUT;
    }

    printf("\nИсходная матрица:\n");
    matrix_print(matrix);

    clock_t start = clock();
    Matrix* result;
    int matrix_error = matrix_power(matrix, exponent, &result);
    clock_t end = clock();

    if (matrix_error == MATRIX_SUCCESS)
    {
        printf("\nРезультат возведения в степень %llu:\n", exponent);
        matrix_print(result);

        char* result_str;
        string_error = matrix_to_string(result, &result_str);
        if (string_error == STRING_SUCCESS)
        {
            printf("\nРезультат в строковом формате: %s\n", result_str);
            free(result_str);
        }

        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1e6;
        printf("Время выполнения: %.8f микросекунд\n", time_taken);

        matrix_free(result);
    }
    else
    {
        printf("Ошибка возведения в степень: %s\n", get_matrix_error_message(matrix_error));
    }

    matrix_free(matrix);
    return UI_SUCCESS;
}

int manual_test()
{
    printf("=== ТЕСТИРОВАНИЕ С ИЗВЕСТНЫМИ ДАННЫМИ ===\n");

    printf("\nТест 1: Матрица 2x2 в степени 2\n");
    Matrix* m1;
    int str_error = string_to_matrix("(1,2;3,4)", 100, &m1);
    if (str_error == STRING_SUCCESS)
    {
        Matrix* r1;
        int mat_error = matrix_power(m1, 2, &r1);
        if (mat_error == MATRIX_SUCCESS)
        {
            matrix_print(m1);
            printf("^2 =\n");
            matrix_print(r1);
            matrix_free(r1);
        }
        else
        {
            printf("Ошибка: %s\n", get_matrix_error_message(mat_error));
        }
        matrix_free(m1);
    }
    else
    {
        printf("Ошибка создания матрицы: %s\n", get_string_error_message(str_error));
    }

    printf("\nТест 2: Матрица 2x2 в степени 10\n");
    Matrix* m2;
    str_error = string_to_matrix("(1,1;1,0)", 100, &m2);
    if (str_error == STRING_SUCCESS)
    {
        Matrix* r2;
        int mat_error = matrix_power(m2, 10, &r2);
        if (mat_error == MATRIX_SUCCESS)
        {
            matrix_print(m2);
            printf("^10 =\n");
            matrix_print(r2);
            matrix_free(r2);
        }
        else
        {
            printf("Ошибка: %s\n", get_matrix_error_message(mat_error));
        }
        matrix_free(m2);
    }
    else
    {
        printf("Ошибка создания матрицы: %s\n", get_string_error_message(str_error));
    }

    printf("\nТест 3: Единичная матрица в степени 5\n");
    Matrix* m3;
    str_error = string_to_matrix("(1,0;0,1)", 100, &m3);
    if (str_error == STRING_SUCCESS)
    {
        Matrix* r3;
        int mat_error = matrix_power(m3, 5, &r3);
        if (mat_error == MATRIX_SUCCESS)
        {
            matrix_print(m3);
            printf("^5 =\n");
            matrix_print(r3);
            matrix_free(r3);
        }
        else
        {
            printf("Ошибка: %s\n", get_matrix_error_message(mat_error));
        }
        matrix_free(m3);
    }
    else
    {
        printf("Ошибка создания матрицы: %s\n", get_string_error_message(str_error));
    }

    return UI_SUCCESS;
}