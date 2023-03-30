#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <strsafe.h>

#include "..\..\inc\SimConnect.h"
#include "..\..\inc\PMDG_NG3_SDK.h"
#include "..\..\inc\Noobfish.h"

#pragma comment(lib, "..\\..\\lib\\MSFS\\SimConnect.lib")

bool requestCDUData();

int main() {
	requestCDUData();
	NG3_run();
	return 0;
}

