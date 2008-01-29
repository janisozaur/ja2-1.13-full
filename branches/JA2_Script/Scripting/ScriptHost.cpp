#include "ScriptHost.h"
#include "screenids.h"

// For testing purposes
#include "MessageBoxScreen.h"
#include "StrategicMap.h"
#include <string>
/////////////////////////

using namespace Ja2;
using namespace Scripting;


// ScriptHost constructor and destructor
ScriptHost::ScriptHost(UINT32* gpuiCurrentScreenVar, BOOLEAN* gpfGamePausedVar)
{
	memset((void*)&this->GenericGameVars, 0, sizeof(this->GenericGameVars));
	memset((void*)&this->ScriptHostThreadInfo, 0, sizeof(this->ScriptHostThreadInfo));

	InitializeCriticalSection(&this->EventData.cs);

	if(!gpuiCurrentScreenVar || !gpfGamePausedVar)
	{
		throw("Invalid pointer passed to ScriptHost::ScriptHost(UINT32*, BOOLEAN*)!");
	}
	else
	{
		this->GenericGameVars.gpuiCurrentScreen = gpuiCurrentScreenVar;
		this->GenericGameVars.gpfGamePaused = gpfGamePausedVar;
		this->ScriptHostThreadInfo.fThreadActive = TRUE;
		this->ScriptHostThreadInfo.fSuspend = FALSE;
		this->ScriptHostThreadInfo.hThread = CreateThread(NULL, 
															0, Ja2::Scripting::ScriptHost::ScriptHostProc, 
															(void*) this, 0, &this->ScriptHostThreadInfo.dwThreadId);
	}
}

ScriptHost::~ScriptHost()
{
	this->ScriptHostThreadInfo.fThreadActive = FALSE;
}


// Private member functions
DWORD WINAPI ScriptHost::ScriptHostProc(void *pvParam)
{
	ScriptHost* sh = (ScriptHost*)pvParam;

	while(sh->ScriptHostThreadInfo.fThreadActive)
	{
		// Scripts only run while..
		// a)...screens, that can advance time, are currently active
		// b)...game time isn't paused
		if(	(*sh->GenericGameVars.gpuiCurrentScreen == ScreenTypes::GAME_SCREEN ||
			*sh->GenericGameVars.gpuiCurrentScreen == ScreenTypes::MAP_SCREEN) &&
			!(*sh->GenericGameVars.gpfGamePaused) )
			sh->ScriptHostLoop();
		else
		{
			if(sh->ScriptHostThreadInfo.fSuspend)
			{

				Sleep(INFINITE);
			}
		}
			
	}

	return 0;
}

BOOLEAN ScriptHost::ScriptHostLoop(void)
{
	while(!this->EventData.event_queue.empty())
	{
		EnterCriticalSection(&this->EventData.cs); // We're working on the event queue, let's lock access

		switch(this->EventData.event_queue[0].eventId)
		{
			case EventType::ON_GAME_START:
				// Test code
				if(*this->GenericGameVars.gpuiCurrentScreen == ScreenTypes::GAME_SCREEN)
					DoMessageBox( MSG_BOX_BASIC_STYLE, L"Game has started!", *this->GenericGameVars.gpuiCurrentScreen, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL );
				else
				{
					LeaveCriticalSection(&this->EventData.cs);
					return TRUE;
				}
				break;

			case EventType::ON_TRACKED_SECTOR_PLAYER_CONTROLLED:
				// Test code
				{
					CHAR16 szSector[4];
					GetShortSectorString(SECTORX(this->EventData.event_queue[0].uiId), SECTORY(this->EventData.event_queue[0].uiId), &szSector[0]);
					ScreenMsg(FONT_MCOLOR_LTGREEN, MSG_TESTVERSION, L"Player has conquered sector %s!", szSector);
				}
				break;

			case EventType::ON_TRACKED_SECTOR_ENEMY_CONTROLLED:
				// Test code
				{
					CHAR16 szSector[4];
					GetShortSectorString(SECTORX(this->EventData.event_queue[0].uiId), SECTORY(this->EventData.event_queue[0].uiId), &szSector[0]);
					ScreenMsg(FONT_MCOLOR_RED, MSG_DEBUG, L"The enemy has conquered sector %s!", szSector);
				}
				break;
		}

		this->EventData.event_queue.pop_front(); // We've processed that event, delete from queue
		LeaveCriticalSection(&this->EventData.cs); // Give the main game thread an opportunity to add a new event
	}

	return TRUE;
}

void ScriptHost::ReportGameEvent(EventType eventId, UINT32 uiObjectId, UINT32 uiAParam, UINT32 uiBParam)
{
	EnterCriticalSection(&this->EventData.cs);
	EventStruct* newEvent = new EventStruct;

	newEvent->eventId = eventId;
	newEvent->uiId = uiObjectId;
	newEvent->uiAParam = uiAParam;
	newEvent->uiBParam = uiBParam;

	this->EventData.event_queue.push_back(*newEvent);
	
	LeaveCriticalSection(&this->EventData.cs);
}

void ScriptHost::ReportGameEvent(EventType eventId, UINT32 uiObjectId)
{
	EnterCriticalSection(&this->EventData.cs);
	EventStruct* newEvent = new EventStruct;

	newEvent->eventId = eventId;
	newEvent->uiId = uiObjectId;

	this->EventData.event_queue.push_back(*newEvent);
	
	LeaveCriticalSection(&this->EventData.cs);
}