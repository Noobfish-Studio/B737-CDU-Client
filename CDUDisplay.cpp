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
	
	// ���ӵ�MSFS
	hr = SimConnect_Open(&hSimConnect, "PMDG NG3 CDU Test", NULL, 0, 0, 0);						
	// ��������-ID����
	hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_CDU_0_NAME, PMDG_NG3_CDU_0_ID);
	// �����������ṹ
	hr = SimConnect_AddToClientDataDefinition(hSimConnect,PMDG_NG3_CDU_0_DEFINITION, 0, sizeof(PMDG_NG3_CDU_Screen), 0, 0);
	// Sign up for notification of data change.
	// SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED ����������������
	// ����ζ��ֻ��CDU�������ݸı�ʱ����
	hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_CDU_0_ID,CDU_DATA_REQUEST, PMDG_NG3_CDU_0_DEFINITION,
		 SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);

	// һ���������Ե���ʱ�洢����
	PMDG_NG3_CDU_Cell* test_cells[CDU_COLUMNS][CDU_ROWS];

	while (true) { 
		// ����SimConnect��Ϣ 
		SimConnect_CallDispatch(hSimConnect, NULL, NULL); 
		// ���CDU�����Ƿ��и��� 
		if (checkSimConnect()) { 
			// ��ȡCDU��Ļ���� 
			for (int x = 0; x < CDU_COLUMNS; ++x) {
				for (int y = 0; y < CDU_ROWS; ++y) {
					PMDG_NG3_CDU_Cell* cell = &(screen.Cells[x][y]);
					test_cells[x][y] = &(screen.Cells[x][y]);
					char symbol = cell->Symbol;
					bool smallFont = (cell->Flags & PMDG_NG3_CDU_FLAG_SMALL_FONT); // ����CDU��Ļ���� 
				}
			}
			// �������
			for (int i = 0; i < CDU_ROWS; i++) {
				for (int j = 0; j < CDU_COLUMNS; j++) {
					std::cout << test_cells[j][i]->Symbol;
				}
				std::cout << '\n';
			}
			std::cout << "��ȡ�ɹ�!" << std::endl;
		}
		Sleep(1000); // �ȴ�һ��ʱ����ٴζ�ȡCDU��Ļ���� 
	}
	SimConnect_Close(hSimConnect);  // �ر�SimConnect����
	return 0;
}