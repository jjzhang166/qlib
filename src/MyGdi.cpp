#include "stdafx.h"
#include "MyGdi.h"
#include "MyLog.h"
using namespace Gdiplus;

/*
void CMyGdi::DrawPng(Gdiplus::Image *pImage, CDC* pDC, const CRect &rect, DrawMode AMode, double rotateAngle)
{
	Graphics graphics(pDC->m_hDC);
	DrawPng(pImage, &graphics, rect, AMode, rotateAngle);
}*/

void CMyGdi::DrawPng(Gdiplus::Image *pImage, Graphics* pGraphics, const CRect &rect, DrawMode AMode /*= dmNormal*/, double rotateAngle /*= 0*/)
{
	ASSERT(pImage && pGraphics);

	Gdiplus::Graphics &graphics(*pGraphics);
	RECT rc = rect;
	//Bitmap bmp(int(rc.right), int(rc.bottom));

	//Graphics bmpGraphics(&bmp);

	if (pImage == NULL) return;

	int imageWidth = pImage->GetWidth();
	int imageHeight = pImage->GetHeight();

	int imageOrigWidth = imageWidth;
	int imageOrigHeight = imageHeight;

	if (!imageHeight || !imageWidth) return;

	if (imageWidth > rect.Width())
	{
		imageHeight = imageHeight * rect.Width() / (double)imageWidth;
		imageWidth = rect.Width();
	}

	if (imageHeight > rect.Height())
	{
		imageWidth = imageWidth * rect.Height() / (double)imageHeight;
		imageHeight = rect.Height();
	}

	int x = rect.left + (rect.Width() - imageWidth) / 2;
	int y = rect.top + (rect.Height() - imageHeight) / 2;

	// 得出图像居中区域
	CRect r(x, y, imageWidth + x, imageHeight + y);
	
	// 设置绘制的旋转角度，以图像中心为原点
	Matrix mtr;
	if (rotateAngle != 0)
	{
		//PointF centerPos(rect.CenterPoint().x, rect.CenterPoint().y);
		PointF centerPos(x + r.Width() / 2.0, y + r.Height() / 2.0);
		mtr.Translate(centerPos.X, centerPos.Y);
		mtr.Rotate(rotateAngle);
		mtr.Translate(-centerPos.X, -centerPos.Y);
		graphics.SetTransform(&mtr);
	}

	// 根据灰度、加亮等设置颜色变换矩阵
	switch (AMode)
	{
	case CMyGdi::dmNormal:
		graphics.DrawImage(pImage, x, y, imageWidth, imageHeight);
		break;
	case CMyGdi::dmLighten:
		// 加亮显示
	{
		ColorMatrix HotMat = { 
			1.05f, 0.00f, 0.00f, 0.00f, 0.00f,
			0.00f, 1.05f, 0.00f, 0.00f, 0.00f,
			0.00f, 0.00f, 1.05f, 0.00f, 0.00f,
			0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
			0.05f, 0.05f, 0.05f, 0.00f, 1.00f };

		ImageAttributes ia;
		ia.SetColorMatrix(&HotMat);

		Unit unit;
		Gdiplus::RectF destF(x, y, imageWidth, imageHeight);
		Gdiplus::RectF srcF;
		pImage->GetBounds(&srcF, &unit);

		graphics.DrawImage(pImage, destF, srcF.X, srcF.Y, srcF.Width, srcF.Height, unit, &ia);

		break;
	}
	case CMyGdi::dmDarken:
	{
		ColorMatrix HotMat = { 
			0.70f, 0.00f, 0.00f, 0.00f, 0.00f,
			0.00f, 0.70f, 0.00f, 0.00f, 0.00f,
			0.00f, 0.00f, 0.70f, 0.00f, 0.00f,
			0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
			0.05f, 0.05f, 0.05f, 0.00f, 1.00f };

		ImageAttributes ia;
		ia.SetColorMatrix(&HotMat);

		Unit unit;
		Gdiplus::RectF destF(x, y, imageWidth, imageHeight);
		Gdiplus::RectF srcF;
		pImage->GetBounds(&srcF, &unit);

		graphics.DrawImage(pImage, destF, srcF.X, srcF.Y, srcF.Width, srcF.Height, unit, &ia);

		break;
	}
	case CMyGdi::dmDisable:
	{
		ColorMatrix GrayMat = { 0.30f, 0.30f, 0.30f, 0.00f, 0.00f,
			0.59f, 0.59f, 0.59f, 0.00f, 0.00f,
			0.11f, 0.11f, 0.11f, 0.00f, 0.00f,
			0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
			0.00f, 0.00f, 0.00f, 0.00f, 1.00f };

		ImageAttributes ia;
		ia.SetColorMatrix(&GrayMat);

		Unit unit;
		Gdiplus::RectF destF(x, y, imageWidth, imageHeight);
		Gdiplus::RectF srcF;
		pImage->GetBounds(&srcF, &unit);

		graphics.DrawImage(pImage, destF, srcF.X, srcF.Y, srcF.Width, srcF.Height, unit, &ia);


		break;
	}
	default:
		graphics.DrawImage(pImage, x, y, imageWidth, imageHeight);
		break;
	}

	// 恢复变形参数
	graphics.ResetTransform();
}

CMyGdi::CMyGdi() :_token(0)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&_token, &gdiplusStartupInput, NULL);
}

CMyGdi::~CMyGdi()
{
	Gdiplus::GdiplusShutdown(_token);
}
