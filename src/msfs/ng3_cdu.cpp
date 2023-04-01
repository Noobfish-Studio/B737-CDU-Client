#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <strsafe.h>

#include "..\..\inc\SimConnect.h"
#include "..\..\inc\PMDG_NG3_SDK.h"
#include "..\..\inc\Noobfish.h"

HANDLE  hSimConnect = NULL;
int		quit = 0;					// ģ��������״̬
bool    AircraftRunning = false;	// ����������״̬
PMDG_NG3_CDU_Screen ng3_screen;		// PMDG737NG3��Ļ

bool NG3_FuelPumpLAftLight = true;
bool NG3_TaxiLightSwitch = false;
bool NG3_LogoLightSwitch = false;

PMDG_NG3_CDU_Screen_Inversed* NG3_readCDUScreenContents(PMDG_NG3_CDU_Screen* screen)
{
	PMDG_NG3_CDU_Screen_Inversed* res = (PMDG_NG3_CDU_Screen_Inversed*)malloc(sizeof(PMDG_NG3_CDU_Screen_Inversed));
	for (int x = 0; x < CDU_COLUMNS; x++) {
		for (int y = 0; y < CDU_ROWS; y++) {
			res->Cells[y][x] = screen->Cells[x][y];
		}
	}
	return res;
}

// This function is called when NG3 data changes
void ProcessNG3Data(PMDG_NG3_Data* pS)
{
	// test the data access:
	// get the state of an annunciator light and display it
	if (pS->FUEL_annunLOWPRESS_Aft[0] != NG3_FuelPumpLAftLight)
	{
		NG3_FuelPumpLAftLight = pS->FUEL_annunLOWPRESS_Aft[0];
		if (NG3_FuelPumpLAftLight)
			printf("\nLOW PRESS LIGHT: [ON]");
		else
			printf("\nLOW PRESS LIGHT: [OFF]");
	}

	// get the state of switches and save it for later use
	if (pS->LTS_TaxiSw != NG3_TaxiLightSwitch)
	{
		NG3_TaxiLightSwitch = pS->LTS_TaxiSw;
		if (NG3_TaxiLightSwitch)
			printf("\nTAXI LIGHTS: [ON]");
		else
			printf("\nTAXI LIGHTS: [OFF]");
	}

	if (pS->LTS_LogoSw != NG3_LogoLightSwitch)
	{
		NG3_LogoLightSwitch = pS->LTS_LogoSw;
		if (NG3_LogoLightSwitch)
			printf("\nLOGO LIGHTS: [ON]");
		else
			printf("\nLOGO LIGHTS: [OFF]");
	}
}

void test_NG3_ScreenOutput(PMDG_NG3_CDU_Screen_Inversed* screen, bool print_char) {
	// �������
	std::cout << '\n';
	for (int i = 0; i < CDU_ROWS; i++) {
		for (int j = 0; j < CDU_COLUMNS; j++) {
			if (print_char) {
				std::cout << (screen->Cells[i][j].Symbol);
			}
			else {
				std::cout << (int)screen->Cells[i][j].Symbol << ' ';
			}
		}
		std::cout << '\n';
	}
}

