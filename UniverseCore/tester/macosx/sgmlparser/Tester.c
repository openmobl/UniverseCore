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
 
#include "libsgml.h"
#include "WBXML.h"
#include "WMLC.h"

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

static void ParseMLFromFile(char *filename, int outputTerminal, int domTree, int safe)
{
    char            *input  = NULL;
    char            *output = NULL;
    char            *fileout = NULL;
    char            *domoutName = NULL;
    FILE            *in      = NULL;
    FILE            *out     = NULL;
    FILE            *domOut  = NULL;
    unsigned long   size    = fsize(filename);
    
	unsigned long   flags   = 0;
    unsigned long   outlen  = 0;
	SGML_PARSER     sgml;
 
    fileout = malloc(1024);
    if (!fileout) {
        return;
    }
    if (domTree) {
        domoutName = malloc(1024);
        if (!domoutName) {
            free(fileout);
            return;
        }
    }
 
    WriteProgress(1);
          
    in = fopen(filename, "r");
    if (!in)
        return;
    
    WriteProgress(2);
    
    input = malloc(size + 1);
    if (!input)
        goto Parse_cleanup;
    
    fread(input, size, 1, in);
    if (!input)
        goto Parse_cleanup;
    
    WriteProgress(3);
        
    strcpy(fileout, filename);
    strcpy(fileout + strlen(fileout), "_output.txt");
    
    if (domTree) {
        strcpy(domoutName, filename);
        strcpy(domoutName + strlen(domoutName), "_dom.txt");
    }
    
    WriteProgress(4);
    
    out = fopen(fileout, "w");
    if (!out)
        goto Parse_cleanup;
    
    if (domTree) {
        domOut = fopen(domoutName, "w");
        if (!domOut)
            goto Parse_cleanup;
    }
    
    WriteProgress(5);
    
    sgmlParserInitialize(&sgml, SGML_EXTENSION_TYPE_HTML, NULL, NULL);
    WriteProgress(6);
    sgmlParserExtensionSetParam(&sgml, SGML_EXTENSION_HTML_PARAM_FLAG, (void *)flags);
    WriteProgress(7);
    sgmlParserParseString(&sgml, input, size);

    
    WriteProgress(33);
    sgmlExtensionHtmlCleanup(sgmlExtensionHtmlGetDocument(&sgml), HTML_MODE_QUIRKS);
    WriteProgress(8);
    outlen = domNodeSerializeToString(sgmlExtensionHtmlGetDocument(&sgml), &output);
    
    WriteProgress(9);
    
    if (outlen) {
        WriteProgress(10);
        fwrite(output, outlen, 1, out);
        
        if (outputTerminal) {
            printf("\nRepresentation:\n");
            printf(output);
            printf("\n");
        }
    }
    
    WriteProgress(11);
    
    
    if (domTree) {
        free(output);
        output = NULL;
        outlen = domNodeSerializeTreeToString(sgmlExtensionHtmlGetDocument(&sgml), &output, 0, safe);
        
        WriteProgress(12);
        
        if (outlen) {
            WriteProgress(13);
            fwrite(output, outlen, 1, domOut);
            
            if (outputTerminal) {
                printf("\n\nDOM:\n");
                printf(output);
                printf("\n");
            }
        }
        
        WriteProgress(14);

        free(output);
        output = NULL;
        outlen = domNodeSerializeTreeToString(sgmlExtensionHtmlGetDocument(&sgml), &output, 1, safe);
        
        WriteProgress(15);
        
        if (outlen) {
            WriteProgress(16);
            fwrite(output, outlen, 1, domOut);
            
            if (outputTerminal) {
                printf("\n\nDOM (with data):\n");
                printf(output);
                printf("\n");
            }
        }
    }

    WriteProgress(17);
    
    sgmlParserDestroy(&sgml, 0);
    
    WriteProgress(18);
    
    
Parse_cleanup:
    WriteProgress(19);
    
    if (in) fclose(in);
    if (out) fclose(out);
    if (domOut && domTree) fclose(domOut);
    
    WriteProgress(20);
    
    if (input)
        free(input);
    WriteProgress(21);
    if (output)
        free(output);
    
    if (fileout)
        free(fileout);
    if (domoutName && domTree)
        free(domoutName);
    
    WriteProgress(22);
}

int main(int argc, char *args[])
{
    char    *input = NULL;
    int     outputTerminal = 0;
    int     domTree = 0;
    int     safe = 0;
    
    if (argc < 2) {
        //printf("%s: %s\n", args[0], "Error! No file input.");
        //exit(0);
        input = malloc(1024 + 1);
        if (!input) {
            fprintf(stderr, "%s: %s\n", args[0], "Error! No file input.");
            exit(0);
        }
        
        printf("Please enter a file to test: ");
        fscanf(stdin, "%s", input);
    } else {
        input = strdup(args[1]);
    }
    
    printf("%s: %s\n", args[0], input);
    
    printf("Please enter 1 for true, 0 for false. Format: outputTerminal domTree safe. : ");
    scanf("%d %d %d", &outputTerminal, &domTree, &safe);
    
    ParseMLFromFile(input, outputTerminal, domTree, safe);
    
    free(input);
    
    return 0;
}

