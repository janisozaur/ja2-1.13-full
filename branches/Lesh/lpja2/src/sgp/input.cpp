#ifdef JA2_PRECOMPILED_HEADERS
	#include "ja2_sgp_all.h"
#else
	#include "platform.h"
	#include "types.h"
	#include "debug.h"
	#include "input.h"
	#include "mem_man.h"
	#include "english.h"
	#include "video.h"
	#include "local.h"
	#include "SDL.h"
	#include "timing.h"
#endif

// Make sure to refer to the translation table which is within one of the following files (depending
// on the language used). ENGLISH.C, JAPANESE.C, FRENCH.C, GERMAN.C, SPANISH.C, etc...

extern UINT16 gsStandardKeyTable[];
extern UINT16 gsExtendedKeyTable[];
extern UINT16 gsStandardShiftKeyTable[];
extern UINT16 gsStandardCtrlKeyTable[];
extern UINT16 gsStandardAltKeyTable[];
extern UINT16 gsExtendedShiftKeyTable[];
extern UINT16 gsExtendedCtrlKeyTable[];
extern UINT16 gsExtendedAltKeyTable[];
extern UINT16 gsExtendedNumKeyTable[];

extern BOOLEAN gfApplicationActive;

// The gfKeyState table is used to track which of the keys is up or down at any one time. This is used while polling
// the interface.

BOOLEAN   gfKeyState[256];				// TRUE = Pressed, FALSE = Not Pressed

// mouse clipping info
BOOLEAN   fCursorWasClipped = FALSE;
SGPRect   gCursorClipRect;



// The gsKeyTranslationTables basically translates scan codes to our own key value table. Please note that the table is 2 bytes
// wide per entry. This will be used since we will use 2 byte characters for translation purposes.

UINT16		gfShiftState;				// TRUE = Pressed, FALSE = Not Pressed
UINT16		gfAltState;					// TRUE = Pressed, FALSE = Not Pressed
UINT16		gfCtrlState;				// TRUE = Pressed, FALSE = Not Pressed

// These data structure are used to track the mouse while polling

BOOLEAN		gfTrackDblClick;
UINT32		guiDoubleClkDelay;			// Current delay in milliseconds for a delay
UINT32		guiSingleClickTimer;
UINT32		guiRecordedWParam;
UINT32		guiRecordedLParam;
UINT16		gusRecordedKeyState;
BOOLEAN		gfRecordedLeftButtonUp;

UINT32		guiLeftButtonRepeatTimer;
UINT32		guiRightButtonRepeatTimer;
UINT32		guiMiddleButtonRepeatTimer;

BOOLEAN		gfTrackMousePos;			// TRUE = queue mouse movement events, FALSE = don't
BOOLEAN		gfLeftButtonState;			// TRUE = Pressed, FALSE = Not Pressed
BOOLEAN		gfRightButtonState;			// TRUE = Pressed, FALSE = Not Pressed
BOOLEAN		gfMiddleButtonState;		// TRUE = Pressed, FALSE = Not Pressed
UINT16		gusMouseXPos;				// X position of the mouse on screen
UINT16		gusMouseYPos;				// y position of the mouse on screen

// The queue structures are used to track input events using queued events

InputAtom gEventQueue[256];
UINT16    gusQueueCount;
UINT16    gusHeadIndex;
UINT16    gusTailIndex;

// Local function headers

void    QueueEvent(UINT16 ubInputEvent, UINT32 usParam, UINT32 uiParam);
void	HandleSingleClicksAndButtonRepeats( void );
void	AdjustMouseForWindowOrigin(void);
void	KeyDown(SDL_keysym *KeySym);
void	KeyUp(SDL_keysym *KeySym);
void	MouseMove(SDL_MouseMotionEvent *MouseMove);
void	MouseButton(SDL_MouseButtonEvent *MouseButton);
BOOLEAN	ClipMouseCursor(UINT16 x, UINT16 y);

// These are the hook functions for both keyboard and mouse

