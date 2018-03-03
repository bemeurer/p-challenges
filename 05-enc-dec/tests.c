#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "fifo.h"

typedef struct {
    size_t succeeded;
    size_t failed;
} result_t;

typedef enum {
    RED,
    RED_B,
    GREEN,
    GREEN_B,
    YELLOW,
    YELLOW_B,
    BLUE,
    BLUE_B,
    MAGENTA,
    MAGENTA_B,
    CYAN,
    CYAN_B,
    NORMAL,
} color_t;

static char *get_color(color_t c) {
    static char *ansi = NULL;
    switch (c) {
        case RED:
            ansi = "\033[31m";
            break;
        case RED_B:
            ansi = "\033[1m\033[31m";
            break;
        case GREEN:
            ansi = "\033[32m";
            break;
        case GREEN_B:
            ansi = "\033[1m\033[32m";
            break;
        case YELLOW:
            ansi = "\x1b[33m";
            break;
        case YELLOW_B:
            ansi = "\033[1m\x1b[33m";
            break;
        case BLUE:
            ansi = "\x1b[34m";
            break;
        case BLUE_B:
            ansi = "\033[1m\x1b[34m";
            break;
        case MAGENTA:
            ansi = "\x1b[35m";
            break;
        case MAGENTA_B:
            ansi = "\033[1m\x1b[35m";
            break;
        case CYAN:
            ansi = "\x1b[36m";
            break;
        case CYAN_B:
            ansi = "\033[1m\x1b[36m";
            break;
        case NORMAL:
            ansi = "\033[0m";
            break;
    }
    return ansi;
}

void printc(color_t c, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char *color = get_color(c);
    char *rst = get_color(NORMAL);
    size_t len = strlen(color) + strlen(format) + strlen(rst) + 1;

    char *colored = calloc(len, sizeof(char));
    snprintf(colored, len, "%s%s%s", color, format, rst);

    vprintf(colored, args);
    va_end(args);

    free(colored);
}

void section(const char *name) {
    printc(BLUE, "Testing ");
    printc(BLUE_B, name);
    printc(BLUE, ":\n");
}

bool assert(const char *name, bool result, result_t *results) {
    if (result) {
        printc(GREEN_B, "✓ ");
        printc(GREEN, "SUCCESS: ");
        ++(results->succeeded);
    } else {
        printc(RED_B, "✕m");
        printc(RED, "FAIL: ");
        ++(results->failed);
    }
    printc(YELLOW_B, "%s\n", name);
    return result;
}

void assert_section(const char *name, result_t *results) {
    printc(BLUE_B, "%s", name);
    printc(BLUE, ": ");
    color_t status = (results->failed > 0) ? RED_B : GREEN_B;
    size_t total = results->failed + results->succeeded;
    printc(status, "PASSED %zu/%zu\n", results->succeeded, total);
}

void fifo_tests() {
    section("FIFO");
    result_t results = {};
    assert("Initializing FIFO", test_fifo_initialize(), &results);
    assert("Counting FIFO", test_fifo_count(), &results);
    assert("Enqueuing empty FIFO", test_fifo_empty_enqueue(), &results);
    assert("Dequeuing empty FIFO", test_fifo_empty_dequeue(), &results);
    assert("Freeing empty FIFO", test_fifo_empty_free(), &results);
    assert("Dequeuing Singleton FIFO", test_fifo_singleton_dequeue(), &results);
    assert("Multiple enqueues", test_fifo_multiple_enqueues(), &results);
    assert("Multiple dequeues", test_fifo_multiple_dequeues(), &results);
    assert("Freeing FIFO", test_fifo_free(), &results);
    assert_section("FIFO", &results);
}

void base64_tests() {
    section("Base64");
    //result_t results = {};
}

int main() {
    fifo_tests();
    base64_tests();
}