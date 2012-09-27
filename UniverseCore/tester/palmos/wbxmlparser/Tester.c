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
#include <PalmOS.h>
#include <PalmOSGlue.h>
#include "TesterRes.h"
#include "libsgml.h"
#include "WBXML.h"
#include "WMLC.h"
#include <VFSMgr.h>

void *GetObjectPtr (UInt16 objectID)
{
    FormPtr         frm = FrmGetActiveForm ();
    return(FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, objectID)));
}

static void WriteProgress(UInt16 pos)
{
    Char    temp[50];
    
    StrPrintF(temp, "%05d", pos);
    
    WinDrawChars(temp, StrLen(temp), 120, 0);
    
    //SysTaskDelay(SysTicksPerSecond());
}

static FileRef FileOpenFromName(Char *filename, Boolean create)
{
	UInt16 		volRefFirst = 0;
	UInt16 		volRefNum = 0; 
	UInt32 		volIterator = vfsIteratorStart;
	FileRef 	fileRefP;
	Err         err;
	UInt32 		vfsMgrVersion;

	// check for a VFS palm
	err = FtrGet(sysFileCVFSMgr, vfsFtrIDVersion, &vfsMgrVersion); 
	if(err) {
		return NULL;
	}

 
	while (volIterator != vfsIteratorStop) { 
		err = VFSVolumeEnumerate(&volRefNum, &volIterator); 
		if (err == errNone) { 
			// Do something with the volRefNum
			volRefFirst = volRefNum;
		} else { 
			// handle error...?
			return NULL;
		} 
	}
	volRefNum = volRefFirst;

    if (create) {
create_top:
        err = VFSFileCreate(volRefNum, filename);
        if (err == vfsErrFileAlreadyExists) {
            err = VFSFileDelete(volRefNum, filename);
            if (err != errNone) {
                return NULL;
            }
            
            goto create_top;
        } else if (err != errNone) {
            return NULL;
        }
    }

	err = VFSFileOpen(volRefNum, filename, vfsModeReadWrite, &fileRefP);
	if (err != errNone) {
		return NULL;
	}
    
    return fileRefP;
}

static Char *FileReadFromRef(FileRef fileRefP)
{
    Char    *output         = NULL;
    UInt32  length          = 0;
	UInt32	bytesRead       = 0;
	UInt32  totalBytesRead 	= 0;
    Err     err             = errNone;
    
    VFSFileSize(fileRefP, &length);
    
    output = MemGluePtrNew(length + 1);
    if (!output) {
        return NULL;
    }
    
    do {
        err = VFSFileRead(fileRefP, length - totalBytesRead,
					output + totalBytesRead, &bytesRead);
        totalBytesRead += bytesRead;
    } while ((totalBytesRead < length) && (err == errNone) && bytesRead);
    
    return output;
}

static UInt32 FileGetSizeFromRef(FileRef fileRefP)
{
    UInt32  size = 0;
    
    VFSFileSize(fileRefP, &size);
    
    return size;
}

static void FileWriteToRef(FileRef fileRefP, Char *data, UInt32 size)
{
	UInt32 		numBytesWrittenP    = 0;
	UInt32 		bufferoffset        = 0;
    Err         err                 = errNone;
    
	do {
        err = VFSFileWrite(fileRefP, size - bufferoffset, data + bufferoffset,
    					&numBytesWrittenP);
        if (err != errNone) {
			return;
		}

        bufferoffset += numBytesWrittenP;
    } while (bufferoffset < size);
}

static void FileCloseFromRef(FileRef fileRefP)
{
    if (fileRefP != NULL) {
        VFSFileClose(fileRefP);
    }
}

