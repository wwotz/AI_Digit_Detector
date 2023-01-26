#include "../include/debug.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEBUG_STACK_SIZE 20
#define DEBUG_FLUSH_FILE "sl_debug.log"

/* defining a circular stack which holds error messages
   required for debugging
*/
static char *stack[DEBUG_STACK_SIZE] = {0};
static int ptr = 0;
static int size = 0;

// check if the debug stack is empty
static inline int stack_empty_debug()
{
        return size == 0;
}

// check if the debug stack is full
static inline int stack_full_debug()
{
        return size == DEBUG_STACK_SIZE;
}

inline int had_error_debug(void)
{
        return !stack_empty_debug();
}

// pop the most recent error message
const char *pop_error_debug(void)
{
        if (stack_empty_debug())
                return "No Error";
        size--;
        ptr--;
        if (ptr < 0)
                ptr += DEBUG_STACK_SIZE;
        return stack[ptr];
}

// push the most recent error message
void push_error_debug(const char *err)
{
        if (err != NULL) {
                // ensures that memory is not leaked,
                // frees allocated memory for previous error messages
                if (!stack_full_debug())
                        size++;
                if (stack[ptr] != NULL) {
                        free(stack[ptr]);
                        stack[ptr] = NULL;
                }
                stack[ptr] = strdup(err);
                ptr = (ptr + 1) % DEBUG_STACK_SIZE;
        }
}

//write the contents of the debug stack out to disk
void flush_debug(void)
{
        FILE *out;

        out = fopen(DEBUG_FLUSH_FILE, "w");
        if (out == NULL) {
                fprintf(stderr, "Failed to open file to flush error, exiting...");
                exit(EXIT_FAILURE);
        }

        int i = 1;
        fprintf(out, "==== MOST RECENT ERROR ====\n");
        while (!stack_empty_debug()) {
                const char *err = pop_error_debug();
                fprintf(out, "%d: %s\n", i++, err);
        }
        fprintf(out, "==== LEAST RECENT ERROR ====\n");
}
