#ifndef NOOB_FISH_H
#define NOOB_FISH_H

/*数据请求ID分配*/
enum DATA_REQUEST_ID {
	DATA_REQUEST,
	CONTROL_REQUEST,
	AIR_PATH_REQUEST,
	CDU_DATA_REQUEST
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

/**
 * @brief 从CDU屏幕读取信息并返回一个24*14的二维结构体数组.
 * 
 * @return PMDG_NG3_CDU_Cell
 */
PMDG_NG3_CDU_Cell** NG3_readCDUScreenContents(PMDG_NG3_CDU_Screen screen);

/**
 * @brief 从CDU屏幕读取信息并返回一个24*14的二维结构体数组.
 *
 * @param cell 代表PMDG737NG3屏幕的24*14二维数组
 * @param print_char 是否以char形式输出结果,false时直接输出数字
 */
void test_NG3_ScreenOutput(PMDG_NG3_CDU_Cell** cell, bool print_char);

#endif


