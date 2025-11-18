#include "../include/tests.h"

#define EXP 19 // [2^EXP;(2^EXP)-1)

static inline uint64_t rand64(void)
{
    return ((uint64_t)(rand() & 0xFFFF) << 48) |
           ((uint64_t)(rand() & 0xFFFF) << 32) |
           ((uint64_t)(rand() & 0xFFFF) << 16) |
           (uint64_t)(rand() & 0xFFFF);
}

static inline uint32_t rand32(void)
{
    return ((uint32_t)(rand() & 0xFFFF) << 16) |
           (uint32_t)(rand() & 0xFFFF);
}

static int get_time_ns(int64_t* out_ns)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    {
        return TEST_ERROR_CLOCK;
    }
    if (ts.tv_sec > (INT64_MAX / 1000000000LL))
    {
        return TEST_ERROR_OVERFLOW;
    }

    *out_ns = (int64_t)ts.tv_sec * 1000000000LL +
        (int64_t)ts.tv_nsec;

    return 0;
}

int generate_random_matrix(int size, ULL field_size,
    Matrix** result)
{
    if (result == NULL)
        return MATRIX_ERROR_NULL_POINTER;
    if (size <= 0)
        return MATRIX_ERROR_INVALID_SIZE;

    int err = matrix_create(size, size, field_size, result);
    if (err != MATRIX_SUCCESS)
        return err;

    ULL min_number = 1ULL << EXP;
    ULL max_number = 1ULL << (EXP + 1);

    ULL range = max_number - min_number + 1;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            ULL random_value;
            ULL r = rand64();
            if (field_size == 0)
            {

                random_value = min_number + (r % range);
            }
            else
            {
                random_value = min_number + (r % range);
                random_value %= field_size;
            }

            (*result)->data[i][j] = random_value;
        }
    }

    return MATRIX_SUCCESS;
}

int generate_test_cases(const char* filename, int min_size,
    int max_size, int num_tests, ULL min_exponent,
    ULL max_exponent, ULL field_size)
{
    if (!filename)
        return TEST_ERROR_FILE_WRITE;
    if (min_size <= 0 || max_size <= 0 || min_size > max_size)
        return TEST_ERROR_INVALID_PARAMS;
    if (num_tests <= 0)
        return TEST_ERROR_INVALID_PARAMS;
    if (min_exponent > max_exponent)
        return TEST_ERROR_INVALID_PARAMS;

    FILE* csv = fopen(filename, "w");
    if (!csv) return TEST_ERROR_FILE_WRITE;

    FILE* short_out = fopen("output-short.txt", "w");
    if (!short_out)
    {
        fclose(csv);
        return TEST_ERROR_FILE_WRITE;
    }

    fprintf(csv, "matrix_size,exponent,modulo,"
                 "computation_time_ns\n");
    fprintf(short_out, "matrix_size exponent modulo "
                       "computation_time_ns\n");

    srand((unsigned)time(NULL));
    static int count_tests = 1;
    int successful_tests = 0;

    for (int test_idx = 0; test_idx < num_tests; test_idx++)
    {
        int size = min_size + (rand32() % (max_size - min_size
            + 1));
        ULL exponent = min_exponent;
        if (min_exponent != max_exponent)
        {
            exponent = min_exponent + (rand64() % (max_exponent
                - min_exponent + 1));
        }

        Matrix* M = NULL;
        int create_err = generate_random_matrix(size,
            field_size, &M);
        if (create_err != MATRIX_SUCCESS)
        {
            printf("\nFailed to create matrix: %s\n",
                get_matrix_error_message(create_err));
            continue;
        }

        char* matrix_str = NULL;
        if (matrix_to_string(M, &matrix_str) != STRING_SUCCESS)
            matrix_str = strdup("SERIALIZE_ERROR");

        int64_t t0 = 0, t1 = 0;
        if (get_time_ns(&t0) != 0) t0 = 0;
        Matrix* R = NULL;
        int pow_err = matrix_power(M, exponent, &R);
        if (get_time_ns(&t1) != 0) t1 = t0;
        ULL dt_ns = t1 - t0;

        char* result_str = NULL;
        if (pow_err == MATRIX_SUCCESS)
        {
            if (matrix_to_string(R, &result_str) !=
                STRING_SUCCESS)
                result_str = strdup("SERIALIZE_ERROR");
        }
        else
        {
            const char* msg = get_matrix_error_message(pow_err);
            result_str = strdup(msg ? msg : "POWER_ERROR");
        }

        printf("%6d %6d %12llu %12llu %12lld\n", count_tests,
            size, exponent, field_size, dt_ns);

        fprintf(short_out, "%d %llu %llu %llu\n", size,
            exponent, field_size, dt_ns);

        fprintf(csv, "%d,%llu,%llu,%lld\n",
                size, exponent, field_size,
                dt_ns);

        free(matrix_str);
        free(result_str);
        if (R) matrix_free(&R);
        matrix_free(&M);
        count_tests++;
        successful_tests++;
    }

    fclose(csv);
    fclose(short_out);

    if (successful_tests == 0)
        return TEST_ERROR_GENERATION;

    printf("Generated and ran %d tests (output: %s and "
           "output-short.txt)\n", successful_tests, filename);
    return TEST_SUCCESS;
}

