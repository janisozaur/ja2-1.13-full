#include <stdio.h>
#include <string.h>
#include <Lua Interpreter.h>
#include "lwstring.h"

#include "Overhead.h"

lua_State *L;

static const CHAR8 * LuaCommonGetSet( lua_State *L )
{
	const CHAR8 *AttrToFind;

	lua_pushstring( L, "__ptr" );
	lua_rawget( L, 1 );
	if (!lua_islightuserdata( L, -1 ) )
	{
		luaL_error( L, "The object pointer is invalid");
	}

	if (!luaL_getmetafield( L, 1, "attribs"))
	{
		luaL_error( L, "Should not happen:  The given object has no attributes table");
	}

	if (!lua_islightuserdata( L, -1 ) )
	{
		luaL_error( L, "Should not happen:  The given object attributes table is invalid");
	}

	if (lua_isnumber( L, 2) )
	{
		AttrToFind = "__idx";
	}
	else if (lua_isstring( L, 2) )
	{
		AttrToFind = lua_tostring( L, 2);

		// Automatically disallow reading of __idx
		if (!strcmp( AttrToFind, "__idx") ||
			!strcmp( AttrToFind, "__ptr") )
		{
			return NULL;
		}
	}
	else
	{
		luaL_argcheck( L, 0, 2, "Invalid index to the table" );
	}

	return AttrToFind;
}

static int LuaGet( lua_State *L)
{
	LuaAttrib *Attributes;
	const CHAR8 * AttrToFind;

	AttrToFind = LuaCommonGetSet( L);
	if (!AttrToFind)
	{
		lua_pop(L, 1);
		lua_pushnil( L );
		return 1;
	}

	Attributes = (LuaAttrib*) lua_touserdata( L, -1 );
	if (!Attributes)
	{
		luaL_error( L, "Should not happen:  The attributes table is NULL");
	}
	lua_pop(L, 1);

	for ( ; Attributes->name; Attributes++ )
	{
		if (!strcmp( Attributes->name, AttrToFind) )
		{
			if (!Attributes->get)
			{
				lua_pop(L, 1);
				lua_pushnil( L );
				return 1;
			}

			return Attributes->get( L );
		}
	}

	// The attribute was not found.  Return nil
	lua_pop(L, 1);
	lua_pushnil( L );
	return 1;
}

static int LuaSet( lua_State *L)
{
	LuaAttrib *Attributes;
	const CHAR8 *AttrToFind;

	AttrToFind = LuaCommonGetSet( L);
	if (!AttrToFind)
	{
		luaL_error( L, "Cannot set attribute" );
	}

	Attributes = (LuaAttrib*) lua_touserdata( L, -1 );
	if (!Attributes)
	{
		luaL_error( L, "Should not happen:  The attributes table is NULL");
	}
	lua_pop(L, 1);

	for ( ; Attributes->name; Attributes++ )
	{
		if (!strcmp( Attributes->name, AttrToFind) )
		{
			if (!Attributes->set)
			{
				luaL_error( L, "The attribute is not writable" );
			}

			return Attributes->set( L );
		}
	}

	luaL_argcheck( L, lua_isstring( L, 2), 2, "Invalid index for this object" );
	// Remove the ptr from the stack since this assignment is local to the LUA environment
	lua_pop(L, 1);
	lua_rawset( L, 1 );

	return 0;
}

static luaL_Reg LuaAccessors[] = {
	{ "__index", LuaGet, },
	{ "__newindex", LuaSet, },
	{ NULL, },
};

void CreateLuaType( lua_State *L, STR8 TypeName, luaL_Reg *LuaAccessors )
{
	luaL_Reg *idx;

	luaL_newmetatable( L, TypeName);
	for (idx = LuaAccessors; idx->name; idx++)
	{
		lua_pushstring( L, idx->name);
		lua_pushcfunction( L, idx->func);
		lua_rawset( L, -3 );
	}
}

void CreateLuaClass( lua_State *L, STR8 ClassName, LuaAttrib *Attribs )
{
	CreateLuaType( L, ClassName, LuaAccessors );
	lua_pushstring( L, "attribs" );
	lua_pushlightuserdata( L, Attribs );
	lua_rawset( L, -3 );
}

void CreateLuaObject( lua_State *L, void *Ptr )
{
	// Create the object and initialize its pointer
	lua_newtable( L );
	lua_pushstring( L, "__ptr" );
	lua_pushlightuserdata( L, Ptr);
	lua_rawset( L, -3 );
	// Swap the table and metatable because the next function expects the metatable to be first
	lua_insert( L, -2 );
	lua_setmetatable( L, -2 );
}

void NewLuaObject( lua_State *L, STR8 ClsName, void *Ptr )
{
	luaL_getmetatable( L, ClsName );
	if (lua_istable( L, -1 ) )
	{
		CreateLuaObject( L, Ptr );
	}
}






static int LuaGetMercPtr( lua_State *L)
{
	int v = lua_tointeger( L, 2 );
	luaL_argcheck( L, (v >= 0 && v < TOTAL_SOLDIERS), 2, "The soldier index is out of range");
	if (MercPtrs[ v ] )
	{
		NewLuaObject( L, "_soldier", MercPtrs[ v ] );
	}
	else
	{
		lua_pushnil( L );
	}
	
	return 1;
}

LuaAttrib SoldierList[] = {
	{ "__idx", LuaGetMercPtr, NULL },
	{ NULL, },
};




int LuaGetSoldierName( lua_State *L )
{
	SOLDIERTYPE *pSoldier = (SOLDIERTYPE*) lua_touserdata( L, -1 );
	lua_pop(L, 1 );
	if (!pSoldier)
	{
		lua_pushnil( L );
	}
	else
	{
		luaWS_newstr( L, pSoldier->name);
	}
	return 1;
}

LuaAttrib Soldier[] = {
	{ "name", LuaGetSoldierName, NULL },
	{ NULL, },
};



extern luaL_Reg WStringMethods[];

void InitializeLua( )
{
	L = lua_open();
	luaL_openlibs(L);
	
	// Create a wide-character savvy string
	CreateLuaType( L, "wstring", WStringMethods);
	lua_setglobal( L, "wstring"); // We also want this class to be known to the script

	// Create a soldier type
	CreateLuaClass( L, "_soldier", Soldier );
	lua_pop(L, 1);

	// Create a soldier list type and create its object
	CreateLuaClass( L, "_soldierlist", SoldierList );
	CreateLuaObject( L, NULL );
	// Give this new object a name
	lua_setglobal( L, "Soldiers" );
}

void EvalLua (STR8 buff) {
	int error;
	error = luaL_loadbuffer(L, buff, strlen(buff), "line") ||
		lua_pcall(L, 0, 0, 0);
	if (error) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1);  /* pop error message from the stack */
	}
}

void ShutdownLua( ) {
	lua_close(L);
}
