/*
 * framework/hardware/l-buzzer.c
 *
 * Copyright(c) 2007-2015 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <buzzer/buzzer.h>
#include <framework/hardware/l-hardware.h>

static int l_buzzer_new(lua_State * L)
{
	const char * name = luaL_checkstring(L, 1);
	struct buzzer_t * buzzer = search_buzzer(name);
	if(!buzzer)
		return 0;
	lua_pushlightuserdata(L, buzzer);
	luaL_setmetatable(L, MT_NAME_HARDWARE_BUZZER);
	return 1;
}

static int l_buzzer_list(lua_State * L)
{
	struct device_list_t * pos, * n;
	struct buzzer_t * buzzer;

	lua_newtable(L);
	list_for_each_entry_safe(pos, n, &(__device_list.entry), entry)
	{
		if(pos->device->type == DEVICE_TYPE_BUZZER)
		{
			buzzer = (struct buzzer_t *)(pos->device->driver);
			if(!buzzer)
				continue;

			lua_pushlightuserdata(L, buzzer);
			luaL_setmetatable(L, MT_NAME_HARDWARE_BUZZER);
			lua_setfield(L, -2, pos->device->name);
		}
	}
	return 1;
}

static const luaL_Reg l_hardware_buzzer[] = {
	{"new",		l_buzzer_new},
	{"list",	l_buzzer_list},
	{NULL,	NULL}
};

static int m_buzzer_set_frequency(lua_State * L)
{
	struct buzzer_t * buzzer = luaL_checkudata(L, 1, MT_NAME_HARDWARE_BUZZER);
	int frequency = luaL_checkinteger(L, 2);
	buzzer_set_frequency(buzzer, frequency);
	lua_pushlightuserdata(L, buzzer);
	luaL_setmetatable(L, MT_NAME_HARDWARE_BUZZER);
	return 1;
}

static int m_buzzer_get_frequency(lua_State * L)
{
	struct buzzer_t * buzzer = luaL_checkudata(L, 1, MT_NAME_HARDWARE_BUZZER);
	int frequency = buzzer_get_frequency(buzzer);
	lua_pushinteger(L, frequency);
	return 1;
}

static const luaL_Reg m_hardware_buzzer[] = {
	{"setFrequency",	m_buzzer_set_frequency},
	{"getFrequency",	m_buzzer_get_frequency},
	{NULL,	NULL}
};

int luaopen_hardware_buzzer(lua_State * L)
{
	luaL_newlib(L, l_hardware_buzzer);
	luahelper_create_metatable(L, MT_NAME_HARDWARE_BUZZER, m_hardware_buzzer);
	return 1;
}
