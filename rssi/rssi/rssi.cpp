#include <windows.h>
#include <wlanapi.h>
#include <windot11.h> // for DOT11_SSID struct
#include <ObjBase.h>
#include <WTypes.h>

#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

int main(void)
{
	FILE* fp = NULL;
	fp = fopen("test.txt", "w");
	
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

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		printf("WlanOpenHandle failed with error: %u\n", dwResult);		
		return 1;		
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		printf("WlanEnumInterfaces failed with error: %u\n", dwResult);
		return 1;		
	}
	else
	{		
		for (int i = 0; i < (int) pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO *) & pIfList->InterfaceInfo[i];			
			iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR) & GuidString, sizeof(GuidString) / sizeof(*GuidString));			
			if (iRet == 0) {
				printf("StringFromGUID2 failed\n");
			}			
		}
		
		while(1) {
			// Query the Interface
			dwResult = WlanQueryInterface(hClient,
				&pIfInfo->InterfaceGuid,
				wlan_intf_opcode_current_connection,					
				NULL,
				&connectInfoSize,
				(PVOID *) &pConnectInfo,					
				&opCode);
			if (dwResult != ERROR_SUCCESS) {
				printf("WlanQueryInterface failed with error: %u\n", dwResult);
				return 0;
			}

			// Scan the connected SSID
			dwResult = WlanScan(hClient,
				&pIfInfo->InterfaceGuid,
				&pConnectInfo->wlanAssociationAttributes.dot11Ssid,
				NULL,
				NULL);
			if (dwResult != ERROR_SUCCESS) {
				printf("WlanScan failed with error: %u\n", dwResult);
				return 0;
			}		

			dwResult = WlanGetAvailableNetworkList(hClient,
				&pIfInfo->InterfaceGuid,
				0,
				NULL,
				&pAvaList);
			if (dwResult != ERROR_SUCCESS) {
				printf("WlanGetAvailableNetworkList failed with error: %u\n", dwResult);
				return 0;
			}

			printf("Number of APs : %lu\n", pAvaList->dwNumberOfItems);
			fprintf(fp, "Number of APs : %lu\n", pAvaList->dwNumberOfItems);
			fflush(fp);
			for(int j = 0; j < pAvaList->dwNumberOfItems; j++)
			{
				pAvaEntry = (WLAN_AVAILABLE_NETWORK *) & pAvaList->Network[j];
				for(int k = 0; k < pAvaEntry->dot11Ssid.uSSIDLength; k++)
				{
					printf("%c", (int) pAvaEntry->dot11Ssid.ucSSID[k]);
					fprintf(fp, "%c", (int) pAvaEntry->dot11Ssid.ucSSID[k]);
					fflush(fp);
				}
				printf("\t");
				fprintf(fp, "\t");
				fflush(fp);

				dwResult = WlanGetNetworkBssList(hClient,
					&pIfInfo->InterfaceGuid,
					&pAvaEntry->dot11Ssid,
					dot11_BSS_type_infrastructure,
					TRUE,
					NULL,
					&pBssList);
				if (dwResult != ERROR_SUCCESS) {
					printf("WlanGetNetworkBssList failed with error: %u\n", dwResult);
					return 0;
				}				

				// Get the RSSI value			
				if (pBssList->dwNumberOfItems > 0) {
					pBssEntry = &pBssList->wlanBssEntries[0];			
					printf("%d dBm\n", pBssEntry->lRssi);
					fprintf(fp, "%d dBm\n", pBssEntry->lRssi);
					fflush(fp);
				} else {
					printf("N/A\n");
					fprintf(fp, "N/A\n");
					fflush(fp);
				}
			}
			printf("\n");
			fprintf(fp, "\n");
			fflush(fp);
		}
	}

	if (fp != NULL)	{
		fclose(fp);
		fp = NULL;
	}
	
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

	return 0;
}