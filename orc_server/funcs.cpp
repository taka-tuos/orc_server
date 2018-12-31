#include "GDPlay.hpp"
#include "GPlayers.h"

#include "orc_server.h"

#include <stdio.h>
#include <tchar.h>

#define	GMAXCHATLINE 101
char ChatData[GMAXCHATLINE][256];
char LastChatData[256];
int  ChatDataStart = 0;
int  ChatDataEnd = 0;

GCHATDATA LastChatInfo;

extern GDPlay *DPlay;
GMYDATA MyPlayerData;
GPLAYERDATA PlayerData[GPLAYERMAX];
GPLAYERDATA PrePlayerData[GPLAYERMAX];

GCHATDATA ChatDataDisp(char *name, char *s)
{
	GCHATDATA ret;
	char str[256 * GMAXCHATLINE];
	char name2[MAX_PLAYER_NAME];
	strcpy(name2, name);
	//日付と時間を調べる
	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR* strLastSharp = _tcsrchr(name2, TEXT('#'));
	if (strLastSharp) *strLastSharp = '\0';
	sprintf(ChatData[ChatDataEnd], "%02d/%02d %02d:%02d:%02d %s>%s\r\n", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, name2, s);
	ret.fin = ChatData[ChatDataEnd];
	ret.dat = (char *)malloc(strlen(s)+1);
	strcpy(ret.dat, s);
	ret.mon = st.wMonth;
	ret.day = st.wDay;
	ret.hou = st.wHour;
	ret.min = st.wMinute;
	ret.sec = st.wSecond;
	ret.nam = (char *)malloc(strlen(name2) + 1);
	strcpy(ret.nam, name2);
	ChatDataEnd++;
	if (ChatDataEnd >= GMAXCHATLINE) {
		ChatDataEnd = 0;
	}
	if (ChatDataEnd <= ChatDataStart) {
		ChatDataStart = ChatDataEnd + 1;
	}
	ChatData[ChatDataEnd][0] = '\0';
	int i = ChatDataStart; if (i >= GMAXCHATLINE) i = 0;
	str[0] = '\0';
	while (i != ChatDataEnd) {
		strcat(str, ChatData[i]);
		i++;
		if (i >= GMAXCHATLINE) i = 0;
	}
	return ret;
}

