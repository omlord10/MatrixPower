#include "../include/common.h"

const char* get_matrix_error_message(int error)
{
    const char* messages[] = {
        "Matrix operation success",
        "Matrix dimension error",
        "Invalid matrix size",
        "Matrix is not square",
        "Matrix creation error",
        "Matrix multiplication error",
        "Matrix power computation error"
    };
    return (error >= 0 && error <= 6) ? messages[error] : "Unknown matrix error";
}

const char* get_string_error_message(int error)
{
    const char* messages[] = {
        "String operation success",
        "String conversion error",
        "Invalid string format",
        "String buffer overflow"
    };
    return (error >= 0 && error <= 3) ? messages[error] : "Unknown string error";
}
