#include <windows.h> 
#include <iostream>

#include "SimConnect.h"
#include "..\header\PMDG_NG3_SDK.h"
#include "..\header\Noobfish.h"


PMDG_NG3_CDU_Cell** NG3_readCDUScreenContents(PMDG_NG3_CDU_Screen screen)
{
	// ¶ÁÈ¡CDUÆÁÄ»ÄÚÈÝ 
	static PMDG_NG3_CDU_Cell** res_cells = NULL;
	if (res_cells == NULL) {
		res_cells = new PMDG_NG3_CDU_Cell* [CDU_ROWS];
		for (int i = 0; i < CDU_ROWS; i++) {
			res_cells[i] = new PMDG_NG3_CDU_Cell [CDU_COLUMNS];
		}
	}
	for (int x = 0; x < CDU_COLUMNS; x++) {
		for (int y = 0; y < CDU_ROWS; y++) {
			res_cells[y][x] = screen.Cells[x][y];
		}
	}
	return res_cells;
}



