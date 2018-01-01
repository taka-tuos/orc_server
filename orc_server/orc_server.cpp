#include <stdio.h>
#include <stdlib.h>

#include "orc_server.h"

const int GPLAYERMAX = 30;
const int MAX_PLAYER_NAME = 128;

#include "GDPlay.hpp"
#include "GPlayers.h"

GUID g_guidApp = { 0x81a83b95, 0x73b, 0x45b4, { 0xa5, 0x67, 0x81, 0xf, 0x97, 0xf9, 0x9b, 0x77 } };

GDPlay *DPlay;

HRESULT MyReceiveFunc(MYAPP_PLAYER_INFO* playerInfo, DWORD size, BYTE *stream);
HRESULT MyDestroyFunc(MYAPP_PLAYER_INFO* playerInfo);
HRESULT MyCreateFunc(MYAPP_PLAYER_INFO* playerInfo);
HRESULT MyTerminateFunc();

struct SERVER_SETTING {
	int port;
	char *scenario;
};

int ipp_argc(int *i)
{
	if (((*i) + 1) < __argc) (*i)++;
	else return 1;

	return 0;
}

int isdigit_string(char *sz)
{
	for (; *sz; sz++) if(!isdigit(*sz)) return 1;
	return 0;
}

char* w32_printf(const char* format, ...)
{
	va_list ap;
	char sz[1024];

	va_start(ap, format);
	vsprintf(sz, format, ap);
	HANDLE Cons = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD NumberOfCharsWritten;
	WriteConsole(Cons, sz, strlen(sz), &NumberOfCharsWritten, NULL);
	va_end(ap);

	return sz;
}

struct SERVER_SETTING read_settings()
{
	struct SERVER_SETTING ret;

	ret.port = 2345;
	ret.scenario = "stat_v100.rcs";

	if (__argc < 2) return ret;

	int i = 1;

	do {
		if (strcmp(__argv[i], "-sv") == 0) {
			if (ipp_argc(&i)) {
				w32_printf("error : �I�v�V���� [-sv] �̌�Ƀ|�[�g���w�肳��Ă��܂���\n");
				puts("�f�t�H���g�ݒ���g�p���܂�(2345)\n");
			}
			else if (isdigit_string(__argv[i])) {
				w32_printf("error : �I�v�V���� [-sv] �Ŏw�肳�ꂽ�|�[�g(%s)���L���ł͂���܂���\n", __argv[i]);
				w32_printf("�f�t�H���g�ݒ���g�p���܂�(2345)\n");
			}
			else ret.port = atoi(__argv[i]);
		} else if(strcmp(__argv[i], "-rcs") == 0) {
			if (ipp_argc(&i)) {
				w32_printf("error : �I�v�V���� [-rcs] �̌�ɃV�i���I�t�@�C���̖��O���w�肳��Ă��܂���\n");
				w32_printf("�f�t�H���g�ݒ���g�p���܂�(stat_v100.rcs)\n");
			}
			else {
				FILE *fp = fopen(__argv[i], "rt");
				if (fp) {
					fclose(fp);
					ret.scenario = __argv[i];
				}
				else {
					w32_printf("error : �I�v�V���� [-rcs] �Ŏw�肳�ꂽ�V�i���I�t�@�C��(%s)��������܂���\n", __argv[i]);
					w32_printf("�f�t�H���g�ݒ���g�p���܂�(stat_v100.rcs)\n");
				}
			}
		}
	} while (!ipp_argc(&i));

	return ret;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	DPlay = new GDPlay;
	DPlay->Init(hInstance);

	DPlay->SetReceiveFunc(MyReceiveFunc);
	DPlay->SetDestroyFunc(MyDestroyFunc);
	DPlay->SetCreateFunc(MyCreateFunc);
	DPlay->SetTerminateFunc(MyTerminateFunc);

	AllocConsole();

	struct SERVER_SETTING s = read_settings();

	luaSystemInit(s.scenario);


	w32_printf("orc_server 1.0.0\n\n�T�[�o�[���\n\n�|�[�g %d\n�V�i���I�t�@�C���� %s\n", s.port, s.scenario);
	w32_printf("----------------���O----------------\n");

	DPlay->NewSession("RigidChips", "host#00FF00", s.port, g_guidApp);

	while (1) {
		Sleep(20);
	}

	FreeConsole();

	DPlay->End();

	return 0;
}