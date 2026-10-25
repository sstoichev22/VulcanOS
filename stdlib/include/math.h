#ifndef VULCAN_MATH_H
#define VULCAN_MATH_H

#define INFINITY (1.0F / 0.0F)
#define NAN (0.0F / 0.0F)
#define HUGE_VAL (1.0 / 0.0)

/** Clear the sign bit, preserving the magnitude and NaN payload. */
double fabs(double value);
float fabsf(float value);

/** Return the magnitude of value with the sign bit taken from sign. */
double copysign(double value, double sign);
float copysignf(float value, float sign);

/** With one NaN, return the other operand; fmin selects -0 and fmax selects +0 on signed-zero ties. */
double fmin(double a, double b);
double fmax(double a, double b);
float fminf(float a, float b);
float fmaxf(float a, float b);

/** Each returns nonzero when its named classification holds; signbit reads the IEEE sign bit. */
int isnan(double value);
int isinf(double value);
int isfinite(double value);
int signbit(double value);

#endif
