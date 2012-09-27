#ifndef _M68KCALLS_H
#define _M68KCALLS_H

#include <stdio.h>
#include <stdarg.h>

#include <PalmOS.h>
#include <VFSMgr.h>


#define	offset1970      0x7C25B080

#define memNewChunkFlagAllowLarge			0x1000	// allow >64K allocations

#ifdef __cplusplus
extern "C" {
#endif

extern SysAppInfoPtr SysGetAppInfo(SysAppInfoPtr *uiAppPP, SysAppInfoPtr *actionCodeAppPP);

#ifdef __cplusplus
}
#endif


#if 0
typedef char Char;
typedef char	Int8;
typedef short  Int16;
typedef long   Int32;
typedef unsigned char	UInt8;
typedef unsigned short  UInt16;
typedef unsigned long   UInt32;
typedef UInt32 FileRef;
typedef UInt32 SndStreamRef;
typedef unsigned char	Boolean;
typedef UInt32 LocalID;
typedef UInt16 Err;
typedef UInt32 MemPtr;
typedef struct _opaque *MemHandle;	// global handle

#define	errNone						  0x0000	// No error
#define	vfsErrorClass				0x2A00	// Virtual Filesystem Manager and Filesystem library
#define vfsErrFileEOF				(vfsErrorClass | 7)	// file pointer is at end of file

#define vfsIteratorStart              0L
#define vfsIteratorStop               0xffffffffL

