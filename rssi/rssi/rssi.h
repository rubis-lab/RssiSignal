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

int createRssi(void);
int destroyRssi(void);
int getRssi(void);