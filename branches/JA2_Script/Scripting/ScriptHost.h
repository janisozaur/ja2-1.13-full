#ifndef __SCRIPT_HOST__
#define __SCRIPT_HOST__

#include "Ja2 all.h"
#include <deque>

namespace Ja2 {
namespace Scripting {

typedef enum
{
	NO_EVENT=0,

	// Game time events
	ON_GAME_START,

	// Shipment related events
	ON_SHIPMENT_DELIVERY,

	// Sector- and town-related events
	ON_TRACKED_SECTOR_PLAYER_CONTROLLED,
	ON_TRACKED_SECTOR_ENEMY_CONTROLLED
} EventType;

typedef struct
{
	EventType	eventId;
	UINT32		uiId;	// Value identifying the affected object
	UINT32		uiAParam;	// Parameter A
	UINT32		uiBParam;	// Parameter B	
	UINT32		uiPlayerId; // Unused for now
} EventStruct;

class ScriptHost
{
	public:
		// Event report function(s) used by the legacy game code
		void ReportGameEvent(EventType eventId, UINT32 uiObjectId, UINT32 uiAParam, UINT32 uiBParam);
		void ReportGameEvent(EventType eventId, UINT32 uiObjectId);

		// Constructors and destructors
		ScriptHost(UINT32* gpuiCurrentScreenVar, BOOLEAN* gpfGamePausedVar);
		~ScriptHost();

	private:
		static DWORD WINAPI ScriptHostProc(void* pvParam); // The actual thread proc

		// This function is called by the thread proc and contains all the logic 
		// to handle script execution and event notifications
		BOOLEAN ScriptHostLoop(void); 

	private:
		struct
		{
			HANDLE				hThread;
			DWORD				dwThreadId;
			BOOLEAN				fThreadActive; // Setting this to FALSE will cause the script host thread to terminate
			BOOLEAN				fSuspend; // Setting this to TRUE will suspend execution of the script host thread
		} ScriptHostThreadInfo;

		struct
		{
			UINT32* gpuiCurrentScreen; // Pointer to the game's global current screen variable
			BOOLEAN* gpfGamePaused;
		} GenericGameVars;

		struct
		{
			// Required! STL containers are not thread-safe when there are reading as well as writing threads, 
			// that access the object
			CRITICAL_SECTION cs;
			// Queue to store event notificatios
			std::deque<EventStruct> event_queue;
		} EventData;

		struct
		{

		} ScriptTables;
};


} // End of namespace Scripting
} // End of namespace Ja2


#endif