#ifdef __cplusplus
extern "C" {
#endif

// operations for the WinScreenMode function
typedef enum WinScreenModeOperationTag 
{
	winScreenModeGetDefaults,
	winScreenModeGet,
	winScreenModeSetToDefaults,
	winScreenModeSet,
	winScreenModeGetSupportedDepths,
	winScreenModeGetSupportsColor
} WinScreenModeOperation;

// selectors for WinScreenGetAttribute
typedef enum WinScreenAttrTag 
{
	winScreenWidth,
	winScreenHeight,
	winScreenRowBytes,
	winScreenDepth,
	winScreenAllDepths,
	winScreenDensity,
	winScreenPixelFormat,
	winScreenResolutionX,
	winScreenResolutionY
} WinScreenAttrType;

// Operations for the WinPalette function
#define winPaletteGet 				  0
#define winPaletteSet 				  1
#define winPaletteSetToDefault	2

// constants used by WinSetCoordinateSystem
#define kCoordinatesNative			0
#define kCoordinatesStandard		72
#define kCoordinatesOneAndAHalf		108
#define kCoordinatesDouble			144
#define kCoordinatesTriple			216
#define kCoordinatesQuadruple		288

typedef enum DensityTag {
	kDensityLow				= 72,
	kDensityOneAndAHalf	= 108,
	kDensityDouble			= 144,
	kDensityTriple			= 216,
	kDensityQuadruple		= 288
} DensityType;

typedef struct PointType {
  UInt16 x;
  UInt16 y;
} PointType;

typedef struct RectangleType {
  PointType  topLeft;
  PointType  extent;
} RectangleType;

typedef struct RGBColorType
{
	UInt8           index;				// index of color or best match to cur CLUT or unused.
	UInt8           r;						// amount of red, 0->255
	UInt8           g;						// amount of green, 0->255
	UInt8           b;						// amount of blue, 0->255
} RGBColorType;

// enum for WinLockScreen
typedef enum WinLockInitTag 
{
	winLockCopy, 
	winLockErase, 
	winLockDontCare
} WinLockInitType;

// enum for WinCreateOffscreenWindow
typedef enum WindowFormatTag 
{ 
	screenFormat = 0, 
	genericFormat, 
	nativeFormat 
} WindowFormatType;

// transfer modes for color drawing
typedef enum WinDrawOperationTag 
{
	winPaint, 
	winErase, 
	winMask, 
	winInvert, 
	winOverlay, 
	winPaintInverse, 
	winSwap
} WinDrawOperation;

// enum for WinScrollRectangle
typedef enum WinDirectionTag 
{ 
	winUp = 0, 
	winDown, 
	winLeft, 
	winRight 
} WinDirectionType;

// Event timeouts
#define  evtWaitForever    -1
#define  evtNoWait          0

// The event record.
struct _GenericEventType {
	UInt16         datum[8];
   };
struct _PenUpEventType {
   PointType      start;            // display coord. of stroke start
   PointType      end;              // display coord. of stroke start
   };
struct _KeyDownEventType {
   UInt16         chr;              // ascii code
   UInt16         keyCode;          // virtual key code
   UInt16         modifiers;
   };
struct _KeyUpEventType {
   UInt16         chr;              // ascii code
   UInt16         keyCode;          // virtual key code
   UInt16         modifiers;
   };
struct _KeyHoldEventType {
   UInt16         chr;              // ascii code
   UInt16         keyCode;          // virtual key code
   UInt16         modifiers;
   };
struct _WinEnterEventType {
   void *      enterWindow;
   void *      exitWindow;
   };
struct _WinExitEventType {
   void *      enterWindow;
   void *      exitWindow;
   };
typedef struct SysEventType {
   UInt16         eType;
   Boolean        penDown;
   UInt8          tapCount;
   Int16          screenX;
   Int16          screenY;
   union {
      struct _GenericEventType		generic;
      struct _PenUpEventType		penUp;
      struct _KeyDownEventType		keyDown;
      struct _KeyUpEventType		keyUp;
      struct _KeyHoldEventType		keyHold;
      struct _WinEnterEventType		winEnter;
      struct _WinExitEventType		winExit;
    } data;
} SysEventType;

typedef enum {
	nilEvent = 0,				// system level
	penDownEvent,				// system level
	penUpEvent,					// system level
	penMoveEvent,				// system level
	keyDownEvent,				// system level
	winEnterEvent,				// system level
	winExitEvent,				// system level
	appStopEvent = 22,			// system level
	keyUpEvent 			= 0x4000,
	keyHoldEvent 		= 0x4001,
	firstUserEvent = 0x6000,
  lastUserEvent  = 0x7FFF
} eventsEnum;


enum SndStreamWidthTag {
	sndMono,
	sndStereo
};

enum SndSampleTag {
	sndInt8 		= 0x01,
	sndUInt8		= 0x11,
	sndInt16Big		= 0x02,
	sndInt16Little	= 0x12,
	sndInt32Big		= 0x04,
	sndInt32Little	= 0x14,
	sndFloatBig		= 0x24,
	sndFloatLittle	= 0x34,
	sndInt16		= sndInt16Little,
	sndInt16Opposite= sndInt16Big,
	sndInt32		= sndInt32Little,		
	sndInt32Opposite= sndInt32Big,
	sndFloat		= sndFloatLittle,
	sndFloatOpposite= sndFloatBig
};

/*
 * special 'cookie' values that can be used instead of a real volume setting
 * */
enum
{
	sndSystemVolume = -1,
	sndGameVolume = -2,
	sndAlarmVolume = -3
};

//***********************************************************
// Stuff required to call traps
//***********************************************************
typedef struct
{
	UInt32 instr;
	UInt32 regD[8];
	UInt32 regA[8];
	UInt32 regPC;
} EmulState68K;

// SystemMgr.h
typedef struct SysAppInfoType {
	Int16				cmd;							// command code for app
	MemPtr			cmdPBP;						// cmd ParamBlock
	UInt16			launchFlags;				// launch flags

	UInt32			taskID;						// AMX task ID of task that app runs in
	MemHandle		codeH;						// handle of the main code segment
	DmOpenRef		dbP;							// Application database access pointer of App
	UInt8				*stackP;						// stack chunk for the App
	UInt8				*globalsChunkP;			// globals chunk for the App

	UInt16			memOwnerID;					// owner ID for Memory Manager chunks
	MemPtr			dmAccessP;					// pointer to linked list of opened DB's
	Err				dmLastErr;					// Last error from Data Manager
	MemPtr			errExceptionP;				// ErrTry,Catch exception list

	// PalmOS v3.0 fields begin here
	UInt8				*a5Ptr;						// A5 pointer for this app
	UInt8				*stackEndP;					// stack chunk end for the App (last byte)
	UInt8				*globalEndP;				// global chunk end for the App (last byte)
	struct SysAppInfoType *rootP;				// Points to the SysAppInfoType first
														// allocated for this thread.
	MemPtr			extraP;						// unused pointer for the App.
	} SysAppInfoType;
typedef SysAppInfoType *SysAppInfoPtr;

//-------------------------------------
// Define traps
//-------------------------------------
#define sysTrapSysTaskDelay							0xA0A0
#define sysTrapSysHandleEvent						0xA0A9
#define sysTrapTimGetTicks							0xA0F7
#define sysTrapEvtGetEvent							0xA11D
#define sysTrapFrmDispatchEvent					0xA1A0
#define sysTrapMenuHandleEvent					0xA1BF
#define sysTrapWinCreateOffscreenWindow	0xA1F7
#define sysTrapWinDeleteWindow					0xA1F8
#define sysTrapWinGetDrawWindow					0xA1FE
#define sysTrapWinGetDisplayWindow			0xA200
#define sysTrapWinCopyRectangle					0xA209
#define sysTrapWinScrollRectangle				0xA20A
#define sysTrapWinDrawRectangle					0xA218
#define sysTrapWinEraseRectangle				0xA219
#define sysTrapWinDrawChars							0xA220
#define sysTrapWinDrawBitmap						0xA226
#define sysTrapKeyCurrentState					0xA2A2
#define sysTrapKeySetMask								0xA2CF
#define sysTrapSysTicksPerSecond				0xA2E9
#define sysTrapWinScreenMode						0xA33E
#define sysTrapFileSystemDispatch				0xA348
#define sysTrapVFSMgr	                  sysTrapFileSystemDispatch
#define vfsTrapFileCreate		2
#define vfsTrapFileOpen		  3
#define vfsTrapFileClose		4
#define vfsTrapFileRead			6
#define vfsTrapFileWrite		7
#define vfsTrapFileDelete		8
#define vfsTrapFileSeek			10
#define vfsTrapFileEOF			11
#define vfsTrapFileTell			12
#define sysTrapBmpGetBits								0xA376
#define sysTrapWinSetForeColor					0xA398
#define sysTrapWinSetBackColor					0xA399
#define sysTrapWinSetTextColor					0xA39A
#define sysTrapWinPalette								0xA39D
#define sysTrapWinScreenLock						0xA3A0
#define sysTrapWinScreenUnlock					0xA3A1
#define sysTrapWinGetBitmap							0xA3A2
#define sysTrapBmpCreate								0xA3DD
#define sysTrapBmpDelete								0xA3DE
#define sysTrapWinCreateBitmapWindow		0xA3E4
#define sysTrapWinSetForeColorRGB				0xA3E8
#define sysTrapHighDensityDispatch			0xA3EC
#define HDSelectorBmpCreateBitmapV3					7
#define HDSelectorWinSetCoordinateSystem		8
#define HDSelectorWinGetCoordinateSystem    9
#define HDSelectorWinScreenGetAttribute			14
#define HDSelectorEvtGetPenNative					  17
#define sysTrapSndStreamCreate					0xA45B
#define sysTrapSndStreamDelete					0xA45C
#define sysTrapSndStreamStart						0xA45D
#define sysTrapSndStreamSetVolume				0xA460

#define memNewChunkFlagAllowLarge			0x1000	// allow >64K allocations

// PalomOS-calls via traps
extern UInt16 VFSFileOpen(UInt16 volRefNum, const Char *pathNameP, UInt16 openMode, FileRef *fileRefP);
extern UInt16 VFSFileClose(FileRef fileRef);
extern UInt16 VFSFileDelete(UInt16 volRefNum, const Char *pathNameP);
extern UInt16 VFSFileCreate(UInt16 volRefNum, const Char *pathNameP);
extern UInt16 VFSFileWrite(FileRef fileRef, UInt32 numBytes, const void *dataP, UInt32 *numBytesWrittenP);
extern UInt16 VFSFileRead(FileRef fileRef, UInt32 numBytes, void *bufP, UInt32 *numBytesReadP);
extern UInt16 VFSFileEOF(FileRef fileRef);
extern UInt16 VFSFileSeek(FileRef fileRef, UInt16 origin, Int32 offset);
extern UInt16 VFSFileTell(FileRef fileRef, UInt32 *filePosP);
extern Err VFSVolumeEnumerate(UInt16 *volRefNumP, UInt32 *volIteratorP);
extern UInt32 TimGetTicks(void);
extern UInt16 SysTicksPerSecond(void);
extern UInt16 SysTaskDelay(Int32 delay);
extern UInt16 SndStreamCreate(SndStreamRef *channel, UInt8 mode, UInt32 samplerate, UInt16 type, 
  UInt8 width, UInt32 func, void *userdata, UInt32 buffsize, UInt8 armNative);
extern UInt16 SndStreamStart(SndStreamRef channel);
extern UInt16 SndStreamSetVolume(SndStreamRef channel, Int32 volume);
extern UInt16 SndStreamDelete(SndStreamRef channel);
extern void *WinScreenLock(WinLockInitType mode);
extern void WinScreenUnlock(void);
extern UInt16 WinGetCoordinateSystem(void);
extern void *WinGetDisplayWindow(void);
extern UInt16 WinScreenMode(UInt16 mode, UInt32 *width, UInt32 *height, UInt32 *bpp, Boolean *enableColorP);
extern UInt16 WinScreenGetAttribute(UInt16 selector, UInt32 *result);
extern void WinSetForeColorRGB(const RGBColorType* newRgbP, RGBColorType* prevRgbP);
extern UInt8 WinSetBackColor(UInt8 index);
extern UInt8 WinSetForeColor(UInt8 index);
extern UInt8 WinSetTextColor(UInt8 index);
extern void WinDrawBitmap(void *bitmap, Int16 x, Int16 y);
extern void WinDrawChars(const char *chars, Int16 len, Int16 x, Int16 y);
extern void WinDrawRectangle (const RectangleType *rP, UInt16 cornerDiam);
extern void WinEraseRectangle(const RectangleType *rP, UInt16 cornerDiam);
extern void WinScrollRectangle(const RectangleType *rP, WinDirectionType dir, Int16 dist, RectangleType *vacatedP);
extern void *WinGetDrawWindow(void);
extern void *WinGetBitmap(void *winHandle);
extern UInt16 WinPalette(UInt8 operation, Int16 startIndex, UInt16 paletteEntries, RGBColorType *tableP);
extern void WinDeleteWindow(void *winHandle, UInt8 erase);
extern void *WinCreateBitmapWindow(void *bitmapP, UInt16 *error);
extern void *WinCreateOffscreenWindow(Int16 width, Int16 height, Int16 mode, UInt16 *error);
extern void WinCopyRectangle(void *srcWin, void *dstWin, RectangleType *rect, Int16 destX, Int16 destY, UInt8 mode);
extern UInt16 WinSetCoordinateSystem(UInt16 coordSys);
extern void EvtGetPenNative(void *winH, Int16* pScreenX, Int16* pScreenY, Boolean* pPenDown);
extern void *BmpGetBits(void *bitmapP);
extern void *BmpCreate(Int16 width, Int16 height, UInt8 depth, void *colortable, UInt16 *error);
extern void *BmpCreateBitmapV3(void *bitmapP, UInt16 density, void *bitsP, void *colortable);
extern UInt16 BmpDelete(void *bitmapP);
extern void EvtGetEvent(SysEventType *event, Int32 timeout);
extern Boolean SysHandleEvent(SysEventType *eventP);
extern Boolean MenuHandleEvent(void *menuP, SysEventType *event, UInt16 *error);
extern Boolean FrmDispatchEvent(SysEventType *eventP);
extern UInt32 KeySetMask(UInt32 mask);
extern UInt32 KeyCurrentState(void);
extern Err SysCurAppDatabase(UInt16 *cardNoP, LocalID *dbIDP);
extern MemPtr MemGluePtrNew(UInt32 size);
extern MemPtr MemPtrNew(UInt32 size);
extern SysAppInfoPtr SysGetAppInfo(SysAppInfoPtr *uiAppPP, SysAppInfoPtr *actionCodeAppPP);

#ifdef __cplusplus
}
#endif
#endif // 0

#endif // _M68KCALLS_H
