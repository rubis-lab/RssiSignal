#include "rssi.h"

void printMenu(void)
{
	printf("====== < M E N U > ======\n");
	printf("1. 1-time measurement\n");
	printf("2. Continuous measurement\n");
	printf("3. Stop measurement\n");
	printf("4. Quit program\n");
	printf("=========================\n");	
}

bool bThread;

DWORD WINAPI thRssiFn(LPVOID lpParam)
{
	while(bThread) {
		getRssi();
	}

	return 0;
}

int main(void)
{
	DWORD dwThreadID;
	HANDLE hThread;
	
	bThread = false;

	int nSelect = 0;
	int nRet = 0;
	bool bQuit = false;
	
	createRssi();

	while(1)
	{
		printMenu();
		printf("Input > ");
		scanf("%d", &nSelect);		
		switch(nSelect)
		{
		case 1:			
			getRssi();	
			break;
		case 2:
			bThread = true;
			hThread = CreateThread(NULL, 0, thRssiFn, NULL, 0, &dwThreadID);
			break;
		case 3:			
			bThread = false;
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			break;
		case 4:
			bQuit = true;
			break;
		default:
			printf("Wrong Input!!! (1-4)\n");
			break;
		}

		if(bQuit)
		{
			break;
		}
	}

	destroyRssi();

	return 0;
}