// *************************************************************
// Keyboard handler
//  [in]  event - key event information
// *************************************************************
void KeyboardHandler(SDL_Event *event)
{
	if( gfApplicationActive )
	{ 
		switch (event->key.state)
		{
		case SDL_PRESSED:
			// Key was up
			KeyDown(&event->key.keysym);
			break;

		case SDL_RELEASED:
			// The key has been released
			KeyUp(&event->key.keysym);
			break;
		default:
			break;
		}
	}
}

// *************************************************************
// Mouse handler
//  [in]  event - mouse information
// *************************************************************
void MouseHandler(SDL_Event *event)
{
	if ( gfApplicationActive )
	{
		switch (event->type)
		{
		case SDL_MOUSEMOTION:
			MouseMove(&event->motion);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			MouseButton(&event->button);
			break;

		default:
			break;
		}
	}
}

// *************************************************************
// Mouse motion handler
//  [in]  MouseMovement - mouse motion information
// *************************************************************
void MouseMove(SDL_MouseMotionEvent *MouseMovement)
{
	UINT32 uiParam;

	// if clip rect is on and we're out of rect, do not remember cursor position
	// there will be second event with (x,y) within rect
	if ( !ClipMouseCursor(MouseMovement->x, MouseMovement->y) )
	{
		gusMouseXPos = MouseMovement->x;
		gusMouseYPos = MouseMovement->y;

		uiParam = gusMouseYPos;
		uiParam = uiParam << 16;
		uiParam = uiParam | gusMouseXPos;

		// Trigger an input event
		if (gfTrackMousePos == TRUE)
		{
			QueueEvent(MOUSE_POS, 0, uiParam);
		}
	}
}

// *************************************************************
// Mouse button down handler
//  [in]  MouseButton - mouse button information
// *************************************************************
void MouseButton(SDL_MouseButtonEvent *MouseButton)
{
	UINT32 uiParam;

	gusMouseXPos = MouseButton->x;
	gusMouseYPos = MouseButton->y;

    uiParam = gusMouseYPos;
    uiParam = uiParam << 16;
    uiParam = uiParam | gusMouseXPos;

	switch (MouseButton->button)
	{
	case SDL_BUTTON_LEFT:
		if (MouseButton->state == SDL_PRESSED)
		{
			// left mouse button down
			gfLeftButtonState = TRUE;
			QueueEvent(LEFT_BUTTON_DOWN, 0, uiParam);
		}
		else
		{
			// left mouse button up
			gfLeftButtonState = FALSE;
			QueueEvent(LEFT_BUTTON_UP, 0, uiParam);
		}
		break;

	case SDL_BUTTON_MIDDLE:
		if (MouseButton->state == SDL_PRESSED)
		{
			// middle mouse button down
			gfMiddleButtonState = TRUE;
			QueueEvent(MIDDLE_BUTTON_DOWN, 0, uiParam);
		}
		else
		{
			// middle mouse button up
			gfMiddleButtonState = FALSE;
			QueueEvent(MIDDLE_BUTTON_UP, 0, uiParam);
		}
		break;

	case SDL_BUTTON_RIGHT:
		if (MouseButton->state == SDL_PRESSED)
		{
			// left mouse button down
			gfRightButtonState = TRUE;
			QueueEvent(RIGHT_BUTTON_DOWN, 0, uiParam);
		}
		else
		{
			// left mouse button up
			gfRightButtonState = FALSE;
			QueueEvent(RIGHT_BUTTON_UP, 0, uiParam);
		}
		break;

	case SDL_BUTTON_WHEELUP:
		if (MouseButton->state == SDL_RELEASED)
		{
			// mouse wheel has finished rotation
			QueueEvent(MOUSE_WHEEL_UP, 0, uiParam);
		}
		break;

	case SDL_BUTTON_WHEELDOWN:
		if (MouseButton->state == SDL_RELEASED)
		{
			// mouse wheel has finished rotation
			QueueEvent(MOUSE_WHEEL_DOWN, 0, uiParam);
		}
		break;

	default:
		break;
	}
}

