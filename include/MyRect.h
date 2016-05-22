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
	 * 根据已有区域，计算出一个最大填充并且保持比例的居中区域 
	 * ratio: 要求的比例：宽/高
	 */
	CMyRect getCenterRect(double ratio);
};