HRESULT MyReceiveFunc(MYAPP_PLAYER_INFO* playerInfo, DWORD size, BYTE *stream) {

	HRESULT hr = S_OK;
	BYTE *strm = new BYTE[size];
	memcpy(strm, stream, size); //いったんコピー
	BYTE *data = strm + sizeof(short);
	short code = *((short*)strm);
	
	if (code == 1) { //チャットデータ
		HANDLE Cons = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD NumberOfCharsWritten;
		LastChatInfo = ChatDataDisp(playerInfo->strPlayerName, (char *)data);
		//WriteConsole(Cons, LastChatInfo.fin, strlen(LastChatInfo.fin), &NumberOfCharsWritten, NULL);
		printf(LastChatInfo.fin);
		strcpy(LastChatData, LastChatInfo.fin);
		luaSystemCall();
	}
	else if (code == 62) {
		GSTREAM strm2;
		DWORD *w = (DWORD*)data;
		w[1] = timeGetTime();

		strm2.code = 63;
		DWORD *sw = (DWORD*)strm2.data;
		sw[0] = w[0];
		sw[1] = w[1];
		DWORD size = sizeof(DWORD) * 4 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 63) {
		GSTREAM strm2;
		DWORD *w = (DWORD*)data;
		w[2] = timeGetTime();

		strm2.code = 64;
		DWORD *sw = (DWORD*)strm2.data;
		sw[0] = w[0];
		sw[1] = w[1];
		sw[2] = w[2];
		DWORD size = sizeof(DWORD) * 4 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 64) {
		GSTREAM strm2;
		DWORD *w = (DWORD*)data;
		w[3] = timeGetTime();

		strm2.code = 65;
		DWORD *sw = (DWORD*)strm2.data;
		sw[0] = w[0];
		sw[1] = w[1];
		sw[2] = w[2];
		sw[3] = w[3];
		DWORD size = sizeof(DWORD) * 4 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 50) { //Land
		GSTREAM strm2;
		strm2.code = 1;
		char *str = (char*)strm2.data;
		sprintf(str, "Land=%s(%X)", "Unknown.x", 0x55aa55aa);
		DWORD size = strlen(str) + 1 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 51) { //Scenario
		GSTREAM strm2;
		strm2.code = 1;
		char *str = (char*)strm2.data;
		sprintf(str, "Scenario=%s(%X)", "Unknown.rcs", 0x55aa55aa);
		DWORD size = strlen(str) + 1 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 52) { //Version
		GSTREAM strm2;
		strm2.code = 1;
		char *str = (char*)strm2.data;
		sprintf(str, "Version=0.1.0-SERVER_CUI");
		DWORD size = strlen(str) + 1 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 53) { //Position
		GSTREAM strm2;
		strm2.code = 1;
		char *str = (char*)strm2.data;
		sprintf(str, "Position=X:%.1f Y:%.1f Z:%.1f ", 0.0f, 0.0f, 0.0f);
		DWORD size = strlen(str) + 1 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 54) { //Name
		GSTREAM strm2;
		strm2.code = 1;
		char *str = (char*)strm2.data;
		char buf[256];
		DPlay->GetPlayersName(DPlay->GetLocalPlayerDPNID(), buf);
		sprintf(str, "Name=%s", buf);
		DWORD size = strlen(str) + 1 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 57) { //FPS
		GSTREAM strm2;
		strm2.code = 1;
		char *str = (char*)strm2.data;
		sprintf(str, "FPS=+inf (Base=+inf) ");
		DWORD size = strlen(str) + 1 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 58) { //OS
		GSTREAM strm2;
		strm2.code = 1;
		char *str = (char*)strm2.data;
		OSVERSIONINFO version;
		memset(&version, 0, sizeof(version));
		version.dwOSVersionInfoSize = sizeof(version);
		GetVersionEx(&version);
		sprintf(str, "OS=Major:%d,Minor:%d,Build:%d,ID:%d,CSD:%s",
			version.dwMajorVersion, version.dwMinorVersion, version.dwBuildNumber, version.dwPlatformId, version.szCSDVersion);
		DWORD size = strlen(str) + 1 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 59) { //Chips
		GSTREAM strm2;
		strm2.code = 1;
		char *str = (char*)strm2.data;
		sprintf(str, "Chips=?");
		DWORD size = strlen(str) + 1 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	else if (code == 60) { //Player num
		GSTREAM strm2;
		strm2.code = 1;
		char *str = (char*)strm2.data;
		sprintf(str, "Players=%d ", DPlay->GetNumPlayers());
		DWORD size = strlen(str) + 1 + sizeof(short);
		DPlay->SendTo(playerInfo->dpnidPlayer, (BYTE*)&strm2, size, 180, DPNSEND_NOLOOPBACK | DPNSEND_NOCOMPLETE);
	}
	delete strm;
	return S_OK;
}

HRESULT MyDestroyFunc(MYAPP_PLAYER_INFO* playerInfo) {
	HRESULT hr = S_OK;
	for (int i = 0; i<DPlay->GetMaxPlayers(); i++) {
		PlayerData[i].ReceiveData.info.dpnidPlayer = 0;
		PlayerData[i].ReceiveData.size = 0;
	}
	return S_OK;
}

HRESULT MyCreateFunc(MYAPP_PLAYER_INFO* playerInfo) {
	HRESULT hr = S_OK;

	DPNID dpnid[GPLAYERMAX];
	int n = DPlay->GetPlayersDPID(dpnid);
	for (int i = 0; i<DPlay->GetMaxPlayers(); i++) {
		PlayerData[i].ReceiveData.size = 0;
		if (i<n) {
			if (playerInfo->dpnidPlayer == dpnid[i]) {
				PlayerData[i].crush = 0;
				PlayerData[i].init = 0;
				PlayerData[i].reset = 0;
				PlayerData[i].yforce = 0;
			}
			PlayerData[i].ReceiveData.info.dpnidPlayer = dpnid[i];
			DPlay->GetPlayersName(dpnid[i], PlayerData[i].ReceiveData.info.strPlayerName);
			TCHAR* strLastSharp = _tcsrchr(PlayerData[i].ReceiveData.info.strPlayerName, TEXT('#'));
			if (strLastSharp == NULL) {
				PlayerData[i].ReceiveData.info.color = 0xffffff;
			}
			else {
				sscanf(strLastSharp + 1, "%x", &PlayerData[i].ReceiveData.info.color);
				*strLastSharp = '\0';
			}
		}
		else {
			PlayerData[i].ReceiveData.info.dpnidPlayer = 0;
			PlayerData[i].ReceiveData.info.strPlayerName[0] = '0';
		}
	}
	return S_OK;
}

HRESULT MyTerminateFunc() {
	HRESULT hr = S_OK;
	DPNID dpnid[GPLAYERMAX];
	int n = DPlay->GetPlayersDPID(dpnid);
	for (int i = 0; i<DPlay->GetMaxPlayers(); i++) {
		PlayerData[i].ReceiveData.size = 0;
		if (i<n) {
			PlayerData[i].ReceiveData.info.dpnidPlayer = dpnid[i];
			DPlay->GetPlayersName(dpnid[i], PlayerData[i].ReceiveData.info.strPlayerName);
			TCHAR* strLastSharp = _tcsrchr(PlayerData[i].ReceiveData.info.strPlayerName, TEXT('#'));
			if (strLastSharp == NULL) {
				PlayerData[i].ReceiveData.info.color = 0xffffff;
			}
			else {
				sscanf(strLastSharp + 1, "%x", &PlayerData[i].ReceiveData.info.color);
				*strLastSharp = '\0';
			}
		}
		else {
			PlayerData[i].ReceiveData.info.dpnidPlayer = 0;
			PlayerData[i].ReceiveData.info.strPlayerName[0] = '0';
		}
	}
	return S_OK;
}