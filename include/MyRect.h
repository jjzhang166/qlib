#pragma once
#include "atltypes.h"
#include "qlibdef.h"

class DLL_EXP_QLIB CMyRect :	public CRect
{
public:
	CMyRect(void);
	CMyRect(const CRect &r);
	CMyRect(int x, int y, int x2, int y2);

	~CMyRect(void);

	/** 
	 * �����������򣬼����һ�������䲢�ұ��ֱ����ľ������� 
	 * ratio: Ҫ��ı�������/��
	 */
	CMyRect getCenterRect(double ratio);
};