// *************************************************************
// Initialize input manager
// *************************************************************
BOOLEAN InitializeInputManager(void)
{
	RegisterDebugTopic(TOPIC_INPUT, "Input Manager");
	printf("Initializing Input Manager...\n");

	// Initialize the gfKeyState table to FALSE everywhere
	memset(gfKeyState, FALSE, 256);

	// Initialize the Event Queue
	gusQueueCount = 0;
	gusHeadIndex  = 0;
	gusTailIndex  = 0;

	// By default, we will not queue mousemove events
	gfTrackMousePos = FALSE;

	// Initialize other variables
	gfShiftState = FALSE;
	gfAltState   = FALSE;
	gfCtrlState  = FALSE;

	// Initialize variables pertaining to DOUBLE CLIK stuff
	gfTrackDblClick			= TRUE;
	guiDoubleClkDelay		= DBL_CLK_TIME;
	guiSingleClickTimer	= 0;
	gfRecordedLeftButtonUp	= FALSE;

	// Initialize variables pertaining to the button states
	gfLeftButtonState   = FALSE;
	gfRightButtonState  = FALSE;
	gfMiddleButtonState = FALSE;

	// Initialize variables pertaining to the repeat mechanism
	guiLeftButtonRepeatTimer   = 0;
	guiRightButtonRepeatTimer  = 0;
	guiMiddleButtonRepeatTimer = 0;

	// Set the mouse to the center of the screen
	gusMouseXPos = SCREEN_WIDTH / 2;
	gusMouseYPos = SCREEN_HEIGHT / 2;

	// Set the mouse clipping area - by default, all screen
	gCursorClipRect.iLeft   = gCursorClipRect.iTop = 0;
	gCursorClipRect.iRight  = SCREEN_WIDTH;
	gCursorClipRect.iBottom = SCREEN_HEIGHT;

	return TRUE;
}

// *************************************************************
// Shutdown input manager
// *************************************************************
void ShutdownInputManager(void)
{	// There's very little to do when shutting down the input manager. In the future, this is where the keyboard and
	// mouse hooks will be destroyed
	UnRegisterDebugTopic(TOPIC_INPUT, "Input Manager");
}

// *************************************************************
// Put an event into event queue
//  [in]  ubInputEvent - event type
//  [in]  usParam - mostly it is key code
//  [in]  uiParam - mostly it is mouse position
// *************************************************************
void QueueEvent(UINT16 ubInputEvent, UINT32 usParam, UINT32 uiParam)
{
  UINT32 uiTimer;
  UINT16 usKeyState;

  uiTimer = SDL_GetTicks();
  usKeyState = gfShiftState | gfCtrlState | gfAltState;

  // Can we queue up one more event, if not, the event is lost forever
  if (gusQueueCount == 256)
  { // No more queue space
    return;
  }

  if (ubInputEvent == LEFT_BUTTON_DOWN)
  {
    guiLeftButtonRepeatTimer = uiTimer + BUTTON_REPEAT_TIMEOUT;
  }

  if (ubInputEvent == RIGHT_BUTTON_DOWN)
  {
    guiRightButtonRepeatTimer = uiTimer + BUTTON_REPEAT_TIMEOUT;
  }

  if (ubInputEvent == MIDDLE_BUTTON_DOWN)
  {
    guiMiddleButtonRepeatTimer = uiTimer + BUTTON_REPEAT_TIMEOUT;
  }

  if (ubInputEvent == LEFT_BUTTON_UP)
  {
    guiLeftButtonRepeatTimer = 0;
  }

  if (ubInputEvent == RIGHT_BUTTON_UP)
  {
    guiRightButtonRepeatTimer = 0;
  }

  if (ubInputEvent == MIDDLE_BUTTON_UP)
  {
    guiMiddleButtonRepeatTimer = 0;
  }

  if ( (ubInputEvent == LEFT_BUTTON_UP) )
  {
		// Do we have a double click
		if ( ( uiTimer - guiSingleClickTimer ) < DBL_CLK_TIME )
		{
				guiSingleClickTimer = 0;

				// Add a button up first...
				gEventQueue[gusTailIndex].uiTimeStamp = uiTimer;
				gEventQueue[gusTailIndex].usKeyState = gusRecordedKeyState;
				gEventQueue[gusTailIndex].usEvent = LEFT_BUTTON_UP;
				gEventQueue[gusTailIndex].usParam = usParam;
				gEventQueue[gusTailIndex].uiParam = uiParam;

				// Increment the number of items on the input queue
				gusQueueCount++;

				// Increment the gusTailIndex pointer
				if (gusTailIndex == 255)
				{ // The gusTailIndex is about to wrap around the queue ring
					gusTailIndex = 0;
				}
				else
				{ // We simply increment the gusTailIndex
					gusTailIndex++;
				}


				// Now do double click
				gEventQueue[gusTailIndex].uiTimeStamp = uiTimer;
				gEventQueue[gusTailIndex].usKeyState = gusRecordedKeyState ;
				gEventQueue[gusTailIndex].usEvent = LEFT_BUTTON_DBL_CLK;
				gEventQueue[gusTailIndex].usParam = usParam;
				gEventQueue[gusTailIndex].uiParam = uiParam;

				// Increment the number of items on the input queue
				gusQueueCount++;

				// Increment the gusTailIndex pointer
				if (gusTailIndex == 255)
				{ // The gusTailIndex is about to wrap around the queue ring
					gusTailIndex = 0;
				}
				else
				{ // We simply increment the gusTailIndex
					gusTailIndex++;
				}

				return;
		}
		else
		{
			// Save time
			guiSingleClickTimer = uiTimer;
		}
  }

  // Okey Dokey, we can queue up the event, so we do it
  gEventQueue[gusTailIndex].uiTimeStamp = uiTimer;
  gEventQueue[gusTailIndex].usKeyState = usKeyState;
  gEventQueue[gusTailIndex].usEvent = ubInputEvent;
  gEventQueue[gusTailIndex].usParam = usParam;
  gEventQueue[gusTailIndex].uiParam = uiParam;

  // Increment the number of items on the input queue
  gusQueueCount++;

  // Increment the gusTailIndex pointer
  if (gusTailIndex == 255)
  { // The gusTailIndex is about to wrap around the queue ring
    gusTailIndex = 0;
  }
  else
  { // We simply increment the gusTailIndex
    gusTailIndex++;
  }
}

