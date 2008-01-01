#ifndef __INPUT_
#define __INPUT_

#include "types.h"
#include "SDL.h"

#define SCAN_CODE_MASK				0xff0000
#define EXT_CODE_MASK				0x01000000
#define TRANSITION_MASK				0x80000000

#define KEY_DOWN					0x0001
#define KEY_UP						0x0002
#define KEY_REPEAT					0x0004
#define LEFT_BUTTON_DOWN			0x0008
#define LEFT_BUTTON_UP				0x0010
#define LEFT_BUTTON_DBL_CLK			0x0020
#define LEFT_BUTTON_REPEAT			0x0040
#define RIGHT_BUTTON_DOWN			0x0080
#define RIGHT_BUTTON_UP				0x0100
#define RIGHT_BUTTON_REPEAT			0x0200
#define MOUSE_POS					0x0400
#define MOUSE_WHEEL_UP				0x0800
#define MOUSE_WHEEL_DOWN			0x1000
#define MIDDLE_BUTTON_DOWN			0x2000
#define MIDDLE_BUTTON_UP			0x4000
#define MIDDLE_BUTTON_REPEAT		0x8000
#define ALL_INPUT					0xFFFF
#define KEYBOARD_INPUTS				(KEY_DOWN | KEY_UP | KEY_REPEAT)
#define LEFT_BUTTON_INPUTS			(LEFT_BUTTON_DOWN | LEFT_BUTTON_UP | LEFT_BUTTON_REPEAT)
#define RIGHT_BUTTON_INPUTS			(RIGHT_BUTTON_DOWN | RIGHT_BUTTON_UP | RIGHT_BUTTON_REPEAT)
#define MIDDLE_BUTTON_INPUTS		(MIDDLE_BUTTON_DOWN | MIDDLE_BUTTON_UP | MIDDLE_BUTTON_REPEAT)
#define MOUSE_WHEEL_INPUTS			(MOUSE_WHEEL_UP | MOUSE_WHEEL_DOWN)
#define MOUSE_BUTTONS_INPUT			(LEFT_BUTTON_INPUTS | RIGHT_BUTTON_INPUTS | MIDDLE_BUTTON_INPUTS)

#define SHIFT_DOWN					0x01
#define CTRL_DOWN					0x02
#define ALT_DOWN					0x04

#define MAX_STRING_INPUT			64
#define DBL_CLK_TIME				300     // Increased by Alex, Jun-10-97, 200 felt too short
#define BUTTON_REPEAT_TIMEOUT		250
#define BUTTON_REPEAT_TIME			50

typedef struct
{ 
  UINT32 uiTimeStamp;
  UINT16 usKeyState;
  UINT16 usEvent;
  UINT32 usParam;
  UINT32 uiParam;

} InputAtom;

//Mouse pos extracting macros from InputAtom
#define GETYPOS(a) HIWORD(((a)->uiParam))
#define GETXPOS(a) LOWORD(((a)->uiParam))


#ifdef __cplusplus
extern "C" {
#endif

extern BOOLEAN		InitializeInputManager(void);
extern void			ShutdownInputManager(void);
extern BOOLEAN		DequeueEvent(InputAtom *Event);
extern void			QueueEvent(UINT16 ubInputEvent, UINT32 usParam, UINT32 uiParam);

extern void			KeyboardHandler(SDL_Event *event);
extern void			MouseHandler(SDL_Event *event);

extern void			EnableDoubleClk(void);
extern void			DisableDoubleClk(void);
extern void			GetMousePos(SGPPos *Point);

extern BOOLEAN		DequeueSpecificEvent(InputAtom *Event, UINT32 uiMaskFlags );
extern void			RestrictMouseToXYXY(UINT16 usX1, UINT16 usY1, UINT16 usX2, UINT16 usY2);
extern void			RestrictMouseCursor(SGPRect *pRectangle);
extern void			FreeMouseCursor(void);
extern BOOLEAN		IsCursorRestricted( void );
extern void			GetRestrictedClipCursor( SGPRect *pRectangle );
extern void         RestoreCursorClipRect( void );
extern void			ForgetAllPressedKeys( void );

void SimulateMouseMovement( UINT32 uiNewXPos, UINT32 uiNewYPos );
BOOLEAN InputEventInside(InputAtom *Event, UINT32 uiX1, UINT32 uiY1, UINT32 uiX2, UINT32 uiY2);

extern void DequeueAllKeyBoardEvents();


extern BOOLEAN   gfKeyState[256];			// TRUE = Pressed, FALSE = Not Pressed

extern UINT16    gusMouseXPos;       // X position of the mouse on screen
extern UINT16    gusMouseYPos;       // y position of the mouse on screen
extern BOOLEAN   gfLeftButtonState;  // TRUE = Pressed, FALSE = Not Pressed
extern BOOLEAN   gfRightButtonState; // TRUE = Pressed, FALSE = Not Pressed
extern BOOLEAN   gfMiddleButtonState;// TRUE = Pressed, FALSE = Not Pressed


#define _KeyDown(a)			gfKeyState[(a)]
#define _LeftButtonDown		gfLeftButtonState
#define _RightButtonDown	gfRightButtonState
#define _MiddleButtonDown	gfMiddleButtonState
#define _MouseXPos			gusMouseXPos
#define _MouseYPos			gusMouseYPos

// NOTE: this may not be the absolute most-latest current mouse co-ordinates, use GetMousePos for that
#define _gusMouseInside(x1,y1,x2,y2)   ((gusMouseXPos >= x1) && (gusMouseXPos <= x2) && (gusMouseYPos >= y1) && (gusMouseYPos <= y2))

#define _EvType(a)         ((InputAtom *)(a))->usEvent
#define _EvTimeStamp(a)    ((InputAtom *)(a))->uiTimeStamp
#define _EvKey(a)          ((InputAtom *)(a))->usParam
#define _EvMouseX(a)       (UINT16)(((InputAtom *)(a))->uiParam & 0x0000ffff)
#define _EvMouseY(a)       (UINT16)((((InputAtom *)(a))->uiParam & 0xffff0000) >> 16)
#define _EvShiftDown(a)    (((InputAtom *)(a))->usKeyState & SHIFT_DOWN)
#define _EvCtrlDown(a)     (((InputAtom *)(a))->usKeyState & CTRL_DOWN)
#define _EvAltDown(a)      (((InputAtom *)(a))->usKeyState & ALT_DOWN)

#ifdef __cplusplus
}
#endif

#endif
