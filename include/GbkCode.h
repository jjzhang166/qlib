#pragma once
#include "stdafx.h"
#include "qlibdef.h"
class DLL_EXP_QLIB CGbkCode
{
public:
	CGbkCode(void);
	~CGbkCode(void);

	static CString toUtf8(LPCTSTR str);
	static CString fromUtf8(LPCTSTR strUtf8);

	
};

