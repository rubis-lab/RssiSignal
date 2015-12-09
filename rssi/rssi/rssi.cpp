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
	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	int iRet = 0;

	WCHAR GuidString[39] = { 0 };
		
	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	PWLAN_INTERFACE_INFO pIfConnInfo = NULL;
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
	if(dwResult != ERROR_SUCCESS)
	{
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		getchar();
		return 1;		
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if(dwResult != ERROR_SUCCESS)
	{
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		return 1;		
	}
	else
	{		
		for(int i = 0; i < (int) pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO *) & pIfList->InterfaceInfo[i];			
			iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR) & GuidString, sizeof(GuidString) / sizeof(*GuidString));			
			if(iRet == 0)
			{
				wprintf(L"StringFromGUID2 failed\n");
			}
			
			if(pIfInfo->isState == wlan_interface_state_connected)
			{
				pIfConnInfo = pIfInfo;
				break;
			}
		}

		if(pIfConnInfo == NULL)
		{
			return 0;
		}

		// Query the Interface
		dwResult = WlanQueryInterface(hClient,
			&pIfInfo->InterfaceGuid,
			wlan_intf_opcode_current_connection,					
			NULL,
			&connectInfoSize,
			(PVOID *) &pConnectInfo,					
			&opCode);

		if(dwResult != ERROR_SUCCESS)
		{
			wprintf(L"WlanQueryInterface failed with error: %u\n", dwResult);
			return 0;
		}

		// Scan the connected SSID
		dwResult = WlanScan(hClient,
			&pIfConnInfo->InterfaceGuid,
			&pConnectInfo->wlanAssociationAttributes.dot11Ssid,
			NULL,
			NULL);
		if(dwResult != ERROR_SUCCESS)
		{
			return 0;
		}

		dwResult = WlanGetAvailableNetworkList(hClient,
			&pIfConnInfo->InterfaceGuid,
			0,
			NULL,
			&pAvaList);
		if(dwResult != ERROR_SUCCESS)
		{
			return 0;
		}

		wprintf(L"Num Entries : %lu\n", pAvaList->dwNumberOfItems);

		for(int j = 0; j < pAvaList->dwNumberOfItems; j++)
		{
			pAvaEntry = (WLAN_AVAILABLE_NETWORK *) & pAvaList->Network[j];
			wprintf(L"SSID[%u] : ", j);
			if(pAvaEntry->dot11Ssid.uSSIDLength == 0)
			{
				wprintf(L"\n");
			}
			else
			{   
				for(int k = 0; k < pAvaEntry->dot11Ssid.uSSIDLength; k++)
				{
					wprintf(L"%c", (int) pAvaEntry->dot11Ssid.ucSSID[k]);
				}
				wprintf(L"\n");
			}	

			// Get the BSS Entry		
			dwResult = WlanGetNetworkBssList(hClient,
				&pIfConnInfo->InterfaceGuid,
				&pAvaEntry->dot11Ssid,
				dot11_BSS_type_infrastructure,
				TRUE,
				NULL,
				&pBssList);

			if(dwResult != ERROR_SUCCESS)
			{
				return 0;
			}				

			// Get the RSSI value			
			if(pBssList->dwNumberOfItems > 0)
			{
				pBssEntry = &pBssList->wlanBssEntries[0];			
				wprintf(L"RSSI : %d dBm\n\n", pBssEntry->lRssi);
			}
			else
			{
				wprintf(L"RSSI : N/A\n\n");
			}
		}		
	}
	
	if (pConnectInfo != NULL) 
	{
		WlanFreeMemory(pConnectInfo);
		pConnectInfo = NULL;
	}

	if (pBssList != NULL)
	{
		WlanFreeMemory(pBssList);
		pBssList = NULL;
	}

	if (pAvaList != NULL)
	{
		WlanFreeMemory(pAvaList);
		pAvaList = NULL;
	}

	if (pIfList != NULL)
	{
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}

	return 0;
}