BOOLEAN DequeueSpecificEvent(InputAtom *Event, UINT32 uiMaskFlags )
{
  // Is there an event to dequeue
  if (gusQueueCount > 0)
  {
		memcpy( Event, &( gEventQueue[gusHeadIndex] ), sizeof( InputAtom ) );

		// Check if it has the masks!
		if ( ( Event->usEvent & uiMaskFlags ) )
		{
			return( DequeueEvent( Event) );
		}
	}

	return( FALSE );
}

BOOLEAN DequeueEvent(InputAtom *Event)
{
	HandleSingleClicksAndButtonRepeats( );

  // Is there an event to dequeue
  if (gusQueueCount > 0)
  {
		// We have an event, so we dequeue it
		memcpy( Event, &( gEventQueue[gusHeadIndex] ), sizeof( InputAtom ) );

		if (gusHeadIndex == 255)
		{
			gusHeadIndex = 0;
		}
		else
		{
			gusHeadIndex++;
		}

		// Decrement the number of items on the input queue
		gusQueueCount--;

		// dequeued an event, return TRUE
		return TRUE;
  }
	else
	{
		// No events to dequeue, return FALSE
		return FALSE;
	}
}


// *************************************************************
// Keychange handler
//  [in]  KeySym - key information
// *************************************************************
void KeyChange(SDL_keysym *KeySym, UINT8 ufKeyState)
{
	UINT16 ubChar;
	SGPPos  MousePos;
	UINT32 uiTmpLParam;

	// Find ucChar by translating KeySym->sym using the Key Translation Tables.

	// handle keypad with numlock on and off
	if ( KeySym->mod & KMOD_NUM && KeySym->sym >= SDLK_KP0 && KeySym->sym <= SDLK_KP_EQUALS )
	{
		ubChar = gsExtendedNumKeyTable[ KeySym->sym - SDLK_KP0 ];
	}
	else if ( gfShiftState )
	{ 
		// SHIFT is pressed
		if ( KeySym->sym >= SDLK_KP0 )
			ubChar = gsExtendedShiftKeyTable[ KeySym->sym - SDLK_KP0 ];
		else
			ubChar = gsStandardShiftKeyTable[ KeySym->sym ];
	}
	else if( gfAltState )
	{
		// ALT is pressed
		if ( KeySym->sym >= SDLK_KP0 )
			ubChar = gsExtendedAltKeyTable[ KeySym->sym - SDLK_KP0 ];
		else
			ubChar = gsStandardAltKeyTable[ KeySym->sym ];
	}
	else if ( gfCtrlState )
	{
		// CTRL is pressed
		if ( KeySym->sym >= SDLK_KP0 )
			ubChar = gsExtendedCtrlKeyTable[ KeySym->sym - SDLK_KP0 ];
		else
			ubChar = gsStandardCtrlKeyTable[ KeySym->sym ];
	}
	else
	{
		// no mod key is pressed
		if ( KeySym->sym >= SDLK_KP0 )
			ubChar = gsExtendedKeyTable[ KeySym->sym - SDLK_KP0 ];
		else
			ubChar = gsStandardKeyTable[ KeySym->sym ];
	}

	//printf("code=%d\n", ubChar);

	GetMousePos(&MousePos);
	uiTmpLParam = ((MousePos.y << 16) & 0xffff0000) | (MousePos.x & 0x0000ffff);

	if (ufKeyState == TRUE)
	{ 
		// Key has been PRESSED
		// !! ubChar holds code of pressed key.
		// 8 low bits is the code, 3 next is a shift, ctrl or alt mod
		// we will be using only 8 lowest bits
		// Find out if the key is already pressed and if not, queue an event and update the gfKeyState array
		if (gfKeyState[ ubChar & 0xFF ] == FALSE)
		{ 
			// There is no string input going on right now, so we queue up the event
			gfKeyState[ ubChar & 0xFF ] = TRUE;
			QueueEvent(KEY_DOWN, ubChar, uiTmpLParam);
		}
		else
		{ 
			// There is no string input going on right now, so we queue up the event
			QueueEvent(KEY_REPEAT, ubChar, uiTmpLParam);
		}
	}
	else
	{ 
		// Key has been RELEASED
		// !! ubChar holds code of pressed key.
		// 8 low bits is the code, 3 next is a shift, ctrl or alt mod
		// we will be using only 8 lowest bits
		// if the alt tab key was pressed
#if 0
		if( ubChar == TAB && gfAltState )
		{
			// therefore minimize the application
			SDL_WM_IconifyWindow();
			gfKeyState[ ALT ] = FALSE;
			gfAltState = FALSE;
		}
#endif
		// check for alt+enter
		if ( ubChar == ENTER && gfAltState )
		{
			SDL_WM_ToggleFullScreen( SDL_GetVideoSurface() );
			gfKeyState[ ALT ] = FALSE;
			gfAltState = FALSE;
		}
		// Find out if the key is already pressed and if so, queue an event and update the gfKeyState array
		if (gfKeyState[ ubChar & 0xFF ] == TRUE)
		{ 
			// Well the key has just been pressed, therefore we queue up and event and update the gsKeyState
			gfKeyState[ ubChar & 0xFF ] = FALSE;
			QueueEvent(KEY_UP, ubChar, uiTmpLParam);
		}
	}
}

