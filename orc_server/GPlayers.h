#ifndef G_GPLAYERS_H
#define G_GPLAYERS_H

#define GRECEIVEMAX 513

typedef struct {
	short		code;
	BYTE		data[256];
} GSTREAM;

typedef struct {
	unsigned long ver_team;
	long haveArm;
	long crush;
	long init;
	long reset;
	long yforce;
	unsigned long base_fps;
	long scenarioCode;
	float dummyf1;
	float dummyf2;
	float dummyf3;
} GMYDATA;


typedef struct {
	char *fin;
	char *dat;
	char *nam;
	int mon, day, hou, min, sec;
} GCHATDATA;

typedef struct {
	signed char x, y, z, w;
} GQuat2;

typedef struct {
	short x, y, z;
} GVector2;

typedef struct {
	short id;
	short color;
	union {
		struct {
			unsigned char type;
			unsigned char option;
			GVector2 pos;
			GQuat2	quat;
		};
		struct {
			float f[3];
		};
	} data;
} GCHIPDATA;

typedef struct {
	MYAPP_PLAYER_INFO info;
	DWORD	size;
	GCHIPDATA	data[GRECEIVEMAX * 2];
} GRECEIVEDATA;

typedef struct {
	int team;
	int haveArm;
	int crush;
	int init;
	int reset;
	int yforce;
	int scenarioCode;

	float x, y, z;
	float w1, w2;
	float ww1, ww2;
	float maxY;
	DWORD time;
	DWORD time2;
	DWORD rtime;
	DWORD rtime2;
	short sendtime;
	short sendtime2;
	DWORD span;
	DWORD span2;
	GRECEIVEDATA	ReceiveData;
	int ChipCount;

	int Jet[GRECEIVEMAX];
	float	X[GRECEIVEMAX];
	float	X2[GRECEIVEMAX];
} GPLAYERDATA;


#endif