int file_operations_test()
{
    printf("=== ВЫБОР РЕЖИМА ГЕНЕРАЦИИ ТЕСТОВ ===\n");
    printf("1) Фиксация по степени "
           "(случайный размер матрицы)\n");
    printf("2) Фиксация по размеру матрицы "
           "(случайная степень)\n");
    printf("Выберите режим [1-2]:");

    int mode = 0;
    if (scanf("%d", &mode) != 1 || (mode != 1
        && mode != 2))
        return UI_ERROR_INPUT;
    while (getchar() != '\n');

    int min_size = 0, max_size = 0;
    ULL min_exp = 0, max_exp = 0, static_size = 0,
    field_size = 0;
    int num_tests = 0;

    printf("\nВведите количество тестов [1-10000]:");
    if (scanf("%d", &num_tests) != 1 || num_tests < 1
        || num_tests > 10000)
        return UI_ERROR_INPUT;
    while (getchar() != '\n');

    if (mode == 1) // фиксированная степень
    {
        printf("\nВведите фиксированную степень "
               "[1-18446744073709551615]:");
        if (scanf("%llu", &static_size) != 1 || static_size < 1
            || static_size > 0xFFFFFFFFFFFFFFFFULL)
            return UI_ERROR_INPUT;
        while (getchar() != '\n');
        min_exp = max_exp = static_size;

        printf("\nВведите минимальный размер матрицы "
               "[1-1000]:");
        if (scanf("%d", &min_size) != 1 || min_size < 1
            || min_size > 1000)
            return UI_ERROR_INPUT;
        while (getchar() != '\n');

        printf("\nВведите максимальный размер матрицы "
               "[%d-1000]:", min_size);
        if (scanf("%d", &max_size) != 1 || max_size < min_size
            || max_size > 1000)
            return UI_ERROR_INPUT;
        while (getchar() != '\n');
    }
    else // фиксированный размер матрицы
    {
        printf("\nВведите фиксированный размер матрицы "
               "[1-1000]:");
        if (scanf("%d", &min_size) != 1 || min_size < 1
            || min_size > 1000)
            return UI_ERROR_INPUT;
        while (getchar() != '\n');
        max_size = min_size;

        printf("\nВведите минимальную степень [1-184467440737095"
               "51615]:");
        if (scanf("%llu", &min_exp) != 1 || min_exp < 1 ||
            min_exp > 0xFFFFFFFFFFFFFFFFULL)
            return UI_ERROR_INPUT;
        while (getchar() != '\n');

        printf("\nВведите максимальную степень [%llu-18446744073"
               "709551615]:",
            min_exp);
        if (scanf("%llu", &max_exp) != 1 || max_exp < min_exp
            || max_exp > 0xFFFFFFFFFFFFFFFFULL)
            return UI_ERROR_INPUT;
        while (getchar() != '\n');
    }

    ULL max_number = 1ULL << (EXP + 1);

    printf("\nВведите размер поля [0-%llu] (0 = без модулей):",
        max_number);
    if (scanf("%llu", &field_size) != 1)
        return UI_ERROR_INPUT;
    while (getchar() != '\n');

    printf("\nНачало генерации тестов...\n");

    int test_error = generate_test_cases(
        "matrix_power_tests.csv", min_size, max_size,
        num_tests,min_exp, max_exp, field_size);

    if (test_error == TEST_SUCCESS)
    {
        printf("\nТестовые данные успешно сохранены в"
               " matrix_power_tests.csv\n");
    }
    else
    {
        printf("\nОшибка при генерации тестов: %d\n",
            test_error);
    }

    return UI_SUCCESS;
}

