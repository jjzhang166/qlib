// qlib.h : qlib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CqlibApp
// �йش���ʵ�ֵ���Ϣ������� qlib.cpp
//

class CqlibApp : public CWinApp
{
public:
	CqlibApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
