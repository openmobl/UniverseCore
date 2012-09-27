/********************************************************************************
 * UniverseCore Web Core                                                        *
 * Copyright (c) 2007 OpenMobl Systems                                          *
 * Copyright (c) 2006-2007 Donald C. Kirker                                     *
 *                                                                              *
 * http://www.openmobl.com/                                                     *
 * dev-support@openmobl.com                                                     *
 *                                                                              *
 * This library is free software; you can redistribute it and/or                *
 * modify it under the terms of the GNU Lesser General Public                   *
 * License as published by the Free Software Foundation; either                 *
 * version 2.1 of the License, or (at your option) any later version.           *
 *                                                                              *
 * This library is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU            *
 * Lesser General Public License for more details.                              *
 *                                                                              *
 * You should have received a copy of the GNU Lesser General Public             *
 * License along with this library; if not, write to the Free Software          *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA*
 *                                                                              *
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "International.h"

static void WriteProgress(int pos)
{
    fprintf(stdout, "%05d\n", pos);
}

long fsize(char *name)
{
    long    n = 0;
    FILE    *f = NULL;
    
    f = fopen(name, "r");
    if (!f)
        return 0;
    
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fclose(f);
    
    return n;
}

int main (int argc, const char * argv[])
{
    char    dest[1024];     // destination file name
    char    destDC[1024];   // "double convert" destination
    char    *input          = NULL;
    char    *srcEnc         = NULL;
    char    *destEnc        = NULL;
    FILE    *in             = NULL;
    FILE    *out            = NULL;
    FILE    *outDC          = NULL;
    unsigned char *inStr    = NULL;
    unsigned char *outStr   = NULL;
    unsigned char *outDCStr = NULL;
    unsigned long inLen     = 0;
    unsigned long outLen    = 0;
    unsigned long outDCLen  = 0;
    
    if (argc < 2) {
        input = malloc(1024 + 1);
        if (!input) {
            fprintf(stderr, "%s: %s\n", argv[0], "Error! No file input.");
            exit(0);
        }
        
        printf("Please enter a file to test: ");
        fscanf(stdin, "%s", input);
    } else {
        input = strdup(argv[1]);
    }
    
    if (argc < 3) {
        srcEnc = malloc(1024 + 1);
        if (!srcEnc) {
            fprintf(stderr, "%s: %s\n", argv[0], "Error! No encoding.");
            exit(0);
        }
        
        printf("Please enter the input encoding: ");
        fscanf(stdin, "%s", srcEnc);
    } else {
        srcEnc = strdup(argv[2]);
    }
    
    if (argc < 4) {
        destEnc = malloc(1024 + 1);
        if (!destEnc) {
            fprintf(stderr, "%s: %s\n", argv[0], "Error! No encoding.");
            exit(0);
        }
        
        printf("Please enter the output encoding: ");
        fscanf(stdin, "%s", destEnc);
    } else {
        destEnc = strdup(argv[3]);
    }
    
    WriteProgress(1);
    
    memset(dest, 0, 1024);
    strcpy(dest, input);
    strcpy(dest + strlen(input), "_out");
    printf(dest);
    printf("\n");
    memset(destDC, 0, 1024);
    strcpy(destDC, input);
    strcpy(destDC + strlen(input), "_doubled");
    printf(destDC);
    printf("\n");
    
    WriteProgress(2);
    
    inLen = fsize(input);
    
    WriteProgress(3);
    
    in = fopen(input, "r");
    if (!in)
        goto cleanup;
    WriteProgress(4);
    out = fopen(dest, "w");
    if (!out)
        goto cleanup;
    WriteProgress(5);
    outDC = fopen(destDC, "w");
    if (!outDC)
        goto cleanup;
    WriteProgress(6);
    
    inStr = malloc(inLen);
    if (!inStr)
        goto cleanup;
    
    fread(inStr, inLen, 1, in);
    
    WriteProgress(7);
    
    // Convert from source encoding to dest
    outLen = intlConvertStrings(inStr, srcEnc, inLen, &outStr, destEnc);
    
    if (outLen) {
        WriteProgress(8);
        fwrite(outStr, outLen, 1, out);
        WriteProgress(9);
    } else {
        WriteProgress(10);
        goto cleanup;
    }
    
    WriteProgress(11);
    
    // Convert from dest back to source
    outDCLen = intlConvertStrings(outStr, destEnc, outLen, &outDCStr, srcEnc);

    if (outLen) {
        WriteProgress(12);
        fwrite(outDCStr, outDCLen, 1, outDC);
        WriteProgress(13);
    } else {
        WriteProgress(14);
        goto cleanup;
    }
    
    WriteProgress(15);
    
cleanup:
    WriteProgress(16);
    
    free(input);
    free(srcEnc);
    free(destEnc);
    
    WriteProgress(17);
    
    if (inStr)
        free(inStr);
    if (outStr)
        free(outStr);
    if (outDCStr)
        free(outDCStr);
    
    WriteProgress(18);
    
    if (in)
        fclose(in);
    if (out)
        fclose(out);
    if (outDC)
        fclose(outDC);
    
    WriteProgress(19);
    
    return 0;
}
