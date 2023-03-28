#ifndef NOOB_FISH_H
#define NOOB_FISH_H

/*��������ID����*/
enum DATA_REQUEST_ID {
	DATA_REQUEST,
	CONTROL_REQUEST,
	AIR_PATH_REQUEST,
	CDU_DATA_REQUEST
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

/**
 * @brief ��CDU��Ļ��ȡ��Ϣ������һ��24*14�Ķ�ά�ṹ������.
 * 
 * @return PMDG_NG3_CDU_Cell
 */
PMDG_NG3_CDU_Cell** NG3_readCDUScreenContents(PMDG_NG3_CDU_Screen screen);

/**
 * @brief ��CDU��Ļ��ȡ��Ϣ������һ��24*14�Ķ�ά�ṹ������.
 *
 * @param cell ����PMDG737NG3��Ļ��24*14��ά����
 * @param print_char �Ƿ���char��ʽ������,falseʱֱ���������
 */
void test_NG3_ScreenOutput(PMDG_NG3_CDU_Cell** cell, bool print_char);

#endif