void CALLBACK NoobFishDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
	switch (pData->dwID)
	{
	case SIMCONNECT_RECV_ID_CLIENT_DATA: // Receive and process the NG3 data block
	{
		SIMCONNECT_RECV_CLIENT_DATA* pObjData = (SIMCONNECT_RECV_CLIENT_DATA*)pData;

		switch (pObjData->dwRequestID)
		{
			case DATA_REQUEST:
			{
				PMDG_NG3_Data* pS = (PMDG_NG3_Data*)&pObjData->dwData;
				ProcessNG3Data(pS);
				break;
			}
			/*
			case CONTROL_REQUEST:
			{
				// keep the present state of Control area to know if the server had received and reset the command
				PMDG_NG3_Control* pS = (PMDG_NG3_Control*)&pObjData->dwData;
				//Control = *pS;
				break;
			}*/
			case CDU_DATA_REQUEST:
			{
				PMDG_NG3_CDU_Screen* pS = (PMDG_NG3_CDU_Screen*)&pObjData->dwData;
				test_NG3_ScreenOutput(NG3_readCDUScreenContents(pS), true);
			}
		}
		break;
	}
	
	case SIMCONNECT_RECV_ID_EVENT:
	{
		SIMCONNECT_RECV_EVENT* evt = (SIMCONNECT_RECV_EVENT*)pData;
		switch (evt->uEventID)
		{
		case EVENT_SIM_START:	// Track aircraft changes
		{
			HRESULT hr = SimConnect_RequestSystemState(hSimConnect, AIR_PATH_REQUEST, "AircraftLoaded");
			hr = SimConnect_RequestDataOnSimObject(hSimConnect, PMDG_NG3_CDU_0_ID, CDU_DATA_REQUEST, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);
			break;
		}
		}
		break;
	}
	
	case SIMCONNECT_RECV_ID_SYSTEM_STATE: // Track aircraft changes
	{
		SIMCONNECT_RECV_SYSTEM_STATE* evt = (SIMCONNECT_RECV_SYSTEM_STATE*)pData;
		if (evt->dwRequestID == AIR_PATH_REQUEST)
		{
			if (strstr(evt->szString, "PMDG 737") != NULL)
				AircraftRunning = true;
			else
				AircraftRunning = false;
		}
		break;
	}

	case SIMCONNECT_RECV_ID_QUIT:
	{
		quit = 1;
		break;
	}

	default:
		//printf("\nReceived:%d", pData->dwID);
		break;
	}
}

bool requestCDUData()
{
	printf("\ncheck0\n");
	HRESULT hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_CDU_0_ID, CDU_DATA_REQUEST, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_CLIENT_DATA_PERIOD_ONCE);
	if (SUCCEEDED(hr))
	{
		printf("\ncheck1\n");
		SIMCONNECT_RECV* pData;
		DWORD cbData;
		hr = SimConnect_GetNextDispatch(hSimConnect, &pData, &cbData);
		if (SUCCEEDED(hr))
		{
			printf("\ncheck2\n");
			if (pData->dwID == SIMCONNECT_RECV_ID_CLIENT_DATA)
			{
				SIMCONNECT_RECV_CLIENT_DATA* pObjData = (SIMCONNECT_RECV_CLIENT_DATA*)pData;
				if (pObjData->dwRequestID == CDU_DATA_REQUEST)
				{
					PMDG_NG3_CDU_Screen* pS = (PMDG_NG3_CDU_Screen*)&pObjData->dwData;
					test_NG3_ScreenOutput(NG3_readCDUScreenContents(pS), true);
					return true;
				}
			}
		}
	}
	return false;
}

bool NG3_run()
{
	HRESULT hr;
	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "NG3_CDU", NULL, 0, 0, 0)))
	{
		printf("\n�ɹ����ӵ�ģ����!");
		// ������������
		// ����ID��PMDG����������
		hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_DATA_NAME, PMDG_NG3_DATA_ID);
		hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_CDU_0_NAME, PMDG_NG3_CDU_0_ID);

		// �����������ṹ
		hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NG3_DATA_DEFINITION, 0, sizeof(PMDG_NG3_Data), 0, 0);
		hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NG3_CDU_0_DEFINITION, 0, sizeof(PMDG_NG3_CDU_Screen), 0, 0);

		// �����ݸı�ʱ������������
		hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_CDU_0_ID, CDU_DATA_REQUEST, PMDG_NG3_CDU_0_DEFINITION,
			SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_DEFAULT, 0, 0, 0);
		hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_DATA_ID, DATA_REQUEST, PMDG_NG3_DATA_DEFINITION,
			SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);

		// 3) Request current aircraft .air file path
		hr = SimConnect_RequestSystemState(hSimConnect, AIR_PATH_REQUEST, "AircraftLoaded");
		// also request notifications on sim start and aircraft change
		hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");

		// ��ѭ��
		while (quit == 0)
		{
			// ���ܺʹ���NG3����
			SimConnect_CallDispatch(hSimConnect, NoobFishDispatchProc, NULL);

			Sleep(1);
		}

		hr = SimConnect_Close(hSimConnect);

		return 1;
	}
	else {
		printf("\nģ��������ʧ��");
		return 0;
	}
}
