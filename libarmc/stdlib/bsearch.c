#include <stdlib.h>

void *bsearch(const void *key, const void *base, size_t high, size_t size, int (*compar)(const void *, const void *))
{
	char *p;
	size_t low;
	size_t mid;
	int r;

	if (size > 0) 
	{
		low = 0;
		while (low < high) 
		{
			mid = low + ((high - low) >> 1); // Avoid possible overflow here.
			p = ((char *)base) + mid * size; // Could overflow here...
			r = (*compar)(key, p); // but that's an application problem!
			if (r > 0) 
				low = mid + 1;
			else if (r < 0) 
				high = mid;
			else 
				return p;
		}
	}

	return NULL;
}
