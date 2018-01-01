#include "orc_server.h"
#include "GDPlay.hpp"
#include "GPlayers.h"

#include <math.h>

lua_State *SystemL;

extern GDPlay *DPlay;
extern GMYDATA MyPlayerData;
extern GPLAYERDATA PlayerData[GPLAYERMAX];
extern GPLAYERDATA PrePlayerData[GPLAYERMAX];
extern char LastChatData[256];

int luaGetPlayers(lua_State *L)
{
	lua_pushnumber(L, (double)DPlay->GetNumPlayers());
	return 1;
}
int luaGetPlayerHostID(lua_State *L)
{
	lua_pushnumber(L, (double)DPlay->GetHostPlayerDPNID());
	return 1;
}
int luaGetPlayerMyID(lua_State *L)
{
	lua_pushnumber(L, (double)DPlay->GetLocalPlayerDPNID());
	return 1;
}
int luaGetPlayerID(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	lua_pushnumber(L, (double)PlayerData[n].ReceiveData.info.dpnidPlayer);
	return 1;
}
int luaGetPlayerCrushes(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)MyPlayerData.crush);
		return 1;
	}
	lua_pushnumber(L, (double)PlayerData[n].crush);
	return 1;
}
int luaGetPlayerResets(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)MyPlayerData.reset);
		return 1;
	}
	lua_pushnumber(L, (double)PlayerData[n].reset);
	return 1;
}
int luaGetPlayerInits(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)MyPlayerData.init);
		return 1;
	}
	lua_pushnumber(L, (double)PlayerData[n].init);
	return 1;
}
int luaGetPlayerColor(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	lua_pushnumber(L, (double)PlayerData[n].ReceiveData.info.color);
	return 1;
}
int luaGetPlayerChips(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)1);
		return 1;
	}
	lua_pushnumber(L, (double)PlayerData[n].ChipCount);
	return 1;
}
int luaGetPlayerX(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	double a = pow((double)PlayerData[n].ChipCount, 1.0 / 3.0) / 2.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)0);
		return 1;
	}
	double v = PlayerData[n].x;
	lua_pushnumber(L, v);
	return 1;
}
int luaGetPlayerY(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, -100000.0);
		return 1;
	}
	double a = pow((double)PlayerData[n].ChipCount, 1.0 / 3.0) / 2.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)0);
		return 1;
	}
	double v = PlayerData[n].y;
	lua_pushnumber(L, v);
	return 1;
}
int luaGetPlayerZ(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	double a = pow((double)PlayerData[n].ChipCount, 1.0 / 3.0) / 2.0;
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)0);
		return 1;
	}
	double v = PlayerData[n].z;
	lua_pushnumber(L, v);
	return 1;
}
int luaGetPlayerArms(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)MyPlayerData.haveArm);
		return 1;
	}
	lua_pushnumber(L, (double)PlayerData[n].haveArm);
	return 1;
}
int luaGetPlayerYForces(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushnumber(L, 0.0);
		return 1;
	}
	if (DPlay->GetLocalPlayerDPNID() == PlayerData[n].ReceiveData.info.dpnidPlayer) {
		lua_pushnumber(L, (double)MyPlayerData.yforce);
		return 1;
	}
	lua_pushnumber(L, (double)PlayerData[n].yforce);
	return 1;
}
int luaGetPlayerName(lua_State *L)
{
	int n = (int)lua_tonumber(L, 1);
	if (n<0 || n >= DPlay->GetNumPlayers()) {
		lua_pushstring(L, "");
		return 1;
	}
	lua_pushstring(L, PlayerData[n].ReceiveData.info.strPlayerName);
	return 1;
}

int luaGetLastChat(lua_State *L)
{
	lua_pushstring(L, LastChatData);
	return 1;
}

int luaFileOpen(lua_State *L)
{
	char *name = (char *)lua_tostring(L, 1);
	char *mode = (char *)lua_tostring(L, 2);
	lua_pushnumber(L, (long)fopen(name,mode));
	return 1;
}
int luaFileClose(lua_State *L)
{
	FILE *no = (FILE *)(long)lua_tonumber(L, 1);
	lua_pushnumber(L, fclose(no));
	return 1;
}
int luaFilePuts(lua_State *L)
{
	FILE *no = (FILE *)(long)lua_tonumber(L, 1);
	char *str = (char *)lua_tostring(L, 2);
	int ret = fputs(str, no);
	lua_pushnumber(L, ret);
	return 1;
}
int luaFileGets(lua_State *L)
{
	char buf[1026];
	buf[0] = '\0';
	FILE *no = (FILE *)(long)lua_tonumber(L, 1);
	fgets(buf, 1024, no);
	lua_pushstring(L, buf);
	return 1;
}

int luaSystemInit(char *sz)
{
	SystemL = lua_open();

	luaopen_string(SystemL);
	luaopen_base(SystemL);
	luaopen_table(SystemL);
	luaopen_math(SystemL);

	lua_register(SystemL, "_PLAYERS", luaGetPlayers);
	lua_register(SystemL, "_PLAYERHOSTID", luaGetPlayerHostID);
	lua_register(SystemL, "_PLAYERMYID", luaGetPlayerMyID);
	lua_register(SystemL, "_PLAYERID", luaGetPlayerID);
	lua_register(SystemL, "_PLAYERCHIPS", luaGetPlayerChips);
	lua_register(SystemL, "_PLAYERCRUSHES", luaGetPlayerCrushes);
	lua_register(SystemL, "_PLAYERRESETS", luaGetPlayerResets);
	lua_register(SystemL, "_PLAYERINITS", luaGetPlayerInits);
	lua_register(SystemL, "_PLAYERCOLOR", luaGetPlayerColor);
	lua_register(SystemL, "_PLAYERX", luaGetPlayerX);
	lua_register(SystemL, "_PLAYERY", luaGetPlayerY);
	lua_register(SystemL, "_PLAYERZ", luaGetPlayerZ);
	lua_register(SystemL, "_PLAYERARMS", luaGetPlayerArms);
	lua_register(SystemL, "_PLAYEYFORCES", luaGetPlayerYForces);
	lua_register(SystemL, "_PLAYERNAME", luaGetPlayerName);
	lua_register(SystemL, "_CHAT", luaGetLastChat);
	lua_register(SystemL, "_FOPEN", luaFileOpen);
	lua_register(SystemL, "_FCLOSE", luaFileClose);
	lua_register(SystemL, "_FPUTS", luaFilePuts);
	lua_register(SystemL, "_FGETS", luaFileGets);

	lua_dofile(SystemL, sz);

	return 0;
}

char* w32_printf(const char* format, ...);

int luaSystemCall()
{
	lua_getglobal(SystemL, "OnChat");
	int ret = lua_pcall(SystemL, 0, 0, 0);

	if (ret) w32_printf(lua_tostring(SystemL, -1));

	return ret;
}