void KeyPrint(SDL_keysym *KeySym)
{
	printf("sym=%d\n", KeySym->sym);
}

// *************************************************************
// Keypress handler
//  [in]  KeySym - keypress information
// *************************************************************
void KeyDown(SDL_keysym *KeySym)
{	
	// Are we PRESSING down one of SHIFT, ALT or CTRL ???
	if (KeySym->sym == SDLK_RSHIFT || KeySym->sym == SDLK_LSHIFT)
	{	
		// SHIFT key is PRESSED
		gfShiftState      = SHIFT_DOWN;
		gfKeyState[SHIFT] = TRUE;
	}
	else if (KeySym->sym == SDLK_RCTRL || KeySym->sym == SDLK_LCTRL)
	{	
		// CTRL key is PRESSED
		gfCtrlState      = CTRL_DOWN;
		gfKeyState[CTRL] = TRUE;
	}
	else if (KeySym->sym == SDLK_RALT || KeySym->sym == SDLK_LALT)
	{	
		// ALT key is pressed
		gfAltState      = ALT_DOWN;
		gfKeyState[ALT] = TRUE;
	}
	else
	{
		if (KeySym->sym == SDLK_PRINT)
		{
			//PrintScreen();
			// DB Done in the KeyUp function
			// this used to be keyed to SCRL_LOCK
			// which I believe Luis gave the wrong value
		}
		else
		{
			// No special keys have been pressed
			// Call KeyChange() and pass TRUE to indicate key has been PRESSED and not RELEASED
			KeyChange(KeySym, TRUE);
			//KeyPrint(KeySym);
		}
	}
}

