///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2004 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://wwhiz.com/LuaPlus/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <windows.h>
#endif WIN32

#ifdef WIN32
#ifndef _WIN32_WCE
#include <io.h>
#endif _WIN32_WCE
#endif WIN32

#include "LuaPlus.h"
#include "LuaPlusFunctions.h"
extern "C" {
#include "include/lua.h"
#include "include/lauxlib.h"
#include "src/lfunc.h"
#include "src/lobject.h"
#include "src/lstring.h"
#include "src/lmem.h"
#include "src/lopcodes.h"
#include "src/lundump.h"
} // extern "C"

#ifdef _MSC_VER
#pragma warning( disable: 4100 )
#endif _MSC_VER

namespace LuaPlus {

typedef void (*fnLuaPlus_RegisterModule)( LuaState* state );
typedef void (*fnLuaPlus_UnregisterModule)( LuaState* state );


static int LS_import( LuaState* state )
{
#ifndef LUAPLUS_LIB
	LuaPlus::LuaStack args(state);
	LPCSTR moduleName = args[ 1 ].GetString();

#ifdef _DEBUG
	const char* luaplusdllName = "LuaPlusD.dll";
#else _DEBUG
	const char* luaplusdllName = "LuaPlus.dll";
#endif _DEBUG

	char path[ _MAX_PATH ];
	GetModuleFileName(GetModuleHandle(luaplusdllName), path, _MAX_PATH);
	char* slashPtr = strrchr( path, '\\' );
	slashPtr++;
	*slashPtr = 0;
	strcat( path, "modules/" );
	strcat( path, moduleName );
	strcat( path, ".dll" );
#ifdef _DEBUG
	strcat( path, "d" );
#endif _DEBUG
	_strlwr( path );

	LuaPlus::LuaObject importModulesObj = state->GetRegistry()[ "ImportModules" ];
	if ( importModulesObj.IsNil() )
	{
		importModulesObj = state->GetRegistry().CreateTable( "ImportModules" );
	}

	LuaPlus::LuaObject moduleObj = importModulesObj[ path ];
	if ( !moduleObj.IsNil() )
	{
		// Already exists.
		state->PushBoolean( true );
		return 1;
	}

	HINSTANCE hInst = LoadLibrary( path );
	if ( hInst == INVALID_HANDLE_VALUE )
	{
		// Didn't load successfully.
		state->PushBoolean( false );
		return 1;
	}

	fnLuaPlus_RegisterModule registerModule = (fnLuaPlus_RegisterModule)
			GetProcAddress( hInst , "LuaPlus_RegisterModule" );
	if ( !registerModule )
	{
		// Isn't a LuaPlus DLL.
		state->PushBoolean( false );
		return 1;
	}

	(*registerModule)( state );

	state->PushBoolean( true );
#else !LUAPLUS_LIB
	state->PushBoolean(false);
#endif LUAPLUS_LIB
	return 1;
}
	

static Proto* combine(lua_State* L, int n)
{
	if (n==1)
	{
		const Closure* c=(const Closure*)lua_topointer(L,-1);
		return c->l.p;
	}
	else
	{
		int i,pc=0;
		Proto* f=luaF_newproto(L);
		f->source=luaS_newliteral(L,"=(none)");
		f->maxstacksize=1;
		f->p=luaM_newvector(L,n,Proto*);
		f->sizep=n;
		f->sizecode=2*n+1;
		f->code=luaM_newvector(L,f->sizecode,Instruction);
		for (i=0; i<n; i++)
		{
			const Closure* c=(const Closure*)lua_topointer(L,i-n);
			f->p[i]=c->l.p;
			f->code[pc++]=CREATE_ABx(OP_CLOSURE,0,i);
			f->code[pc++]=CREATE_ABC(OP_CALL,0,1,1);
		}
		f->code[pc++]=CREATE_ABC(OP_RETURN,0,1,0);
		return f;
	}
}


static void strip(lua_State* L, Proto* f)
{
	int i,n=f->sizep;
	luaM_freearray(L, f->lineinfo, f->sizelineinfo, int);
	luaM_freearray(L, f->locvars, f->sizelocvars, struct LocVar);
	f->lineinfo=NULL;
	f->sizelineinfo=0;
	f->locvars=NULL;
	f->sizelocvars=0;
	f->source=luaS_newliteral(L,"=(none)");
	for (i=0; i<n; i++) strip(L,f->p[i]);
}


static int writer(lua_State* L, const void* p, size_t size, void* u)
{
	UNUSED(L);
	return fwrite(p,size,1,(FILE*)u)==1;
}


static int LS_dump( LuaState* state )
{
	LuaPlus::LuaStack args(state);
	const char* inFileName = args[1].GetString();
	const char* outFileName = args[2].GetString();
	if (luaL_loadfile(*state, inFileName) != 0)
	{
		state->PushBoolean(false);
		return 1;
	}

	FILE* D = fopen(outFileName, "wb");
	if (!D)
	{
		state->PushBoolean(false);
		return 1;
	}
	
	Proto* f = combine(*state, 1);
//	if (D==NULL) cannot(output,"open","out");
//	if (stripping) strip(L,f);
	luaU_dump(*state, f, writer, D);
	fclose(D);

	state->PushBoolean(true);
	return 1;
}



void ScriptFunctionsRegister( LuaState* state )
{
	LuaPlus::LuaObject globalsObj = state->GetGlobals();
	globalsObj.Register("import",			LS_import);
	globalsObj.Register("LuaDumpBinary",	LS_dump);
}

} // namespace LuaPlus





/*
--[[function io.lines(file, openType)
	if type( file ) == "string" then
		if file:sub( 1, 1 ) == "|" then
			file = io.popen( file:sub( 2 ), openType )
		else
			file = io.open( file, openType )
		end
	end
	
	return function ()
		return file:read() or (assert(file:close()) and nil), i
	end
end


function DirProcessFiles(path, wildcard, func)
	if type(func) ~= "function" then
		return
	end

	path = path:gsub("/", "\\")
	if path:sub(path:len()) ~= '\\' then
		path = path .. "\\"
	end

	local handle = FileFind.First(path .. wildcard)

	while 1 do
		local fileName = handle:GetFileName()
		local isDirectory = handle:IsDirectory()
		func(path, fileName)
		if not handle:Next() then
			break
		end
	end

	handle:Close()
end


function DirProcessFilesRecursive(path, func, ext)
	if ext then
		ext = ext:lower()
	end

	if type(func) ~= "function" then
		return
	end

	path = path:gsub("/", "\\")
	if path:sub(path:len()) ~= '\\' then
		path = path .. "\\"
	end

	function ReadDirectory(path)
		local handle = FileFind.First(path .. "*.*")

		while handle do
			local fileName = handle:GetFileName()
			local isDirectory = handle:IsDirectory()
			if isDirectory then
				if fileName ~= "." and fileName ~= ".." then
					ReadDirectory(path .. fileName .. "\\")
				end
			else
				if ext then
					local dotPos, endPos, foundExt = fileName:find("%.(%w*)$")
					if foundExt then
						foundExt = foundExt:lower()
						if ext == foundExt then
							func(path, fileName)
						end
					end
				else
					func(path, fileName)
				end
			end
			if not handle:Next() then
				break
			end
		end
		
		if handle then
			handle:Close()
		end
	end

	ReadDirectory(path)
end
]]
*/