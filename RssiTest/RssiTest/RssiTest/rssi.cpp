#include <stdio.h>
#include "rssi.h"

FILE* fp = NULL;
HANDLE hClient = NULL;
DWORD dwMaxClient = 2;
DWORD dwCurVersion = 0;
DWORD dwResult = 0;
DWORD dwRetVal = 0;
int iRet = 0;

WCHAR GuidString[39] = { 0 };

PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
PWLAN_INTERFACE_INFO pIfInfo = NULL;
PWLAN_BSS_LIST pBssList = NULL;
PWLAN_BSS_ENTRY pBssEntry = NULL;
PWLAN_AVAILABLE_NETWORK_LIST pAvaList = NULL;
PWLAN_AVAILABLE_NETWORK pAvaEntry = NULL;

PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

LONG rssi = 0;
DWORD dwSizeRssi = sizeof(rssi);

time_t ltime;
struct tm *today;

apList aplist;
int nAP;

int createRssi(void)
{
	nAP = 0;
	fp = fopen("result.txt", "a");

	// Open a connection to the server
	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		printf("WlanOpenHandle failed with error: %u\n", dwResult);		
		return 0;			
	}
	
	return 1;
}

int destroyRssi(void)
{
	if (fp != NULL)	{
		fclose(fp);
		fp = NULL;
	}

	dwResult = WlanCloseHandle(hClient, NULL);
	if (dwResult != ERROR_SUCCESS) {
		printf("WlanCloseHandle failed with error : %u\n", dwResult);
		return 0;
	}
}


apList getRssi(void)
{	
	aplist.bSuccess = true;
	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		printf("WlanEnumInterfaces failed with error: %u\n", dwResult);
		aplist.bSuccess = false;
		return aplist;		
	}

	for (int i = 0; i < (int) pIfList->dwNumberOfItems; i++) {
		pIfInfo = (WLAN_INTERFACE_INFO *) & pIfList->InterfaceInfo[i];			
		iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR) & GuidString, sizeof(GuidString) / sizeof(*GuidString));			
		if (iRet == 0) {
			printf("StringFromGUID2 failed\n");
		}			
	}

	// Query various parameters of a specified interface
	dwResult = WlanQueryInterface(hClient,
		&pIfInfo->InterfaceGuid,
		wlan_intf_opcode_current_connection,					
		NULL,
		&connectInfoSize,
		(PVOID *) &pConnectInfo,					
		&opCode);
	if (dwResult != ERROR_SUCCESS) {
		printf("WlanQueryInterface failed with error: %u\n", dwResult);
		aplist.bSuccess = false;
		return aplist;
	}

	// Request a scan for available networks on the indicated interface
	dwResult = WlanScan(hClient,
		&pIfInfo->InterfaceGuid,
		&pConnectInfo->wlanAssociationAttributes.dot11Ssid,
		NULL,
		NULL);
	if (dwResult != ERROR_SUCCESS) {
		//printf("WlanScan failed with error: %u\n", dwResult);
		aplist.bSuccess = false;
		return aplist;
	}		

	// Retrieves the list of available networks on a wireless LAN interface
	dwResult = WlanGetAvailableNetworkList(hClient,
		&pIfInfo->InterfaceGuid,
		0,
		NULL,
		&pAvaList);
	if (dwResult != ERROR_SUCCESS) {
		printf("WlanGetAvailableNetworkList failed with error: %u\n", dwResult);
		aplist.bSuccess = false;
		return aplist;
	}

	time(&ltime);
	today = localtime(&ltime);
	printf("%04d-%02d-%02d %02d:%02d:%02d\n", today->tm_year + 1900, today->tm_mon + 1, today->tm_mday,
		today->tm_hour,	today->tm_min, today->tm_sec);
	fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d\n", today->tm_year + 1900, today->tm_mon + 1, today->tm_mday,
		today->tm_hour,	today->tm_min, today->tm_sec);
	fflush(fp);

	printf("Number of APs : %lu\n", pAvaList->dwNumberOfItems);
	fprintf(fp, "Number of APs : %lu\n", pAvaList->dwNumberOfItems);
	fflush(fp);
	nAP = pAvaList->dwNumberOfItems;
	aplist.nList = nAP;
	for(int j = 0; j < pAvaList->dwNumberOfItems; j++)
	{
		pAvaEntry = (WLAN_AVAILABLE_NETWORK *) & pAvaList->Network[j];	
		for(int k = 0; k < pAvaEntry->dot11Ssid.uSSIDLength; k++)
		{
			printf("%c", (int) pAvaEntry->dot11Ssid.ucSSID[k]);
			aplist.aps[j].ssid[k] = (int) pAvaEntry->dot11Ssid.ucSSID[k];
			fprintf(fp, "%c", (int) pAvaEntry->dot11Ssid.ucSSID[k]);
			fflush(fp);
		}
		printf("\t");
		fprintf(fp, "\t");
		fflush(fp);

		// retrieves a list of the basic service set (BSS) entries of the wireless or networks on a given interface
		dwResult = WlanGetNetworkBssList(hClient,
			&pIfInfo->InterfaceGuid,
			&pAvaEntry->dot11Ssid,
			dot11_BSS_type_infrastructure,
			TRUE,
			NULL,
			&pBssList);
		if (dwResult != ERROR_SUCCESS) {
			printf("WlanGetNetworkBssList failed with error: %u\n", dwResult);
			aplist.bSuccess = false;
			return aplist;
		}				

		// Get the RSSI value			
		if (pBssList->dwNumberOfItems > 0) {
			pBssEntry = &pBssList->wlanBssEntries[0];
			aplist.aps[j].rssi = (int)pBssEntry->lRssi;
			printf("%d dBm\n", pBssEntry->lRssi);
			fprintf(fp, "%d dBm\n", pBssEntry->lRssi);
			fflush(fp);
		} else {
			aplist.aps[j].rssi = -100;
			printf("N/A\n");
			fprintf(fp, "N/A\n");
			fflush(fp);
		}		
	}
	printf("\n");
	fprintf(fp, "\n");
	fflush(fp);
	
	if (pConnectInfo != NULL) {
		WlanFreeMemory(pConnectInfo);
		pConnectInfo = NULL;
	}

	if (pBssList != NULL) {
		WlanFreeMemory(pBssList);
		pBssList = NULL;
	}

	if (pAvaList != NULL) {
		WlanFreeMemory(pAvaList);
		pAvaList = NULL;
	}

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}	
	return aplist;
}

void initAP(void)
{
	for(int i = 0; i < 50; i++)
	{
		aplist.aps[i].rssi = -100;		
	}
}

int getOptimalAP(void)
{
	int maxRssi = -100;
	int maxSsid = nAP - 1;
	for(int i = 0; i < nAP; i++)
	{
		if(aplist.aps[i].rssi > maxRssi)
		{
			maxRssi = aplist.aps[i].rssi;
			maxSsid = i;
		}
	}

	return maxSsid;
}