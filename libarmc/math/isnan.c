
#include "math.h"
#include "math_private.h"


#define      dExpMask           0x7FF00000
#define      dHighMan           0x000FFFFF


static int __fpclassify(double arg)
{
	unsigned int exponent;
  union
  {
    dHexParts hex;
    double dbl;
  } x;

	x.dbl = arg;

	exponent = x.hex.high & dExpMask;
	if ( exponent == dExpMask )
	{
		if ( ( ( x.hex.high & dHighMan ) | x.hex.low ) == 0 )
			return FP_INFINITE;
		else
    	return FP_NAN;
	}
	else if ( exponent != 0)
		return FP_NORMAL;
	else 
	{
		if ( arg == 0.0 )
			return FP_ZERO;
		else
			return FP_SUBNORMAL;
	}
}


int isnan(double x)
{
	int class = __fpclassify(x);
	return ( class == FP_NAN );
}
