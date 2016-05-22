#include "StdAfx.h"
#include "MyRect.h"


CMyRect::CMyRect(void):CRect(0,0,0,0)
{

}

CMyRect::CMyRect( const CRect &r ):CRect(r)
{

}

CMyRect::CMyRect( int x, int y, int x2, int y2 ):CRect(x,y,x2, y2)
{

}


CMyRect::~CMyRect(void)
{
}

CMyRect CMyRect::getCenterRect( double ratio )
{
	CMyRect ret;
	if (Width()<=0 || Height()<=0) return ret;
	if (ratio<=0) return ret;

	int nWidth = Width();
	int nHeight = Height();

	if (nWidth>=nHeight* ratio)
	{
		// 左右留出空白
		int nNewWidth = nHeight*ratio;
		ret.left = left + (nWidth - nNewWidth)/2;
		ret.right = ret.left + nNewWidth;
		ret.top = top;
		ret.bottom = bottom;
	} else {
		// 上下留出空白
		int nNewHieght = nWidth / ratio;
		ret.left = left;
		ret.right = right;
		ret.top = top + (nHeight-nNewHieght)/2;
		ret.bottom = ret.top + nNewHieght;
	}
	return ret;
}
