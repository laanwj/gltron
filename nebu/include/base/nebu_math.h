#ifndef NEBU_MATH_H
#define NEBU_MATH_H

#include <math.h>
#include <limits.h>

#ifndef M_PI
#define M_PI 3.141592654
#endif

#define PI ((float) M_PI)

#ifndef floorf
#define floorf (float)floor
#endif
#ifndef fabsf
#define fabsf (float)fabs
#endif
#ifndef sqrtf
#define sqrtf (float)sqrt
#endif
#ifndef cosf
#define cosf (float) cos
#endif
#ifndef sinf
#define sinf (float) sin
#endif
#ifndef acosf
#define acosf (float) acos
#endif
#ifndef tanf
#define tanf (float) tan
#endif


#define nebu_cosf_deg(X)	cosf( (X) * M_PI/180.0 )
#define nebu_sinf_deg(X)	sinf( (X) * M_PI/180.0 )

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F
#endif

#ifndef FLT_MIN
#define FLT_MIN 1.175494351e-38F
#endif

#endif
