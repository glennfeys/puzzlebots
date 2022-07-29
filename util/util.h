#ifndef SYSPROG_UTIL_H
#define SYSPROG_UTIL_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

void fatal(const char *fmt, ...);

//same as fatal, but only "fatal" of cond is true. Otherwise, does nothing.
void fatal_if(int cond, const char *fmt, ...);

//same as fatal, but returns an int (off course, fatal never returns, this is useful if you need to call it with a non void return)
int fatal_int(const char *fmt, ...);

#ifndef M_PI
//M_PI is only define in GNU99, not in C99
#define M_PI 3.14159265358979323846 /*pi with maximum digits that fit in double */
#endif

#define M_PI_F ((float)M_PI)

#define DEGREES_TO_RADIANS(degrees)((M_PI * (degrees))/180.0f)
#define DEGREES_TO_RADIANS_F(degrees)((M_PI_F * (degrees))/180.0f)

#ifndef MAX
#define MAX(a, b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a, b) ((a)<(b)?(a):(b))
#endif

#ifdef __cplusplus
}
#endif

#endif //SYSPROG_UTIL_H
