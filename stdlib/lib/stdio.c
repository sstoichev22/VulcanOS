#include <stdio.h>

#ifndef VCC_PRINTF_INTEGER
int puts(const char *text) {
    int written = 0;
    while (*text) {
        putchar(*text++);
        written++;
    }
    putchar('\n');
    return written + 1;
}

static int print_float(double value, int precision) {
    union {
        double floating;
        unsigned long bits;
    } representation;
    representation.floating = value;

    unsigned long exponent = (representation.bits >> 52) & 2047UL;
    int written = 0;
    if (representation.bits >> 63) {
        putchar('-');
        written++;
        value = -value;
    }

    if (exponent == 2047UL) {
        if (representation.bits & 4503599627370495UL) {
            putchar('n');
            putchar('a');
            putchar('n');
        } else {
            putchar('i');
            putchar('n');
            putchar('f');
        }
        return written + 3;
    }

    int exponent10 = 0;
    while (value >= 1.0e18) {
        value /= 10.0;
        exponent10++;
    }

    double rounding = 0.5;
    for (int index = 0; index < precision; index++) rounding /= 10.0;
    value += rounding;

    unsigned long integral = (unsigned long)value;
    double fraction = value - (double)integral;
    char digits[24];
    int digitCount = 0;
    do {
        digits[digitCount++] = (int)(integral % 10UL) + '0';
        integral /= 10UL;
    } while (integral);

    while (digitCount) {
        putchar(digits[--digitCount]);
        written++;
    }

    if (precision) {
        putchar('.');
        written++;
    }
    for (int index = 0; index < precision; index++) {
        fraction *= 10.0;
        int digit = (int)fraction;
        putchar(digit + '0');
        written++;
        fraction -= (double)digit;
    }

    if (exponent10) {
        putchar('e');
        putchar('+');
        written += 2;
        digitCount = 0;
        do {
            digits[digitCount++] = exponent10 % 10 + '0';
            exponent10 /= 10;
        } while (exponent10);
        while (digitCount) {
            putchar(digits[--digitCount]);
            written++;
        }
    }
    return written;
}
#endif

static int print_integer(unsigned long value, int radix, int uppercase) {
    char digits[24];
    int digitCount = 0;
    do {
        int digit;
        if (radix == 16) {
            digit = (int)(value & 15UL);
            value >>= 4;
        } else if (radix == 8) {
            digit = (int)(value & 7UL);
            value >>= 3;
        } else {
            unsigned long quotient = value / 10UL;
            digit = (int)(value - quotient * 10UL);
            value = quotient;
        }
        digits[digitCount++] = digit < 10 ? digit + '0' : digit - 10 + (uppercase ? 'A' : 'a');
    } while (value);

    int written = digitCount;
    while (digitCount) putchar(digits[--digitCount]);
    return written;
}

int printf(const char *format, ...) {
    unsigned long *arguments = (unsigned long *)(&format + 1);
    int written = 0;

    while (*format) {
        char conversion = *format++;
        if (conversion != '%') {
            putchar(conversion);
            written++;
            continue;
        }

        conversion = *format++;
        if (conversion == '%') {
            putchar('%');
            written++;
            continue;
        }

        int precision = 6;
        if (conversion == '.') {
            precision = 0;
            conversion = *format++;
            while (conversion >= '0' && conversion <= '9') {
                if (precision < 9) {
                    precision = precision * 10 + conversion - '0';
                    if (precision > 9) precision = 9;
                }
                conversion = *format++;
            }
        }

        int wide = 0;
        if (conversion == 'l') {
            wide = 1;
            conversion = *format++;
            if (conversion == 'l') conversion = *format++;
        } else if (conversion == 'h') {
            conversion = *format++;
            if (conversion == 'h') conversion = *format++;
        } else if (conversion == 'z') {
            wide = 1;
            conversion = *format++;
        }

#ifndef VCC_PRINTF_INTEGER
        if (conversion == 'f' || conversion == 'F') {
            double value = *(double *)arguments++;
            written += print_float(value, precision);
            continue;
        }
#endif

        if (conversion == 'c') {
            putchar((int)*arguments++);
            written++;
            continue;
        }
        if (conversion == 's') {
            const char *text = (const char *)*arguments++;
            while (*text) {
                putchar(*text++);
                written++;
            }
            continue;
        }

        if (conversion == 'd' || conversion == 'i' || conversion == 'u'
                || conversion == 'x' || conversion == 'X' || conversion == 'o' || conversion == 'p') {
            unsigned long number = *arguments++;
            if (conversion == 'd' || conversion == 'i') {
                if (wide) {
                    if ((long)number < 0) {
                        putchar('-');
                        written++;
                        number = -number;
                    }
                } else if ((int)number < 0) {
                    putchar('-');
                    written++;
                    number = (unsigned int)-(int)number;
                }
            }

            if (conversion == 'p') wide = 1;
            int radix = conversion == 'x' || conversion == 'X' || conversion == 'p' ? 16
                    : conversion == 'o' ? 8 : 10;
            written += print_integer(number, radix, conversion == 'X');
            continue;
        }

        putchar(conversion);
        written++;
    }
    return written;
}
