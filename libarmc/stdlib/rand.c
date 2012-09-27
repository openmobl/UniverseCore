
struct random_data
{
  long *fptr;		  /* Front pointer.  */
  long *rptr;		  /* Rear pointer.  */
  long *state;		/* Array of state values.  */
  int rand_deg;		/* Degree of random number generator.  */
  int rand_sep;		/* Distance between front and rear.  */
  long *end_ptr;		/* Pointer behind state table.  */
};

static long _randtbl[] =
{
    3,
    -1726662223, 379960547, 1735697613, 1040273694, 1313901226,
    1627687941, -179304937, -2073333483, 1780058412, -1989503057,
    -615974602, 344556628, 939512070, -1249116260, 1507946756,
    -812545463, 154635395, 1388815473, -1926676823, 525320961,
    -1009028674, 968117788, -123449607, 1284210865, 435012392,
    -2017506339, -911064859, -370259173, 1132637927, 1398500161,
    -205601318
};

static struct random_data _rand_state = { 0, 0, 0, 0, 0, 0};


int rand(void)
{
	long *fptr = _rand_state.fptr;
  long *rptr = _rand_state.rptr;
  long *end_ptr = _rand_state.end_ptr;
	long val;
  
  if(fptr == 0)
    return 0;
    
	val = *fptr += *rptr;
	/* Chucking least random bit.  */
  val = (val >> 1) & 0x7fffffff;
  ++fptr;
  if (fptr >= end_ptr)
  {
    fptr = _rand_state.state;
    ++rptr;
  }
  else
  {
    ++rptr;
    if (rptr >= end_ptr)
	    rptr = _rand_state.state;
	}
  _rand_state.fptr = fptr;
  _rand_state.rptr = rptr;

  return val;
}


void srand(unsigned int seed)
{
  long *state;
  long i;
  long word;
  long *dst;
  int kc;

  _rand_state.rand_sep = 3;
  _rand_state.rand_deg = 31;
  state = &_randtbl[1];  
  _rand_state.end_ptr = &state[_rand_state.rand_deg];
  _rand_state.state = state;

  if (seed == 0)
  	seed = 1;
  state[0] = seed;

  dst = state;
  word = seed;
  kc = _rand_state.rand_deg;
  for (i = 1; i < kc; ++i)
  {
	/* This does:
	   state[i] = (16807 * state[i - 1]) % 2147483647;  but avoids overflowing 31 bits.  */
  	long hi = word / 127773;
  	long lo = word % 127773;
  	word = 16807 * lo - 2836 * hi;
	  if (word < 0)
	    word += 2147483647;
  	*++dst = word;
  }

  _rand_state.fptr = &state[_rand_state.rand_sep];
  _rand_state.rptr = &state[0];
  kc *= 10;
  while (--kc >= 0)
  {
  	rand();
  }
}


