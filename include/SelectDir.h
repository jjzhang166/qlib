#pragma once
#include "qlibdef.h"
class DLL_EXP_QLIB CSelectDir
{
public:
	CSelectDir(void);
	virtual ~CSelectDir(void);

	static bool Show(HWND hParent, CString & szInitDir );
};

