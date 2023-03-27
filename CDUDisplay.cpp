#include <windows.h> 
#include <iostream> 
#include <vector>

#include "PMDG_NG3_SDK.h" 
#include "SimConnect.h"

HANDLE hSimConnect = NULL;
PMDG_NG3_CDU_Screen screen;

enum DATA_REQUEST_ID {
	CDU_DATA_REQUEST
};

bool checkSimConnect()
{
	SIMCONNECT_RECV* pData;
	DWORD cbData;
	HRESULT hr = SimConnect_GetNextDispatch(hSimConnect, &pData, &cbData);
	if (SUCCEEDED(hr))
	{
		if (pData->dwID == SIMCONNECT_RECV_ID_CLIENT_DATA)
		{
			SIMCONNECT_RECV_CLIENT_DATA* pObjData = (SIMCONNECT_RECV_CLIENT_DATA*)pData;
			if (pObjData->dwRequestID == CDU_DATA_REQUEST)
			{
				PMDG_NG3_CDU_Screen* pS = (PMDG_NG3_CDU_Screen*)&pObjData->dwData;
				memcpy(&screen, pS, sizeof(PMDG_NG3_CDU_Screen));
				return true;
			}
		}
	}
	return false;
}

int main() {
	
	HRESULT hr; 
	
	// 连接到MSFS
	hr = SimConnect_Open(&hSimConnect, "PMDG NG3 CDU Test", NULL, 0, 0, 0);						
	// 数据名称-ID关联
	hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_CDU_0_NAME, PMDG_NG3_CDU_0_ID);
	// 定义数据区结构
	hr = SimConnect_AddToClientDataDefinition(hSimConnect,PMDG_NG3_CDU_0_DEFINITION, 0, sizeof(PMDG_NG3_CDU_Screen), 0, 0);
	// Sign up for notification of data change.
	// SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED 参数用于请求数据
	// 这意味着只在CDU界面数据改变时发送
	hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_CDU_0_ID,CDU_DATA_REQUEST, PMDG_NG3_CDU_0_DEFINITION,
		 SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);

	// 一个用来测试的临时存储数组
	PMDG_NG3_CDU_Cell* test_cells[CDU_COLUMNS][CDU_ROWS];

	while (true) { 
		// 接收SimConnect消息 
		SimConnect_CallDispatch(hSimConnect, NULL, NULL); 
		// 检查CDU数据是否有更新 
		if (checkSimConnect()) { 
			// 读取CDU屏幕内容 
			for (int x = 0; x < CDU_COLUMNS; ++x) {
				for (int y = 0; y < CDU_ROWS; ++y) {
					PMDG_NG3_CDU_Cell* cell = &(screen.Cells[x][y]);
					test_cells[x][y] = &(screen.Cells[x][y]);
					char symbol = cell->Symbol;
					bool smallFont = (cell->Flags & PMDG_NG3_CDU_FLAG_SMALL_FONT); // 处理CDU屏幕内容 
				}
			}
			// 测试输出
			for (int i = 0; i < CDU_ROWS; i++) {
				for (int j = 0; j < CDU_COLUMNS; j++) {
					std::cout << test_cells[j][i]->Symbol;
				}
				std::cout << '\n';
			}
			std::cout << "读取成功!" << std::endl;
		}
		Sleep(1000); // 等待一段时间后再次读取CDU屏幕内容 
	}
	SimConnect_Close(hSimConnect);  // 关闭SimConnect连接
	return 0;
}