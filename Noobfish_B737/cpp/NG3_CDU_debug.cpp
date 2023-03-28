#include <windows.h> 
#include <iostream>

#include "SimConnect.h"
#include "..\header\PMDG_NG3_SDK.h"
#include "..\header\Noobfish.h"

void test_NG3_ScreenOutput(PMDG_NG3_CDU_Cell** cell, bool print_char) {
	// ≤‚ ‘ ‰≥ˆ
	std::cout << '\n';
	for (int i = 0; i < CDU_ROWS; i++) {
		for (int j = 0; j < CDU_COLUMNS; j++) {
			if (print_char) {
				std::cout << (cell[i][j].Symbol);
			}
			else {
				std::cout << (int)(cell[i][j].Symbol) << ' ';
			}
			
		}
		std::cout << '\n';
	}
}
