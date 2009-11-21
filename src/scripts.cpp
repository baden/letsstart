#include<stdlib.h>
#include<stdio.h>

#include "scripts.h"

#ifdef	__cplusplus
extern "C" {
#endif

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#ifdef	__cplusplus
};
#endif


static int print(lua_State *L)
{
 int n=lua_gettop(L);
 int i;
 for (i=1; i<=n; i++)
 {
  if (i>1) printf("\t");
  if (lua_isstring(L,i))
   printf("%s",lua_tostring(L,i));
  else if (lua_isnil(L,i))
   printf("%s","nil");
  else if (lua_isboolean(L,i))
   printf("%s",lua_toboolean(L,i) ? "true" : "false");
  else
   printf("%s:%p",luaL_typename(L,i),lua_topointer(L,i));
 }
 printf("\n");
 return 0;
}


lua_State * lua;

static void l_message (const char *pname, const char *msg) {
  if (pname) fprintf(stderr, "%s: ", pname);
  fprintf(stderr, "%s\n", msg);
  fflush(stderr);
}

int scripttest()
{
	printf("Hello, world!!!\n");

    lua = lua_open ();         // create Lua context
 lua_register(lua,"print",print);
//	luaopen_jit ();

    if ( lua == NULL ){
        printf("Scripts initializatin fail.\n");
    } else {
        luaL_openlibs(lua);                              // open standart libraries

        if ( luaL_loadfile ( lua, "test.lua" ) ){
//        if ( luaL_loadfile ( lua, "scripts\\levels.lub" ) ){
            printf("Error loading 'test.lua'.\n%s", lua_tostring(lua, -1));
//            MessageBox(NULL, text, "Error", MB_ICONERROR | MB_OK);
        }
        int status = lua_pcall( lua, 0, LUA_MULTRET, 0 );
	if(status){
        	l_message(NULL, lua_pushfstring(lua,
                               "error calling " LUA_QL("print") " (%s)",
                               lua_tostring(lua, -1)));
		//printf("Error in script\n");
	} else printf("Lua starting successful !!!\n");
    }
    return 1;
}