// *************************************************************
// Keyrelease handler
//  [in]  KeySym - keyrelease information
// *************************************************************
void KeyUp(SDL_keysym *KeySym)
{	
	// Are we RELEASING one of SHIFT, ALT or CTRL ???
	if (KeySym->sym == SDLK_RSHIFT || KeySym->sym == SDLK_LSHIFT)
	{	
		// SHIFT key is RELEASED
		gfShiftState      = FALSE;
		gfKeyState[SHIFT] = FALSE;
	}
	else if (KeySym->sym == SDLK_RCTRL || KeySym->sym == SDLK_LCTRL)
	{	
		// CTRL key is RELEASED
		gfCtrlState      = FALSE;
		gfKeyState[CTRL] = FALSE;
	}
	else if (KeySym->sym == SDLK_RALT || KeySym->sym == SDLK_LALT)
	{	
		// ALT key is RELEASED
		gfAltState      = FALSE;
		gfKeyState[ALT] = FALSE;
	}
	else
	{
		if (KeySym->sym == SDLK_PRINT)
		{
			// DB this used to be keyed to SCRL_LOCK
			// which I believe Luis gave the wrong value
			if (_KeyDown(CTRL))
				VideoCaptureToggle();
			else
				PrintScreen();
		}
		else
		{
			// No special keys have been pressed
			// Call KeyChange() and pass FALSE to indicate key has been PRESSED and not RELEASED
			KeyChange(KeySym, FALSE);
			//KeyPrint(KeySym);
		}
	}
}

void EnableDoubleClk(void)
{
	// Obsolete
}

void DisableDoubleClk(void)
{
	// Obsolete
}

void GetMousePos(SGPPos *Point)
{
	Point->x = (UINT32) gusMouseXPos;
	Point->y = (UINT32) gusMouseYPos;

	return;
}




//
// Miscellaneous input-related utility functions:
//

void RestrictMouseToXYXY(UINT16 usX1, UINT16 usY1, UINT16 usX2, UINT16 usY2)
{
	SGPRect TempRect;

	TempRect.iLeft   = usX1;
	TempRect.iTop    = usY1;
	TempRect.iRight  = usX2;
	TempRect.iBottom = usY2;

	RestrictMouseCursor(&TempRect);
}

void RestrictMouseCursor(SGPRect *pRectangle)
{
	// Make a copy of our rect....
	memcpy( &gCursorClipRect, pRectangle, sizeof( gCursorClipRect ) );
	fCursorWasClipped = TRUE;
}

void FreeMouseCursor(void)
{
	gCursorClipRect.iLeft   = 0;
	gCursorClipRect.iTop    = 0;
	gCursorClipRect.iRight  = SCREEN_WIDTH;
	gCursorClipRect.iBottom = SCREEN_HEIGHT;
	fCursorWasClipped = FALSE;
}

void GetRestrictedClipCursor( SGPRect *pRectangle )
{
	*pRectangle = gCursorClipRect;
}

BOOLEAN IsCursorRestricted( void )
{
	return( fCursorWasClipped );
}

