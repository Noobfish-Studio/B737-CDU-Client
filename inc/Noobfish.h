#ifndef NOOB_FISH_H
#define NOOB_FISH_H

#include<vector>
#include<string>
#include<fstream>

/*数据请求ID分配*/
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
//             CDU初始显示界面
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
//             连接运行函数
//
//-----------------------------------------

/**
 * @brief 通过simconnect连接到PMDG737NG3.
 */
bool NG3_run();

//-----------------------------------------
//
//             主调控函数
//
//-----------------------------------------

/**
 * @brief 用于在数据变化时调控的回调函数.
 */
void CALLBACK NoobFishDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);


//-----------------------------------------
//
//             CDU 读取函数
//
//-----------------------------------------

struct PMDG_NG3_CDU_Screen_Inversed
{
	PMDG_NG3_CDU_Cell Cells[CDU_ROWS][CDU_COLUMNS];
	bool Powered;							// true if the CDU is powered
};

void ProcessNG3Data(PMDG_NG3_Data* pS);

/**
 * @brief 从CDU屏幕读取信息并返回一个24*14的二维结构体数组.
 * 
 * @return PMDG_NG3_CDU_Cell
 */
PMDG_NG3_CDU_Screen_Inversed* NG3_readCDUScreenContents(PMDG_NG3_CDU_Screen* screen);

/**
 * @brief 输出读取到的数组.
 *
 * @param cell 代表PMDG737NG3屏幕的24*14二维数组
 * @param print_char 是否以char形式输出结果,false时直接输出数字
 */
void test_NG3_ScreenOutput(PMDG_NG3_CDU_Screen_Inversed* screen, bool print_char);

#endif


