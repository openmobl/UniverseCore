#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>


//***********************************************************
// Procedure: fgetc
//
// Parameter: stream    Reference to file
//
// Returnval: Read byte or EOF
//
// Description: Get one byte from file file.
//***********************************************************
int fgetc(FILE *stream) 
{
	UInt32 lBytesRead;
	Char c;

	lBytesRead = fread(&c, 1, 1, stream);
	return (int) (lBytesRead == 1 ? c : EOF);
}



