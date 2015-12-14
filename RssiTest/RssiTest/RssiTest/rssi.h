#include <windows.h>
#include <wlanapi.h>
#include <windot11.h> // for DOT11_SSID struct
#include <ObjBase.h>
#include <WTypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

typedef struct _ap {
	char ssid[256];
	int rssi;
} ap;

typedef struct _apList {
	ap aps[50];
	int nList;
	bool bSuccess;
} apList;

int createRssi(void);
int destroyRssi(void);
apList getRssi(void);
void initAP(void);
int getOptimalAP(void);
