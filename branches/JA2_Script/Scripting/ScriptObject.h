#ifndef __SCRIPT_OBJECT__
#define __SCRIPT_OBJECT__

#include "Ja2 all.h"


namespace Ja2 {
namespace Scripting {
namespace Script {

typedef enum
{
	SO_NONE=0
} ScriptObjectType;

class ScriptObject
{
	public:
		virtual BOOLEAN Load(HWFILE hFile);
		virtual BOOLEAN Save(HWFILE hFile);
};

} // End of namespace Script
} // End of namespace Scripting
} // End of namespace Ja2




#endif