#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STR_INIT_SIZE 64
#define STR_HEADER_SIZE (2 * sizeof(size_t) / sizeof(char))

#define ERROR(msg) do { fputs("[ERR] " msg "\n", stderr); exit(EXIT_FAILURE); } while(0);
#define STR_LEN_ADDR(str) ((size_t *) (str - STR_HEADER_SIZE))
#define STR_CAP_ADDR(str) ((size_t *) (str - STR_HEADER_SIZE / 2))

typedef uint8_t u8;
typedef int8_t i8;
typedef int32_t i32;
typedef uint64_t u64;

char *str_alloc();
char *str_realloc(char *str, size_t new_size);
size_t str_len(char *str);
size_t str_cap(char *str);
void str_free(char *str);
void str_getline(char **str);

int main(void) {
    char *str;
    str = str_alloc();
    str_getline(&str);
    printf("(%zu/%zu) [%s]\n", str_len(str), str_cap(str), str);
    str_free(str);

    return EXIT_SUCCESS;
}

char *str_alloc(void) {
    char *str;

    str = calloc(STR_INIT_SIZE + STR_HEADER_SIZE, sizeof(u8));
    *((size_t *) str) = 0;
    *(((size_t *) str) + 1) = STR_INIT_SIZE;
    str += STR_HEADER_SIZE;

    return str;
}

char *str_realloc(char *str, size_t new_size) {
    str = realloc(str - STR_HEADER_SIZE, new_size + STR_HEADER_SIZE);
    if (str == NULL) return NULL;
    return str + STR_HEADER_SIZE;
}

size_t str_len(char *str) {
    return *STR_LEN_ADDR(str);
}

size_t str_cap(char *str) {
    return *STR_CAP_ADDR(str);
}

void str_free(char *str) {
    free(str - STR_HEADER_SIZE);
}

void str_getline(char **str) {
    i32 ch;
    size_t len, cap;

    len = 0;
    cap = str_cap(*str);
    memset(*str, 0, len);

    while (1) {
        ch = getchar();
        if (ch == EOF || ch == '\n') {
            (*str)[len] = 0;
            break;
        }

        (*str)[len] = (char) ch;

        if (len + 1 >= cap) {
            cap *= 2;
            *str = str_realloc(*str, cap);
            if (*str == NULL)
                ERROR("Failed to allocate memory for a string.");
        }

        len++;
    }

    *STR_LEN_ADDR(*str) = len;
    *STR_CAP_ADDR(*str) = cap;
}
