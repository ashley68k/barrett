/*
Copyright (C) 1994-1995 Apogee Software, Ltd.
Copyright (C) 2002-2015 icculus.org, GNU/Linux port
Copyright (C) 2017-2018 Steven LeVesque
Copyright (C) 2025 lunarmeadow (she/her)
Copyright (C) 2025 erysdren (it/its)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
//******************************************************************************
//
// RT_MENU_LUA.C
//    Contains the (LUA) menu stuff!
//
//******************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include "SDL2/SDL.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "rt_def.h"
#include "_rt_menu.h"
#include "rt_menu.h"
#include "rt_menu_lua.h"
#include "rt_sound.h"
#include "fx_man.h"
#include "rt_build.h"
#include "rt_in.h"
#include "isr.h"
#include "z_zone.h"
#include "w_wad.h"
#include "rt_util.h"
#include "rt_main.h"
#include "rt_playr.h"
#include "rt_rand.h"
#include "rt_game.h"
#include "rt_floor.h"
#include "rt_draw.h"
#include "rt_view.h"
#include "rt_str.h"
#include "rt_vid.h"
#include "rt_ted.h"
#include "rt_com.h"
#include "lumpy.h"
#include "rt_cfg.h"
#include "version.h"
#include "modexlib.h"
#include "rt_msg.h"
#include "rt_net.h"
#include "rt_scale.h"

#include "rt_battl.h"
#include "develop.h"

//=============================================================================
//
// Lua ROTT Game Library
//
//=============================================================================

static int L_rott_echo(lua_State *L)
{
	int i;
	static char msg[256];
	char *ptr = msg;
	char *end = ptr + sizeof(msg);

	for (i = 1; i <= lua_gettop(L); i++)
	{
		ptr += SDL_snprintf(ptr, end - ptr, "%s ", lua_tostring(L, i));
	}

	SDL_Log("%.*s", (int)sizeof(msg), msg);

	return 0;
}

static int L_rott_quit(lua_State *L)
{
	(void)(L);
	QuitGame();
	return 0;
}

static luaL_Reg L_rott_reg[] = {
	{"echo", L_rott_echo},
	{"print", L_rott_echo},
	{"quit", L_rott_quit},
	{NULL, NULL}
};

int luaopen_rott(lua_State *L)
{
	luaL_newlib(L, L_rott_reg);
	return 1;
}

//=============================================================================
//
// Lua Control Panel (LCP)
//
//=============================================================================

static boolean MENU_LUA_INITIALIZED = false;
static lua_State *MENU_LUA_STATE = NULL;

static const luaL_Reg MENU_LUA_LIBS[] = {
	{LUA_GNAME, luaopen_base},
	{LUA_TABLIBNAME, luaopen_table},
	{LUA_STRLIBNAME, luaopen_string},
	{LUA_MATHLIBNAME, luaopen_math},
	{LUA_UTF8LIBNAME, luaopen_utf8},
	{LUA_ROTTLIBNAME, luaopen_rott},
	{NULL, NULL}
};

void LCP_Init(void)
{
	const luaL_Reg *lib;

	if (MENU_LUA_INITIALIZED)
		return;

	MENU_LUA_STATE = luaL_newstate();
	if (!MENU_LUA_STATE)
		return;

	for (lib = MENU_LUA_LIBS; lib->func; lib++)
	{
		luaL_requiref(MENU_LUA_STATE, lib->name, lib->func, 1);
		lua_pop(MENU_LUA_STATE, 1);
	}

	MENU_LUA_INITIALIZED = true;
}

void LCP_Quit(void)
{
	if (!MENU_LUA_INITIALIZED)
		return;
	if (MENU_LUA_STATE)
		lua_close(MENU_LUA_STATE);
	MENU_LUA_STATE = NULL;
	MENU_LUA_INITIALIZED = false;
}

#if 0
// https://stackoverflow.com/a/6142700
static void iterate_menu_table(lua_State *L, int index, int (*callback)(const char *key, const char *value, void *user), void *user)
{
	const char *key = NULL, *value = NULL;
	boolean done = false;
	// Push another reference to the table on top of the stack (so we know
	// where it is, and this function can work for negative, positive and
	// pseudo indices
	lua_pushvalue(L, index);
	// stack now contains: -1 => table
	lua_pushnil(L);
	// stack now contains: -1 => nil; -2 => table
	while (lua_next(L, -2))
	{
		// stack now contains: -1 => value; -2 => key; -3 => table
		// copy the key so that lua_tostring does not modify the original
		lua_pushvalue(L, -2);
		// stack now contains: -1 => key; -2 => value; -3 => key; -4 => table
		key = lua_tostring(L, -1);
		if (lua_istable(L, -2))
		{
			iterate_menu_table(L, -2, callback, user);
		}
		else
		{
			value = lua_tostring(L, -2);
			if (callback)
				if (callback(key, value, user))
					done = true;
		}
		// pop value + copy of key, leaving original key
		lua_pop(L, 2);
		// stack now contains: -1 => key; -2 => table
		if (done)
			break;
	}
	// stack now contains: -1 => table (when lua_next returns 0 it pops the key
	// but does not push anything.)
	// Pop table
	lua_pop(L, 1);
	// Stack is now the same as it was on entry to this function
}
#endif

typedef struct rt_lua_menu {
	char *title; ///< menu title
	CP_iteminfo info; ///< menu info
	CP_itemtype *items; ///< menu items
	CP_MenuNames *names; ///< menu item names
	int ref; ///< reference value in LUA_REGISTRYINDEX table
} rt_lua_menu_t;

static void FreeMenu(lua_State *L, rt_lua_menu_t *menu)
{
	if (menu)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, menu->ref);
		if (menu->title)
			free(menu->title);
		if (menu->items)
			free(menu->items);
		if (menu->names)
			free(menu->names);
		free(menu);
	}
}

// evaluate the menu table at the top of the stack
static rt_lua_menu_t *EvaluateMenu(lua_State *L)
{
	int i;
	rt_lua_menu_t *menu = NULL;

	// check if user returned a table
	if (!lua_istable(L, -1))
	{
		luaL_error(L, "returned value from menu is not a table");
		return NULL;
	}

	// allocate menu state
	menu = calloc(1, sizeof(rt_lua_menu_t));

	// register the menu table at the top of the stack
	menu->ref = luaL_ref(L, LUA_REGISTRYINDEX);

	// push the table to the stack by the reference we just made
	lua_rawgeti(L, LUA_REGISTRYINDEX, menu->ref);

	// get title string
	if (lua_getfield(L, -1, "title") == LUA_TSTRING)
		menu->title = strdup(lua_tostring(L, -1));
	else
		menu->title = strdup("Menu");
	lua_pop(L, 1);

	// initialize item info
	menu->info.x = MENU_X;
	menu->info.y = MENU_Y;
	menu->info.curpos = 0;
	menu->info.indent = 32;

	// get items array
	if (lua_getfield(L, -1, "items") == LUA_TTABLE)
	{
		// get number of items
		menu->info.amount = (int)lua_rawlen(L, -1);
		if (menu->info.amount <= 0)
			goto no_items;

		// allocate items and names
		menu->items = calloc(menu->info.amount, sizeof(CP_itemtype));
		menu->names = calloc(menu->info.amount, sizeof(CP_MenuNames));

		// initialize items and names
		for (i = 0; i < menu->info.amount; i++)
		{
			menu->items[i].active = CP_Active;

			// strncpy(menu->names[i], sizeof(menu->names[i]), );
		}
	}
	else
	{
no_items:
		// no items in the table, just crash out
		luaL_error(L, "no items array present in menu table");
		FreeMenu(L, menu);
		return NULL;
	}

	return menu;
}

#define MENU_MAIN_SCRIPT "res/menus/main.lua"

void LCP_MainMenu(void)
{
	rt_lua_menu_t *menu = NULL;

	// setup menu
	SetupMenuBuf();
	SetUpControlPanel();

	// load main menu script
	if (luaL_loadfile(MENU_LUA_STATE, MENU_MAIN_SCRIPT) != LUA_OK)
		Error("Lua error: %s", lua_tostring(MENU_LUA_STATE, -1));
	if (lua_pcall(MENU_LUA_STATE, 0, 1, 0) != LUA_OK)
		Error("Lua error: %s", lua_tostring(MENU_LUA_STATE, -1));

	// evaluate menu
	menu = EvaluateMenu(MENU_LUA_STATE);
	if (!menu)
		Error("Lua error: %s", lua_tostring(MENU_LUA_STATE, -1));

	// run menu loop
	while (1)
	{
		IN_ClearKeysDown();

		EnableScreenStretch();
		SetAlternateMenuBuf();
		ClearMenuBuf();

#if 0
		SetMenuTitle(title);
		iterate_menu_table(MENU_LUA_STATE, -1, do_table, NULL);
#endif

		DisplayInfo(0);

		FlipMenuBuf();
	}

	// cleanup menu
	CleanUpControlPanel();
	ShutdownMenuBuf();
}