BOOLEAN ClipMouseCursor(UINT16 x, UINT16 y)
{
	BOOLEAN fClippingOccured = FALSE;
	UINT16	usX, usY;

	if ( fCursorWasClipped )
	{
		usX = x;
		usY = y;

		if ( usX < gCursorClipRect.iLeft )
		{
			usX = gCursorClipRect.iLeft;
			fClippingOccured = TRUE;
		}
		if ( usX > gCursorClipRect.iRight )
		{
			usX = gCursorClipRect.iRight;
			fClippingOccured = TRUE;
		}
		if ( usY < gCursorClipRect.iTop )
		{
			usY = gCursorClipRect.iTop;
			fClippingOccured = TRUE;
		}
		if ( usY > gCursorClipRect.iBottom )
		{
			usY = gCursorClipRect.iBottom;
			fClippingOccured = TRUE;
		}

		if ( fClippingOccured )
			SDL_WarpMouse(usX, usY);
	}

	return fClippingOccured;
}

void SimulateMouseMovement( UINT32 uiNewXPos, UINT32 uiNewYPos )
{
	SDL_WarpMouse( (UINT16)uiNewXPos, (UINT16)uiNewYPos );
}



BOOLEAN InputEventInside(InputAtom *Event, UINT32 uiX1, UINT32 uiY1, UINT32 uiX2, UINT32 uiY2)
{
	UINT32 uiEventX, uiEventY;

	uiEventX = _EvMouseX(Event);
	uiEventY = _EvMouseY(Event);

	return((uiEventX >= uiX1) && (uiEventX <= uiX2) && (uiEventY >= uiY1) && (uiEventY <= uiY2));
}


void DequeueAllKeyBoardEvents()
{
	InputAtom	InputEvent;

	//Deque all the events waiting in the SGP queue
	while (DequeueEvent(&InputEvent) == TRUE)
	{
		//dont do anything
	}
}



void HandleSingleClicksAndButtonRepeats( void )
{
	UINT32 uiTimer;


	uiTimer = GetTickCount();

	// Is there a LEFT mouse button repeat
	if (gfLeftButtonState)
	{
    	if ((guiLeftButtonRepeatTimer > 0)&&(guiLeftButtonRepeatTimer <= uiTimer))
    	{
      		UINT32 uiTmpLParam;
      		SGPPos  MousePos;

      		GetMousePos(&MousePos);
      		uiTmpLParam = ((MousePos.y << 16) & 0xffff0000) | (MousePos.x & 0x0000ffff);
      		QueueEvent(LEFT_BUTTON_REPEAT, 0, uiTmpLParam);
      		guiLeftButtonRepeatTimer = uiTimer + BUTTON_REPEAT_TIME;
    	}
  	}
  	else
  	{
    	guiLeftButtonRepeatTimer = 0;
  	}


	// Is there a RIGHT mouse button repeat
	if (gfRightButtonState)
  	{
    	if ((guiRightButtonRepeatTimer > 0)&&(guiRightButtonRepeatTimer <= uiTimer))
    	{
      		UINT32 uiTmpLParam;
      		SGPPos  MousePos;

      		GetMousePos(&MousePos);
      		uiTmpLParam = ((MousePos.y << 16) & 0xffff0000) | (MousePos.x & 0x0000ffff);
      		QueueEvent(RIGHT_BUTTON_REPEAT, 0, uiTmpLParam);
      		guiRightButtonRepeatTimer = uiTimer + BUTTON_REPEAT_TIME;
    	}
  	}
  	else
  	{
    	guiRightButtonRepeatTimer = 0;
  	}


	// Is there a MIDDLE mouse button repeat
	if (gfMiddleButtonState)
  	{
    	if ((guiMiddleButtonRepeatTimer > 0)&&(guiMiddleButtonRepeatTimer <= uiTimer))
    	{
      		UINT32 uiTmpLParam;
      		SGPPos  MousePos;

      		GetMousePos(&MousePos);
      		uiTmpLParam = ((MousePos.y << 16) & 0xffff0000) | (MousePos.x & 0x0000ffff);
      		QueueEvent(MIDDLE_BUTTON_REPEAT, 0, uiTmpLParam);
      		guiMiddleButtonRepeatTimer = uiTimer + BUTTON_REPEAT_TIME;
    	}
  	}
  	else
  	{
    	guiMiddleButtonRepeatTimer = 0;
  	}
}
