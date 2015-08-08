#include <stdint.h>
#include "srofs.h"

#include <glcd_text_line.h>

void print_ln(char *s, uint8_t r) {
    char b[3];
    i8toh(r, b);
    glcd_text_line_print_ln(s);
    glcd_text_line_print_ln(b);
}

#define srofs_spec_printf print_ln

#define SROFS_SPEC_IGNORE_0
#define SROFS_SPEC_IGNORE_1
#define SROFS_SPEC_IGNORE_2
#define SROFS_SPEC_IGNORE_3
#define SROFS_SPEC_IGNORE_4
#define SROFS_SPEC_IGNORE_5
#define SROFS_SPEC_IGNORE_6
#define SROFS_SPEC_IGNORE_7
#define SROFS_SPEC_IGNORE_8
#define SROFS_SPEC_IGNORE_9
#define SROFS_SPEC_IGNORE_10
#define SROFS_SPEC_IGNORE_11
#define SROFS_SPEC_IGNORE_12
#define SROFS_SPEC_IGNORE_13
#define SROFS_SPEC_IGNORE_14
#define SROFS_SPEC_IGNORE_15
#define SROFS_SPEC_IGNORE_16
#define SROFS_SPEC_IGNORE_17
#define SROFS_SPEC_IGNORE_18
#define SROFS_SPEC_IGNORE_19
#define SROFS_SPEC_IGNORE_20
#define SROFS_SPEC_IGNORE_21
#define SROFS_SPEC_IGNORE_22

#include "srofs_spec.h"
