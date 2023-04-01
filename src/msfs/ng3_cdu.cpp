#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <strsafe.h>

#include "..\..\inc\SimConnect.h"
#include "..\..\inc\PMDG_NG3_SDK.h"
#include "..\..\inc\Noobfish.h"

HANDLE  hSimConnect = NULL;
int        quit = 0;                    // 模拟器运行状态
bool    AircraftRunning = false;    // 航空器运行状态
PMDG_NG3_CDU_Screen ng3_screen;        // PMDG737NG3屏幕
PMDG_NG3_Control Control;             // PMDG737NG3控制器

bool NG3_FuelPumpLAftLight = true;
bool NG3_TaxiLightSwitch = false;
bool NG3_LogoLightSwitch = false;

bool NG3_run()
{
    HRESULT hr;
    SetConsoleOutputCP(CP_UTF8);
    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "NG3_CDU", NULL, 0, 0, 0)))
    {
        printf("\nSuccessfully Connected!");

        // 关联ID与PMDG数据区名称
        hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_DATA_NAME, PMDG_NG3_DATA_ID);
        hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_CDU_0_NAME, PMDG_NG3_CDU_0_ID);

        // 定义数据区结构
        hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NG3_DATA_DEFINITION, 0, sizeof(PMDG_NG3_Data), 0, 0);
        hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NG3_CDU_0_DEFINITION, 0, sizeof(PMDG_NG3_CDU_Screen), 0, 0);

        // 在数据改变时用于请求数据
        hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_DATA_ID, DATA_REQUEST, PMDG_NG3_DATA_DEFINITION,
            SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
        hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_CDU_0_ID, CDU_DATA_REQUEST, PMDG_NG3_CDU_0_DEFINITION,
            SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_DEFAULT, 0, 0, 0);

        // 控制事件映射
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_L1, "#70166");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_L2, "#70167");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_L3, "#70168");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_L4, "#70169");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_L5, "#70170");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_L6, "#70171");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_R1, "#70172");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_R2, "#70173");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_R3, "#70174");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_R4, "#70175");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_R5, "#70176");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_R6, "#70177");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_INIT_REF, "#70178");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_RTE, "#70179");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_CLB, "#70180");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_CRZ, "#70181");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_DES, "#70182");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_MENU, "#70183");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_LEGS, "#70184");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_DEP_ARR, "#70185");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_HOLD, "#70186");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_PROG, "#70187");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_EXEC, "#70188");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_N_LIMIT, "#70189");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_FIX, "#70190");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_PREV_PAGE, "#70191");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_NEXT_PAGE, "#70192");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_1, "#70193");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_2, "#70194");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_3, "#70195");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_4, "#70196");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_5, "#70197");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_6, "#70198");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_7, "#70199");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_8, "#70200");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_9, "#70201");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_DOT, "#70202");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_0, "#70203");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_PLUS_MINUS, "#70204");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_A, "#70205");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_B, "#70206");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_C, "#70207");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_D, "#70208");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_E, "#70209");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_F, "#70210");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_G, "#70211");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_H, "#70212");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_I, "#70213");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_J, "#70214");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_K, "#70215");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_L, "#70216");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_M, "#70217");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_N, "#70218");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_O, "#70219");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_P, "#70220");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_Q, "#70221");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_R, "#70222");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_S, "#70223");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_T, "#70224");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_U, "#70225");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_V, "#70226");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_W, "#70227");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_X, "#70228");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_Y, "#70229");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_Z, "#70230");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_SPACE, "#70231");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_DEL, "#70232");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_SLASH, "#70233");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_CLR, "#70234");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CDU_L_BRITENESS, "#70237");


        // 请求当前飞行器.air文件路径
        hr = SimConnect_RequestSystemState(hSimConnect, AIR_PATH_REQUEST, "AircraftLoaded");
        // 订阅模拟器启动或航空器切换事件
        hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");

        // 屏幕初始化(?)
        NG3_pressButton(EVENT_CDU_L_BRITENESS);
        SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CDU_L_BRITENESS, MOUSE_FLAG_RIGHTSINGLE,
            SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
        SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CDU_L_BRITENESS, MOUSE_FLAG_RIGHTRELEASE,
            SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

        // 主循环
        while (quit == 0)
        {
            SimConnect_CallDispatch(hSimConnect, NoobFishDispatchProc, NULL);
            Sleep(1);
        }

        hr = SimConnect_Close(hSimConnect);

        return 1;
    }
    else {
        printf("\nConnection Failed!");
        return 0;
    }
}

void CALLBACK NoobFishDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
    switch (pData->dwID)
    {
        case SIMCONNECT_RECV_ID_CLIENT_DATA:
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
                
                case CONTROL_REQUEST:
                {
                    PMDG_NG3_Control* pS = (PMDG_NG3_Control*)&pObjData->dwData;
                    Control = *pS;
                    break;
                }
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
                case EVENT_SIM_START:
                {
                    HRESULT hr = SimConnect_RequestSystemState(hSimConnect, AIR_PATH_REQUEST, "AircraftLoaded");
                    break;
                }
            }
            break;
        }
        
        case SIMCONNECT_RECV_ID_SYSTEM_STATE:
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

void ProcessNG3Data(PMDG_NG3_Data* pS)
{
    if (pS->FUEL_annunLOWPRESS_Aft[0] != NG3_FuelPumpLAftLight)
    {
        NG3_FuelPumpLAftLight = pS->FUEL_annunLOWPRESS_Aft[0];
        if (NG3_FuelPumpLAftLight)
            printf("\nLOW PRESS LIGHT: [ON]");
        else
            printf("\nLOW PRESS LIGHT: [OFF]");
    }

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

PMDG_NG3_CDU_Screen_Inversed* NG3_readCDUScreenContents(PMDG_NG3_CDU_Screen* screen)
{
    PMDG_NG3_CDU_Screen_Inversed* res = (PMDG_NG3_CDU_Screen_Inversed*)malloc(sizeof(PMDG_NG3_CDU_Screen_Inversed));
    
    for (int x = 0; x < CDU_COLUMNS; x++) 
    {
        for (int y = 0; y < CDU_ROWS; y++) 
            res->Cells[y][x] = screen->Cells[x][y];
    }
    
    return res;
}

void test_NG3_ScreenOutput(PMDG_NG3_CDU_Screen_Inversed* screen, bool print_char) 
{
    std::cout << '\n';    
    
    for (int i = 0; i < CDU_ROWS; i++) {
        for (int j = 0; j < CDU_COLUMNS; j++) {
            if (print_char)
            {
                    std::cout << (screen->Cells[i][j].Symbol);
            }    
            else
                std::cout << (int)screen->Cells[i][j].Symbol << ' ';
        }
        std::cout << '\n';
    }
}

void NG3_pressButton(EVENT_ID event)
{
    SimConnect_TransmitClientEvent(hSimConnect, 0, event, MOUSE_FLAG_LEFTSINGLE,
 		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	SimConnect_TransmitClientEvent(hSimConnect, 0, event, MOUSE_FLAG_LEFTRELEASE,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}