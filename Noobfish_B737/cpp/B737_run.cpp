#include <windows.h> 
#include <iostream>

#include "SimConnect.h"
#include "..\header\PMDG_NG3_SDK.h"
#include "..\header\Noobfish.h"

HANDLE  hSimConnect = NULL;
int		quit = 0;					// 模拟器运行状态
bool    AircraftRunning = false;	// 航空器运行状态
PMDG_NG3_CDU_Screen ng3_screen;		// PMDG737NG3屏幕

void CALLBACK NoobFishDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
	switch (pData->dwID)
	{
	case SIMCONNECT_RECV_ID_CLIENT_DATA: // Receive and process the NG3 data block
	{
		SIMCONNECT_RECV_CLIENT_DATA* pObjData = (SIMCONNECT_RECV_CLIENT_DATA*)pData;

		switch (pObjData->dwRequestID)
		{
			/*case DATA_REQUEST:
			{
				PMDG_NG3_Data* pS = (PMDG_NG3_Data*)&pObjData->dwData;
				//ProcessNG3Data(pS);
				break;
			}
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
			memcpy(&ng3_screen, pS, sizeof(PMDG_NG3_CDU_Screen));
			test_NG3_ScreenOutput(NG3_readCDUScreenContents(ng3_screen), true);
		}
		}
		break;
	}
	/*
	case SIMCONNECT_RECV_ID_EVENT:
	{
		SIMCONNECT_RECV_EVENT* evt = (SIMCONNECT_RECV_EVENT*)pData;
		switch (evt->uEventID)
		{
		case EVENT_SIM_START:	// Track aircraft changes
		{
			HRESULT hr = SimConnect_RequestSystemState(hSimConnect, AIR_PATH_REQUEST, "AircraftLoaded");
			break;
		}
		case EVENT_KEYBOARD_A:
		{
			toggleTaxiLightSwitch();
			break;
		}
		case EVENT_KEYBOARD_B:
		{
			toggleLogoLightsSwitch();
			break;
		}
		case EVENT_KEYBOARD_C:
		{
			toggleFlightDirector();
			break;
		}
		case EVENT_KEYBOARD_D:
		{
			slewHeadingSelector();
			break;
		}
		}
		break;
	}
	*/
	/*case SIMCONNECT_RECV_ID_SYSTEM_STATE: // Track aircraft changes
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
	}*/

	case SIMCONNECT_RECV_ID_QUIT:
	{
		quit = 1;
		break;
	}

	default:
		printf("\nReceived:%d", pData->dwID);
		break;
	}
}

bool NG3_run()
{
	HRESULT hr;
	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "NG3_CDU", NULL, 0, 0, 0)))
	{
		printf("\n成功连接到MSFS!");
		// 建立数据连接

		// 关联ID与PMDG数据区名称
		hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_DATA_NAME, PMDG_NG3_DATA_ID);
		hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_CDU_0_NAME, PMDG_NG3_CDU_0_ID);

		// 定义数据区结构
		hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NG3_DATA_DEFINITION, 0, sizeof(PMDG_NG3_Data), 0, 0);
		hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NG3_CDU_0_DEFINITION, 0, sizeof(PMDG_NG3_CDU_Screen), 0, 0);

		// 在数据改变时用于请求数据
		hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_CDU_0_ID, CDU_DATA_REQUEST, PMDG_NG3_CDU_0_DEFINITION,
			SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
		hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_DATA_ID, DATA_REQUEST, PMDG_NG3_DATA_DEFINITION,
			SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);

		// 主循环
		while (quit == 0)
		{
			// 接受和处理NG3数据
			SimConnect_CallDispatch(hSimConnect, NoobFishDispatchProc, NULL);

			Sleep(1);
		}

		hr = SimConnect_Close(hSimConnect);

		return 1;
	}
	else {
		printf("\nMSFS连接失败");
		return 0;
	}
}