int input_test()
{
    printf("=== РУЧНОЕ ТЕСТИРОВАНИЕ ===\n");

    char buffer[32767];
    int size;
    ULL exponent, field_size;

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
    printf("Введите матрицу в формате (a11,a12,...;a21,"
           "a22,...):");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        printf("Ошибка ввода матрицы\n");
        return UI_ERROR_INPUT;
    }
    buffer[strcspn(buffer, "\n")] = 0;

    Matrix* matrix;
    int string_error = string_to_matrix(buffer, field_size,
        &matrix);
    if (string_error != STRING_SUCCESS)
    {
        printf("Ошибка преобразования строки в матрицу: %s\n",
            get_string_error_message(string_error));
        return UI_ERROR_INPUT;
    }

    printf("\nИсходная матрица:\n");
    matrix_print(matrix);

    int64_t t0 = 0, t1 = 0;
    if (get_time_ns(&t0) != 0) t0 = 0;
    Matrix* R = NULL;
    int pow_err = matrix_power(matrix, exponent, &R);
    if (get_time_ns(&t1) != 0) t1 = t0;
    ULL dt_ns = t1 - t0;

    if (pow_err == MATRIX_SUCCESS)
    {
        printf("\nРезультат возведения в степень %llu:\n",
            exponent);
        matrix_print(R);

        char* result_str;
        string_error = matrix_to_string(R, &result_str);
        if (string_error == STRING_SUCCESS)
        {
            printf("\nРезультат в строковом формате: %s\n",
                result_str);
            free(result_str);
        }

        printf("Время выполнения: %llu наносекунд\n", dt_ns);

        matrix_free(&R);
    }
    else
    {
        printf("Ошибка возведения в степень: %s\n",
            get_matrix_error_message(pow_err));
    }

    matrix_free(&matrix);
    return UI_SUCCESS;
}

int manual_test()
{
    printf("=== ТЕСТИРОВАНИЕ С ИЗВЕСТНЫМИ ДАННЫМИ ===\n");

    printf("\nТест 1: Матрица 2x2 в степени 2\n");
    Matrix* m1 = NULL;
    int str_error = string_to_matrix("(1,2;3,4)", 100, &m1);
    if (str_error == STRING_SUCCESS)
    {
        Matrix* r1 = NULL;
        int mat_error = matrix_power(m1, 2, &r1);
        if (mat_error == MATRIX_SUCCESS)
        {
            matrix_print(m1);
            printf("^2 =\n");
            matrix_print(r1);
            matrix_free(&r1);
        }
        else
        {
            printf("Ошибка: %s\n",
                get_matrix_error_message(mat_error));
        }
        matrix_free(&m1);
    }
    else
    {
        printf("Ошибка создания матрицы: %s\n",
            get_string_error_message(str_error));
    }

    printf("\nТест 2: Матрица 2x2 в степени 10\n");
    Matrix* m2 = NULL;
    str_error = string_to_matrix("(1,1;1,0)", 100, &m2);
    if (str_error == STRING_SUCCESS)
    {
        Matrix* r2 = NULL;
        int mat_error = matrix_power(m2, 10, &r2);
        if (mat_error == MATRIX_SUCCESS)
        {
            matrix_print(m2);
            printf("^10 =\n");
            matrix_print(r2);
            matrix_free(&r2);
        }
        else
        {
            printf("Ошибка: %s\n",
                get_matrix_error_message(mat_error));
        }
        matrix_free(&m2);
    }
    else
    {
        printf("Ошибка создания матрицы: %s\n",
            get_string_error_message(str_error));
    }

    printf("\nТест 3: Единичная матрица в степени 5\n");
    Matrix* m3 = NULL;
    str_error = string_to_matrix("(1,0;0,1)", 100, &m3);
    if (str_error == STRING_SUCCESS)
    {
        Matrix* r3 = NULL;
        int mat_error = matrix_power(m3, 5, &r3);
        if (mat_error == MATRIX_SUCCESS)
        {
            matrix_print(m3);
            printf("^5 =\n");
            matrix_print(r3);
            matrix_free(&r3);
        }
        else
        {
            printf("Ошибка: %s\n",
                get_matrix_error_message(mat_error));
        }
        matrix_free(&m3);
    }
    else
    {
        printf("Ошибка создания матрицы: %s\n",
            get_string_error_message(str_error));
    }

    return UI_SUCCESS;
}