#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STR_INIT_SIZE 64
#define STR_HEADER_SIZE (2 * sizeof(size_t) / sizeof(char))

#define error(msg) do { fputs("[ERR] " msg "\n", stderr); exit(EXIT_FAILURE); } while(0)
#define str_free(str) free(str - STR_HEADER_SIZE)
#define str_len_addr(str) ((size_t *) (str - STR_HEADER_SIZE))
#define str_cap_addr(str) ((size_t *) (str - STR_HEADER_SIZE / 2))
#define str_len(str)      (*((size_t *) (str - STR_HEADER_SIZE)))
#define str_cap(str)      (*((size_t *) (str - STR_HEADER_SIZE / 2)))

typedef uint8_t u8;
typedef int8_t i8;
typedef int32_t i32;
typedef uint64_t u64;

char *str_alloc();
char *str_realloc(char *str, size_t new_size);
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

void str_getline(char **str) {
    i32 ch;
    size_t len, cap;

    len = 0;
    cap = str_cap(*str);
    memset(*str, 0, len + 1);

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
                error("Failed to allocate memory for a string.");
        }

        len++;
    }

    *str_len_addr(*str) = len;
    *str_cap_addr(*str) = cap;
}
