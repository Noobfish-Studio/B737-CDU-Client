#ifndef NOOB_FISH_H
#define NOOB_FISH_H

#include<vector>
#include<string>
#include<fstream>

/*��������ID����*/
enum DATA_REQUEST_ID {
	DATA_REQUEST,
	CONTROL_REQUEST,
	AIR_PATH_REQUEST,
	CDU_DATA_REQUEST
};

enum EVENT_ID {
	EVENT_SIM_START,	// used to track the loaded aircraft

	EVENT_LOGO_LIGHT_SWITCH,
	EVENT_FLIGHT_DIRECTOR_SWITCH,
	EVENT_HEADING_SELECTOR
};

//-----------------------------------------
//
//             CDU��ʼ��ʾ����
//
//-----------------------------------------

class CDUInitDisplay
{
private:
	std::ifstream init;
	std::vector<std::string> init_lines;
public:
	CDUInitDisplay();
	void init_page();
	void connection_failed();
	void connection_success();
};

//-----------------------------------------
//
//             �������к���
//
//-----------------------------------------

/**
 * @brief ͨ��simconnect���ӵ�PMDG737NG3.
 */
bool NG3_run();

//-----------------------------------------
//
//             �����غ���
//
//-----------------------------------------

/**
 * @brief ���������ݱ仯ʱ���صĻص�����.
 */
void CALLBACK NoobFishDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);


//-----------------------------------------
//
//             CDU ��ȡ����
//
//-----------------------------------------

struct PMDG_NG3_CDU_Screen_Inversed
{
	PMDG_NG3_CDU_Cell Cells[CDU_ROWS][CDU_COLUMNS];
	bool Powered;							// true if the CDU is powered
};

void ProcessNG3Data(PMDG_NG3_Data* pS);

/**
 * @brief ��CDU��Ļ��ȡ��Ϣ������һ��24*14�Ķ�ά�ṹ������.
 * 
 * @return PMDG_NG3_CDU_Cell
 */
PMDG_NG3_CDU_Screen_Inversed* NG3_readCDUScreenContents(PMDG_NG3_CDU_Screen* screen);

/**
 * @brief �����ȡ��������.
 *
 * @param cell ����PMDG737NG3��Ļ��24*14��ά����
 * @param print_char �Ƿ���char��ʽ������,falseʱֱ���������
 */
void test_NG3_ScreenOutput(PMDG_NG3_CDU_Screen_Inversed* screen, bool print_char);

#endif


