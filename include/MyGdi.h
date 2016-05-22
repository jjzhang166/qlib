#include "qlibdef.h"
#include "gdiplus.h"

#ifndef _MYGDI__H_
#define _MYGDI__H_
class DLL_EXP_QLIB CMyGdi
{

public:
	CMyGdi();
	~CMyGdi();

	enum DrawMode{
		dmNormal=0,
		dmLighten,
		dmDisable,
		dmDarken,
	};
	// 居中，如果大太，则缩小
	//static void DrawPng(Gdiplus::Image *pImage, CDC* pDC, const CRect &rect, DrawMode AMode = dmNormal, double rotateAngle=0);
	
	static void DrawPng(Gdiplus::Image *pImage, Graphics* pGraphics, const CRect &rect, DrawMode AMode = dmNormal, double rotateAngle = 0);
	
private:
	ULONG _token;

};
#endif // !_MYGDI__H_
