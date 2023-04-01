/**
 * @file Noobfish.h
 * @author 738NGX
 * @brief For Noobfish Product
 * @date 2023-03-31
 * 
 * @copyright Copyright (c) 2023 Noobfish Studio
 * 
 */
#ifndef NOOB_FISH_H
#define NOOB_FISH_H

#include<vector>
#include<string>
#include<fstream>

/**
 * @brief 数据区请求ID分配
 * 
 */
enum DATA_REQUEST_ID {
	DATA_REQUEST,
	CONTROL_REQUEST,
	AIR_PATH_REQUEST,
	CDU_DATA_REQUEST
};

/**
 * @brief 事件ID分配
 * 
 */
enum EVENT_ID {
	EVENT_SIM_START,	

	// CDU按钮
	
	EVENT_CDU_L_L1,
	EVENT_CDU_L_L2,
	EVENT_CDU_L_L3,
	EVENT_CDU_L_L4,
	EVENT_CDU_L_L5,
	EVENT_CDU_L_L6,
	EVENT_CDU_L_R1,
	EVENT_CDU_L_R2,
	EVENT_CDU_L_R3,
	EVENT_CDU_L_R4,
	EVENT_CDU_L_R5,
	EVENT_CDU_L_R6,
	EVENT_CDU_L_INIT_REF,
	EVENT_CDU_L_RTE,
	EVENT_CDU_L_CLB,
	EVENT_CDU_L_CRZ,
	EVENT_CDU_L_DES,
	EVENT_CDU_L_MENU,
	EVENT_CDU_L_LEGS,
	EVENT_CDU_L_DEP_ARR,
	EVENT_CDU_L_HOLD,
	EVENT_CDU_L_PROG,
	EVENT_CDU_L_EXEC,
	EVENT_CDU_L_N_LIMIT,
	EVENT_CDU_L_FIX,
	EVENT_CDU_L_PREV_PAGE,
	EVENT_CDU_L_NEXT_PAGE,
	EVENT_CDU_L_1,
	EVENT_CDU_L_2,
	EVENT_CDU_L_3,
	EVENT_CDU_L_4,
	EVENT_CDU_L_5,
	EVENT_CDU_L_6,
	EVENT_CDU_L_7,
	EVENT_CDU_L_8,
	EVENT_CDU_L_9,
	EVENT_CDU_L_DOT,
	EVENT_CDU_L_0,
	EVENT_CDU_L_PLUS_MINUS,
	EVENT_CDU_L_A,
	EVENT_CDU_L_B,
	EVENT_CDU_L_C,
	EVENT_CDU_L_D,
	EVENT_CDU_L_E,
	EVENT_CDU_L_F,
	EVENT_CDU_L_G,
	EVENT_CDU_L_H,
	EVENT_CDU_L_I,
	EVENT_CDU_L_J,
	EVENT_CDU_L_K,
	EVENT_CDU_L_L,
	EVENT_CDU_L_M,
	EVENT_CDU_L_N,
	EVENT_CDU_L_O,
	EVENT_CDU_L_P,
	EVENT_CDU_L_Q,
	EVENT_CDU_L_R,
	EVENT_CDU_L_S,
	EVENT_CDU_L_T,
	EVENT_CDU_L_U,
	EVENT_CDU_L_V,
	EVENT_CDU_L_W,
	EVENT_CDU_L_X,
	EVENT_CDU_L_Y,
	EVENT_CDU_L_Z,
	EVENT_CDU_L_SPACE,
	EVENT_CDU_L_DEL,
	EVENT_CDU_L_SLASH,
	EVENT_CDU_L_CLR,
	EVENT_CDU_L_BRITENESS,
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
//               CDU上位机
//
//-----------------------------------------

/*----------------基础配置------------------*/

/**
 * @brief PMDG_NG3_CDU_Screen的转置矩阵
 * 
 */
struct PMDG_NG3_CDU_Screen_Inversed
{
	PMDG_NG3_CDU_Cell Cells[CDU_ROWS][CDU_COLUMNS];
	bool Powered;
};

/**
 * @brief 通过simconnect连接到PMDG 737 NG3,PMDG 737 NGXu经过测试同样适用于此函数
 */
bool NG3_run();

/**
 * @brief 用于在数据变化时调控的回调函数
 */
void CALLBACK NoobFishDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);


/*----------------读取数据------------------*/


/**
 * @brief 读取处理PMDG 737 NG3的数据,PMDG 737 NGXu经过测试同样适用于此函数.
 * 
 * @param pS 
 */
void ProcessNG3Data(PMDG_NG3_Data* pS);

/**
 * @brief 从CDU屏幕读取信息并返回一个24*14的二维结构体数组
 * 
 * @return PMDG_NG3_CDU_Cell
 */
PMDG_NG3_CDU_Screen_Inversed* NG3_readCDUScreenContents(PMDG_NG3_CDU_Screen* screen);

/**
 * @brief 输出读取到的数组
 *
 * @param screen 
 * @param print_char 是否以char形式输出结果,false时直接输出数字
 * @note 测试函数
 */
void test_NG3_ScreenOutput(PMDG_NG3_CDU_Screen_Inversed* screen, bool print_char);

/*----------------写入控制------------------*/

void NG3_pressButton(EVENT_ID event);

#endif
