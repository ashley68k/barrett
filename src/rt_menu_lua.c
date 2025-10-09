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

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "rt_def.h"
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

static lua_State *MENU_LUA_STATE = NULL;

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

static luaL_Reg L_rott_reg[] = {
	{"echo", L_rott_echo},
	{"print", L_rott_echo},
	{NULL, NULL}
};

int luaopen_rott(lua_State *L)
{
	luaL_newlib(L, L_rott_reg);
	return 1;
}

void LCP_Init(void)
{
	MENU_LUA_STATE = luaL_newstate();
	if (!MENU_LUA_STATE)
		return;

	luaopen_math(MENU_LUA_STATE);
	luaopen_rott(MENU_LUA_STATE);
}

void LCP_Quit(void)
{
	if (MENU_LUA_STATE)
		lua_close(MENU_LUA_STATE);
	MENU_LUA_STATE = NULL;
}
