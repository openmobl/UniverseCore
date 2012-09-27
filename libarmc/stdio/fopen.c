#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <implementation.h>



//***********************************************************
// Procedure: fopen
//
// Parameter: filename  Filename to open
//            type      Open mode
//
// Returnval: Pointer to file struct (NULL on error)
//
// Description: Open file.
//              Note: '+' in open mode currently not supported.
//***********************************************************
FILE *fopen(const char *filename, const char *type)
{
	UInt16 error;
	UInt16 openMode;
	bool cache = true;
	FILE *pFile;
	char realfilename[FILENAME_MAX];
	bool bAppend = false;
	bool bDelete = true;
	
	//---------------------------------------------
	// Get memory for file struct
	//---------------------------------------------
	pFile = (FILE *) malloc(sizeof(FILE));
	if (!pFile)
		return NULL;
	memset(pFile, 0, sizeof(FILE));

	//---------------------------------------------
  // Check open mode
	//---------------------------------------------
	if(strcmp(type, "r") == 0 || strcmp(type, "rb") == 0)
	{
  	//---------------------------------------------
  	// Open file for reading
  	//---------------------------------------------
		pFile->mode = MODE_BUFREAD;
		openMode = vfsModeRead;
		bDelete = false;
  }
  else if(strcmp(type, "w") == 0 || strcmp(type, "wb") == 0)
  {  
  	//---------------------------------------------
  	// Open file for write
  	//---------------------------------------------
		pFile->mode = MODE_BUFWRITE;
		openMode = vfsModeCreate | vfsModeWrite;
  }
  else if(strcmp(type, "a") == 0 || strcmp(type, "ab") == 0)
  {
  	//---------------------------------------------
  	// Open file for write (append)
  	//---------------------------------------------
		pFile->mode = MODE_BUFWRITE;
		openMode = vfsModeCreate | vfsModeWrite;
		bAppend = true;
		bDelete = false;
  }
  else if(strcmp(type, "r+") == 0 || strcmp(type, "rb+") == 0 || strcmp(type, "r+b") == 0)
  {
  	//---------------------------------------------
  	// Open for read/write
  	//---------------------------------------------
		cache = false;
		pFile->mode = MODE_BUFNONE;
		openMode = vfsModeReadWrite;
	  bDelete = false;
  }
  else if(strcmp(type, "w+") == 0 || strcmp(type, "wb+") == 0 || strcmp(type, "w+b") == 0)
  {
  	//---------------------------------------------
  	// Open for read/write (discard content)
  	//---------------------------------------------
		cache = false;
		pFile->mode = MODE_BUFNONE;
		openMode = vfsModeCreate | vfsModeReadWrite;
  }
  else if(strcmp(type, "a+") == 0 || strcmp(type, "ab+") == 0 || strcmp(type, "a+b") == 0)
  {
  	//---------------------------------------------
  	// Open for read/write (append)
  	//---------------------------------------------
		cache = false;
		pFile->mode = MODE_BUFNONE;
		openMode = vfsModeCreate | vfsModeReadWrite;
		bAppend = true;
	  bDelete = false;
  }
  else
  {
  	//---------------------------------------------
  	// Invalid mode -> clean up
  	//---------------------------------------------
    fclose(pFile);    
    return NULL;      
  }
  
	if (cache) 
	{
		//---------------------------------------------
		// Get memory for cache
		//---------------------------------------------
		pFile->cacheSize = CACHE_SIZE;
		pFile->cache = malloc(CACHE_SIZE);
		if (!pFile->cache)
		  pFile->cacheSize = 0;
	}

  //---------------------------------------------
  // Create complete filename with path
  //---------------------------------------------
  if(filename[0] == '/')
    strncpy(realfilename, filename, FILENAME_MAX);
  else
  {
    getcwd(realfilename, FILENAME_MAX);
    strncat(realfilename, "/", FILENAME_MAX);
    strncat(realfilename, filename, FILENAME_MAX);
  }

  //---------------------------------------------
  // Delete it if exists (in some modes)
  //---------------------------------------------
  if(bDelete)
	  error = VFSFileDelete(DefaultSDCard, realfilename);	
   
	if (openMode & vfsModeRead) 
	{
		//---------------------------------------------
		// Open file for read
		//---------------------------------------------
		error = VFSFileOpen(DefaultSDCard, realfilename, openMode, &(pFile->fileRef));
		if(error)
		{
		  // Failed to open -> clean up
		  fclose(pFile);
		  return NULL;
		}

		if(bAppend)
		  fseek(pFile, 0, SEEK_END);
    return pFile;
  }
  else
  {
		//---------------------------------------------
		// Open file for write
		//---------------------------------------------
		error = VFSFileCreate(DefaultSDCard, realfilename);
		openMode = vfsModeWrite;
		if (!error) 
		{
			error = VFSFileOpen(DefaultSDCard, realfilename, openMode, &(pFile->fileRef));
  		if(error)
  		{
  		  // Failed to open -> clean up
  		  fclose(pFile);
  		  return NULL;
  		}
  		if(bAppend)
  		  fseek(pFile, 0, SEEK_END);
      return pFile;
		}
  }

  //---------------------------------------------
  // Error opening file -> free resources
  //---------------------------------------------
  if(pFile->cache)
    free(pFile->cache);
  free(pFile);
  
  return NULL;
}


FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
  FILE *newFile;
  
  //---------------------------------------------
  // Close before reopen
  //---------------------------------------------
  if(stream)
    fclose(stream);

  //---------------------------------------------
  // Open filename
  //---------------------------------------------
  newFile = fopen(filename, mode);
  if(newFile && stream && (int) stream != _stdin && (int) stream != _stdout && (int) stream != _stderr)
    memcpy(stream, newFile, sizeof(FILE));
  
  return newFile;
}


