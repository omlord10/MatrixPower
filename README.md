# ⚡ Matrix Power CLI (C)

## Introduction
Fast utility in C for raising square matrices to a power modulo a finite field.  
Includes a CLI with manual input, predefined tests, and a test-data generator that writes CSV with timing. Modular code: matrix operations, string parsing/serialization, tests and UI.

## Installation
Build the project using the provided Makefile:
```bash
git clone https://github.com/omlord10/MatrixPower.git
cd MatrixPower
make
```

## Usage

Run the program and choose a mode from the menu:

```bash
./matrix_power
```

**Menu options:**
- **1. Manual testing** — enter matrix, field size and exponent interactively
- **2. Predefined tests** — run built-in example matrices
- **3. Generate test data** — create CSV with random matrices, results and computation times
- **4. Exit**

**Matrix string format** (input/output):
```
(a11,a12,...; a21,a22,...; ...)
```

*Example:* `(1,2;3,4)` — 2×2 matrix

## Project Structure

```
MatrixPower/
│
├── src/
│   ├── main.c            # UI and menu
│   ├── matrix.c          # matrix operations, power algorithm
│   ├── string_utils.c    # parsing/serialization of matrices
│   ├── tests.c           # test modes and CSV generator
│   └── common.c          # enums, shared utilities
│
├── include/
│   ├── matrix.h
│   ├── string_utils.h
│   ├── tests.h
│   └── common.h
│
├── matrix_power_tests.csv
└── Makefile
```

## Run Locally 🖥️

1. **Build:**
```bash
make
```

2. **Run:**
```bash
./matrix_power
```

3. **Example interactive session:**
```
Выберите опцию: 1
Введите размер матрицы: 2
Введите размер конечного поля: 100
Введите степень: 3
Введите матрицу: (1,2;3,4)

Исходная матрица:
 1 2
 3 4

Результат возведения в степень 3:
...
Время выполнения: 123.45 микросекунд
```

Each row contains the input matrix and result in string format plus measured time in nanoseconds.

## Technologies Used 🛠

[![My Skills](https://skillicons.dev/icons?i=c,clion)](https://skillicons.dev)

## License

This project is licensed under the GPL-3.0 License.

## Support 🙋‍♂️

If you encounter issues, open an issue in the GitHub repository.