static void ParseWBXMLFromFile(Char *filename)
{
    Char            *input  = NULL;
    Char            *output = NULL;
    Char            *fileout = NULL;
    Char            *domoutName = NULL;
    FileRef         in      = NULL;
    FileRef         out     = NULL;
    FileRef         domOut  = NULL;
    Int32           startTick = 0;
    Int32           stopTick = 0;
    
    unsigned long   outlen  = 0;
	WBXMLParserPtr  parser;
 
    fileout = MemPtrNew(1024);
    if (!fileout) {
        return;
    }
    domoutName = MemPtrNew(1024);
    if (!domoutName) {
        MemPtrFree(fileout);
        return;
    }
 
    WriteProgress(1);
          
    in = FileOpenFromName(filename, false);
    if (!in)
        return;
    
    WriteProgress(2);
    
    input = FileReadFromRef(in);
    if (!input)
        goto Parse_cleanup;
    
    WriteProgress(3);
        
    StrCopy(fileout, filename);
    StrCopy(fileout + StrLen(fileout), "_output.txt");
    
    StrCopy(domoutName, filename);
    StrCopy(domoutName + StrLen(domoutName), "_dom.txt");
    
    WriteProgress(4);
    
    out = FileOpenFromName(fileout, true);
    if (!out)
        goto Parse_cleanup;
        
    domOut = FileOpenFromName(domoutName, true);
    if (!domOut)
        goto Parse_cleanup;
    
    WriteProgress(5);
    
    startTick = TimGetTicks();
    parser = wbxmlNewParser();
    WriteProgress(6);
    wbxmlExtensionWMLCNew(parser);
    WriteProgress(7);
    wbxmlParse(input, FileGetSizeFromRef(in), 0, parser);

    stopTick = TimGetTicks();
    WriteProgress(8);
    outlen = domNodeSerializeToString(wbxmlExtensionWMLCGetDocument(parser), &output);
    
    WriteProgress(9);
    
    if (outlen) {
        WriteProgress(10);
        FileWriteToRef(out, output, outlen);
    }
    
    WriteProgress(11);
    
    FileWriteToRef(out, "\r\n", 2);
    
    WriteProgress(12);
    
    MemPtrFree(output);
    output = NULL;
    outlen = domNodeSerializeTreeToString(wbxmlExtensionWMLCGetDocument(parser), &output, CtlGetValue(GetObjectPtr(checkMore)), true);
    
    WriteProgress(13);
    
    if (outlen) {
        WriteProgress(14);
        FileWriteToRef(domOut, output, outlen);
    }
    
    WriteProgress(15);
    
    FileWriteToRef(domOut, "\r\n", 2);
    
    WriteProgress(16);
    
    wbxmlParserDestroy(&parser);
    
    WriteProgress(17);
    
    {
        Char    temp[75];
        
        StrPrintF(temp, "time (on m68k): ticks: %ld seconds: %ld", stopTick - startTick, (stopTick - startTick) / SysTicksPerSecond());
        WinDrawChars(temp, StrLen(temp), 3, 110);
    }
    
Parse_cleanup:
    WriteProgress(18);
    
    FileCloseFromRef(in);
    FileCloseFromRef(out);
    FileCloseFromRef(domOut);
    
    WriteProgress(19);
    
    if (input)
        MemPtrFree(input);
    WriteProgress(20);
    if (output)
        MemPtrFree(output);
    
    if (fileout)
        MemPtrFree(fileout);
    if (domoutName)
        MemPtrFree(domoutName);
    
    WriteProgress(21);
}


static Boolean MainFormHandleEvent(EventType * eventP)
{
    Boolean     handled     = false;
    Char        *filename   = NULL;

    switch (eventP->eType) 
    {
        case frmOpenEvent:
            FrmDrawForm(FrmGetActiveForm());
            break;
        case ctlSelectEvent:
            switch (eventP->data.ctlSelect.controlID) {
                case buttonTest:
                    filename = FldGetTextPtr(GetObjectPtr(fldFile));

                    ParseWBXMLFromFile(filename);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
    return handled;
}

static Boolean ApplicationHandleEvent(EventPtr event)
{
    FormPtr frm;
    Int16   formId;
    Boolean handled = false;

    if (event->eType == frmLoadEvent) {
        formId = event->data.frmLoad.formID;
        frm = FrmInitForm(formId);
        FrmSetActiveForm(frm);
        
        switch(formId) {
            case frmMain:
                FrmSetEventHandler(frm, MainFormHandleEvent);
                break;
            default:
                break;
        }
        
        handled = true;
    }
    
    return handled;
}


UInt32 PilotMain( UInt16 cmd, void *cmdPBP, UInt16 launchFlags)
{
    EventType   event;
    
    if(cmd == sysAppLaunchCmdNormalLaunch)
    {
        FrmGotoForm(frmMain);
            
        do {
            EvtGetEvent(&event, 100);
            
            if (!SysHandleEvent(&event))
                if (!ApplicationHandleEvent(&event))
						FrmDispatchEvent(&event);
        } while (event.eType != appStopEvent);
        
        FrmCloseAllForms();
    }
    
    return 0; 
}

