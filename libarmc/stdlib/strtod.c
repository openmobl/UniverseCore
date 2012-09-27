/*
 * Copyright (C) 2000-2005     Manuel Novoa III
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

/* Notes:
 *
 * The primary objective of this implementation was minimal size and
 * portablility, while providing robustness and resonable accuracy.
 *
 * This implementation depends on IEEE floating point behavior and expects
 * to be able to generate +/- infinity as a result.
 *
 * There are a number of compile-time options below.
 */

/* July 27, 2003
 *
 * General cleanup and some minor size optimizations.
 * Change implementation to support __strtofpmax() rather than strtod().
 *   Now all the strto{floating pt}() funcs are implemented in terms of
 *   of the internal __strtofpmax() function.
 * Support "nan", "inf", and "infinity" strings (case-insensitive).
 * Support hexadecimal floating point notation.
 * Support wchar variants.
 * Support xlocale variants.
 *
 * TODO:
 *
 * Consider accumulating blocks of digits in longs to save floating pt mults.
 *   This would likely be much better on anything that only supported floats
 *   where DECIMAL_DIG == 9.  Actually, if floats have FLT_MAX_10_EXP == 38,
 *   we could calculate almost all the exponent multipliers (p_base) in
 *   long arithmetic as well.
 */

/**********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>


#define DECIMAL_DIG 18
#ifndef DBL_MIN_10_EXP
#define DBL_MIN_10_EXP (-307)
#endif
#define EXP_DENORM_ADJUST DECIMAL_DIG
#define MAX_ALLOWED_EXP (DECIMAL_DIG  + EXP_DENORM_ADJUST - DBL_MIN_10_EXP)


double strtod(const char *str, char **endptr)
{
  int exponent_power = 0;
	double number;
	double p_base = 10;			/* Adjusted to 16 in the hex case. */
	char *pos0;
	char *pos1;
	char *pos = (char *) str;
	int exponent_temp;
	int negative; /* A flag for the number, a multiplier for the exponent. */
	int num_digits;

  // Skip leading whitespace.
	while(isspace(*pos)) 
		++pos;

	negative = 0;
  // Handle optional sign.
	switch(*pos) 
	{
		case '-': 
		  negative = 1;	// Fall through to increment position.
		case '+': 
		  ++pos;
	}

	number = 0.;
	num_digits = -1;
  //	exponent_power = 0;
	pos0 = NULL;

LOOP:
	while(isdigit(*pos)) 
	{	// Process string of digits.
		if (num_digits < 0) 
		{	// First time through?
			++num_digits;		// We've now seen a digit.
		}
		if (num_digits || (*pos != '0')) 
		{ // Had/have nonzero.
			++num_digits;
			if (num_digits <= DECIMAL_DIG) 
			{ // Is digit significant?
				number = number * p_base + (*pos - '0');
			}
		}
		++pos;
	}

	if ((*pos == '.') && !pos0) 
	{ // First decimal point?
		pos0 = ++pos;			// Save position of decimal point
		goto LOOP;				//   and process rest of digits.
	}

	if (num_digits<0) 
	{			// Must have at least one digit.
		if (!pos0) 
		{			// No decimal point, so check for inf/nan.
			// Note: nan is the first string so 'number = i/0.;' works.
			static const char nan_inf_str[] = "\05nan\0\012infinity\0\05inf\0";
			int i = 0;

			do 
			{
				// Unfortunately, we have no memcasecmp().
				int j = 0;
				while (tolower(pos[j]) == nan_inf_str[i+1+j]) 
				{
					++j;
					if (!nan_inf_str[i+1+j]) 
					{
						number = i / 0.;
						if (negative) 
						{	// Correct for sign.
							number = -number;
						}
						pos += nan_inf_str[i] - 2;

          	if (endptr)
          		*endptr = pos;
          	return number;
					}
				}
				i += nan_inf_str[i];
			} while (nan_inf_str[i]);
		}

  	if (endptr)
  		*endptr = (char *) str;
  	return number;
	}

	if (num_digits > DECIMAL_DIG) 
	{ // Adjust exponent for skipped digits.
		exponent_power += num_digits - DECIMAL_DIG;
	}

	if (pos0) 
	{
		exponent_power += pos0 - pos; // Adjust exponent for decimal point.
	}

	if (negative) 
	{				// Correct for sign.
		number = -number;
	}

	/* process an exponent string */
	if (((*pos) | 0x20) == 'e') 
	{
		pos1 = pos;
		negative = 1;
		switch(*++pos) 
		{		// Handle optional sign.
			case '-': 
			  negative = -1; // Fall through to increment pos.
			case '+': 
			  ++pos;
		}

		pos0 = pos;
		exponent_temp = 0;
		while (isdigit(*pos)) 
		{	// Process string of digits.
			if(exponent_temp < MAX_ALLOWED_EXP) 
			{ // Avoid overflow.
				exponent_temp = exponent_temp * 10 + (*pos - '0');
			}
			++pos;
		}

		if (pos == pos0) 
		{	// No digits?
			pos = pos1;		// Back up to {e|E}/{p|P}.
		}

		exponent_power += negative * exponent_temp;
	}

	if (number == 0.) 
  {
	  if (endptr)
		  *endptr = pos;
	  return number;
  }

	// scale the result
	exponent_temp = exponent_power;
	if (exponent_temp < 0) 
		exponent_temp = -exponent_temp;

	while (exponent_temp) 
	{
		if (exponent_temp & 1) 
		{
			if (exponent_power < 0) 
			{
				// Warning... caluclating a factor for the exponent and
				// then dividing could easily be faster.  But doing so
				// might cause problems when dealing with denormals.
				number /= p_base;
			} 
			else
				number *= p_base;
		}
		exponent_temp >>= 1;
		p_base *= p_base;
	}

	if (endptr)
		*endptr = pos;
	return number;
}


double atof(const char *str)
{
  return strtod(str, 0);
}

