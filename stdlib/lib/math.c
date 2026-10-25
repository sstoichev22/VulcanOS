#include <math.h>

#define DOUBLE_SIGN_MASK 9223372036854775808UL
#define DOUBLE_ABSOLUTE_MASK 9223372036854775807UL
#define DOUBLE_EXPONENT_MASK 9218868437227405312UL
#define DOUBLE_FRACTION_MASK 4503599627370495UL
#define FLOAT_SIGN_MASK 2147483648U
#define FLOAT_ABSOLUTE_MASK 2147483647U

double fabs(double value) {
    union {
        double value;
        unsigned long bits;
    } representation;
    representation.value = value;
    representation.bits &= DOUBLE_ABSOLUTE_MASK;
    return representation.value;
}

float fabsf(float value) {
    union {
        float value;
        unsigned int bits;
    } representation;
    representation.value = value;
    representation.bits &= FLOAT_ABSOLUTE_MASK;
    return representation.value;
}

double copysign(double value, double sign) {
    union {
        double value;
        unsigned long bits;
    } magnitude, signSource;
    magnitude.value = value;
    signSource.value = sign;
    magnitude.bits = (magnitude.bits & DOUBLE_ABSOLUTE_MASK) | (signSource.bits & DOUBLE_SIGN_MASK);
    return magnitude.value;
}

float copysignf(float value, float sign) {
    union {
        float value;
        unsigned int bits;
    } magnitude, signSource;
    magnitude.value = value;
    signSource.value = sign;
    magnitude.bits = (magnitude.bits & FLOAT_ABSOLUTE_MASK) | (signSource.bits & FLOAT_SIGN_MASK);
    return magnitude.value;
}

int isnan(double value) {
    union {
        double value;
        unsigned long bits;
    } representation;
    representation.value = value;
    return (representation.bits & DOUBLE_EXPONENT_MASK) == DOUBLE_EXPONENT_MASK
            && (representation.bits & DOUBLE_FRACTION_MASK) != 0;
}

int isinf(double value) {
    union {
        double value;
        unsigned long bits;
    } representation;
    representation.value = value;
    return (representation.bits & DOUBLE_ABSOLUTE_MASK) == DOUBLE_EXPONENT_MASK;
}

int isfinite(double value) {
    union {
        double value;
        unsigned long bits;
    } representation;
    representation.value = value;
    return (representation.bits & DOUBLE_EXPONENT_MASK) != DOUBLE_EXPONENT_MASK;
}

int signbit(double value) {
    union {
        double value;
        unsigned long bits;
    } representation;
    representation.value = value;
    return (representation.bits & DOUBLE_SIGN_MASK) != 0;
}

double fmin(double a, double b) {
    if (isnan(a)) return b;
    if (isnan(b)) return a;
    if (a == b) return signbit(a) ? a : b;
    return a < b ? a : b;
}

double fmax(double a, double b) {
    if (isnan(a)) return b;
    if (isnan(b)) return a;
    if (a == b) return signbit(a) ? b : a;
    return a > b ? a : b;
}

float fminf(float a, float b) {
    return (float)fmin(a, b);
}

float fmaxf(float a, float b) {
    return (float)fmax(a, b);
}
