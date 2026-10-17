#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint8_t u8;
typedef int8_t i8;
typedef int32_t i32;
typedef uint64_t u64;

typedef struct {
    size_t length;
    size_t capacity;
} StrHeader;

#define STR_INIT_SIZE 64
#define STR_HEADER_SIZE (2 * sizeof(size_t) / sizeof(char))

#define error(msg) do { fputs("[ERR] " msg "\n", stderr); exit(EXIT_FAILURE); } while(0)

#define str_header(str)   (((StrHeader *) str) - 1)
#define str_length(str)   str_header(str)->length
#define str_capacity(str) str_header(str)->capacity
#define str_free(str)     free(str_header(str))

char *str_alloc();
char *str_realloc(char *str, size_t new_size);
void str_getline(char **str);

int main(void) {
    char *str;
    str = str_alloc();
    str_getline(&str);
    printf("(%zu/%zu) [%s]\n", str_length(str), str_capacity(str), str);
    str_free(str);

    return EXIT_SUCCESS;
}

char *str_alloc(void) {
    StrHeader *header;

    header = calloc(STR_INIT_SIZE + sizeof(StrHeader), sizeof(u8));
    header->length = 0;
    header->capacity = STR_INIT_SIZE;
    return (char *) (header + 1);
}

char *str_realloc(char *str, size_t new_size) {
    str = realloc(str_header(str), new_size + sizeof(StrHeader));
    if (str == NULL) return NULL;
    return (char *) (str + sizeof(StrHeader));
}

void str_getline(char **str) {
    i32 ch;
    size_t len, cap;

    memset(*str, 0, str_length(*str) + 1);
    len = 0;
    cap = str_capacity(*str);

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

    str_length(*str) = len;
    str_capacity(*str) = cap;
}
