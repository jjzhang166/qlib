// PngStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "PngStatic.h"
#include "MyLayout.h"
#include "ImageManager.h"

//#include "GbkConvert.h"

using namespace Gdiplus;
// CPngStatic

IMPLEMENT_DYNAMIC(CPngStatic, CStatic)

CPngStatic::CPngStatic():m_imageWidth(0),m_imageHeight(0)
{
//	m_imagesManager = NULL;
	m_image = NULL;
	m_IsInWindow = false;
	m_allPngs = NULL;
	m_checked = false;
	m_DecoImage = NULL;
	m_SelectImage = NULL;
	m_showText = true;
	m_isDisable = false;
	m_disableImage = NULL;

	m_textColor = GetSysColor(COLOR_BTNTEXT);
	m_textSize = 11;
}

CPngStatic::~CPngStatic()
{
	if (m_image) delete m_image;
	if (m_SelectImage) delete m_SelectImage;
	if (m_DecoImage) delete m_DecoImage;
}


BEGIN_MESSAGE_MAP(CPngStatic, CStatic)
	ON_WM_PAINT()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_MOUSEMOVE()
	//ON_CONTROL_REFLECT(STN_CLICKED, &CPngStatic::OnStnClicked)
END_MESSAGE_MAP()



// CPngStatic 消息处理程序



using namespace Gdiplus;
// 
// GraphicsPath* MakeRoundRect(Point topLeft, Point bottomRight, INT percentageRounded=10)
// {
// 	ASSERT (percentageRounded >= 1 && percentageRounded <= 100);
// 
// 	INT left  = min(topLeft.X, bottomRight.X);
// 	INT right = max(topLeft.X, bottomRight.X);
// 
// 	INT top    = min(topLeft.Y, bottomRight.Y);
// 	INT bottom = max(topLeft.Y, bottomRight.Y);
// 
// 	INT offsetX = (right-left)*percentageRounded/100; 
// 	INT offsetY = (bottom-top)*percentageRounded/100;
// 
// 	GraphicsPath pt;
// 	GraphicsPath * path = pt.Clone();
// 
// 	path->AddArc(right-offsetX, top, offsetX, offsetY, 270, 90);
// 
// 	path->AddArc(right-offsetX, bottom-offsetY, offsetX, offsetY, 0, 90);
// 
// 	path->AddArc(left, bottom - offsetY, offsetX, offsetY, 90, 90);
// 
// 	path->AddArc(left, top, offsetX, offsetY, 180, 90);
// 
// 	//path->AddLine(left + offsetX, top, right - offsetX/2, top);
// 	path->CloseFigure();
// 
// 	return path;
// }
GraphicsPath* MakeRoundRect(GraphicsPath * path, Point topLeft, Point bottomRight, INT percentageRounded=10)
{
	ASSERT (percentageRounded >= 1 && percentageRounded <= 100);

	INT left  = min(topLeft.X, bottomRight.X);
	INT right = max(topLeft.X, bottomRight.X);

	INT top    = min(topLeft.Y, bottomRight.Y);
	INT bottom = max(topLeft.Y, bottomRight.Y);

	INT offsetX = (right-left)*percentageRounded/100; 
	INT offsetY = (bottom-top)*percentageRounded/100;

	if (offsetY>offsetX) 
		offsetX = offsetY;
	else
		offsetY = offsetX;

#if(0)
	// 原来是这样的，但是四个角的弧度明显不一样
	path->AddArc(right-offsetX, top, offsetX, offsetY, 270, 90);
	path->AddArc(right-offsetX, bottom-offsetY, offsetX, offsetY, 0, 90);
	path->AddArc(left, bottom - offsetY, offsetX, offsetY, 90, 90);
	path->AddArc(left, top, offsetX, offsetY, 180, 90);
#else
	// 只好改成这样的了，勉强使弧度一样。
	path->AddArc(right-offsetX-2, top, offsetX+2, offsetY+2, 270, 90);	// 右上角
	path->AddArc(right-offsetX-8, bottom-offsetY-8, offsetX+8, offsetY+8, 0, 90);	// 右下角
	path->AddArc(left, bottom - offsetY-2, offsetX+2, offsetY+2, 90, 90);		// 左下角
	path->AddArc(left, top, offsetX, offsetY, 180, 90);		// 左上角
#endif
	//path->AddLine(left + offsetX, top, right - offsetX/2, top);
	path->CloseFigure();

	return path;
}

void CPngStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	
	CRect rect;
	GetClientRect(&rect);
	
	//rect.DeflateRect(0,0,3,3);
	//dc.FillSolidRect(rect, RGB(0,0,0));
	
	//Graphics graphics(GetDC()->m_hDC);
	Graphics graphics(dc.m_hDC);
	RECT rc=rect;
	Bitmap bmp(int(rc.right),int(rc.bottom));

	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	Gdiplus::Image *pCurrImage = m_image;

	if (isDisable()){
	
	}

	else if ( CSingleSelectItem::isSelected() )
	{
		//取一张图形
		Rect r(0,0, rc.right, rc.bottom);
		bmpGraphics.DrawImage(m_SelectImage, r, 0,0, m_SelectImage->GetWidth(), m_SelectImage->GetHeight(), UnitPixel);
	}
	else if (m_IsInWindow)
	{
#if(0)
		// 附加图的选择效果
		Rect r(0,0, rc.right, rc.bottom);
		bmpGraphics.DrawImage(m_SelectImage, r, 0,0, m_SelectImage->GetWidth(), m_SelectImage->GetHeight(), UnitPixel);
#endif
		// 更换图的选择效果
		pCurrImage = m_SelectImage;
#if(0)
		
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(255,255,0));
		CPen *oldPen = dc.SelectObject(&pen);
		CBrush brush(RGB(0,255,0));
		CBrush *oldBrush = dc.SelectObject(&brush);
		dc.Rectangle(&rect);
		dc.SelectObject(oldPen);
		dc.SelectObject(oldBrush);
		pen.DeleteObject();
//#else
		//rect.DeflateRect(2,2,2,2);
		COLORREF clrMask = RGB(255,255,255);


		int trans_value = CSingleSelectItem::isSelected()?128:64;
		
		
		Gdiplus::SolidBrush pBrush(Color(trans_value, GetRValue(clrMask), GetGValue(clrMask), GetBValue(clrMask))); // 透明度 128

		GraphicsPath  path;
		rect.OffsetRect(2,2);
		
		MakeRoundRect(&path, Point(rect.left, rect.top), Point(rect.Width(), rect.Height()), 10);
		pBrush.SetColor(Color(trans_value, 192, 192,192));
		bmpGraphics.FillPath(&pBrush, &path);

		//graphics.FillRectangle(pBrush, rect.left, rect.top, rect.right, rect.bottom);
		rect.OffsetRect(-2,-2);

		//rect.OffsetRect(2,2);
		path.Reset();

		MakeRoundRect(&path, Point(rect.left, rect.top), Point(rect.Width(), rect.Height()), 10);
		pBrush.SetColor(Color(trans_value,  GetRValue(clrMask), GetGValue(clrMask), GetBValue(clrMask)));
		bmpGraphics.FillPath(&pBrush, &path);

		//gdiRect.Offset(-2,-2);
		//gdiRect.Inflate(4,4);
	
		//DrawRoundRect(&graphics, gdiRect, Color(255,128,128,128), 6,2);
		
		

		//rect.InflateRect(2,2,2,2);
#endif

	}
	if (pCurrImage == NULL) return;

// 	Graphics graphics(dc.m_hDC); // Create a GDI+ graphics object

	int imageWidth = pCurrImage->GetWidth();
	int imageHeight = pCurrImage->GetHeight();
	if (!imageHeight || !imageWidth) return;

	m_imageWidth = imageWidth;
	m_imageHeight = imageHeight;

#define TEXT_RECT_HEIGHT 20

	if (m_showText)
		rect.bottom -= TEXT_RECT_HEIGHT;
	
	if (imageWidth>rect.Width())
	{
		imageHeight = imageHeight * rect.Width()/imageWidth;
		imageWidth = rect.Width();
	}

	if (imageHeight>rect.Height())
	{
		imageWidth = imageWidth * rect.Height()/imageHeight;
		imageHeight = rect.Height();
	}

	int x = (rect.Width() - imageWidth) /2;
	int y = (rect.Height() - imageHeight)/2;

	bmpGraphics.DrawImage(pCurrImage, x, y, imageWidth, imageHeight);
	

	// 画Checked 图饰
	if (m_checked)
	{
		Rect r(rect.left+2,rect.bottom-20, 20,20);
		
		bmpGraphics.DrawImage(m_DecoImage, r, 0,0, m_DecoImage->GetWidth(), m_DecoImage->GetHeight(), UnitPixel );
	}
	
	// 画文字
	
	if (m_showText) {
		rect.top = rect.bottom + 2;
		rect.bottom = rect.top + TEXT_RECT_HEIGHT-2;
		//CStringW str;




		USES_CONVERSION;
		CStringW str = A2W(getCaption());

		Color textColor=Color::White;
		
		if (isDisable())
			textColor = Color::Gray;
		else if ( isSelected()) 
			textColor = Color::Yellow;
		else
			textColor = (255<<24)|m_textColor;
		


		FontFamily fontFamily(L"宋体"); 
		Gdiplus::Font font(&fontFamily, m_textSize, FontStyleRegular, UnitPoint);  
		StringFormat stringFormat; 
	
		stringFormat.SetAlignment(StringAlignmentCenter);

		SolidBrush solidBrush(textColor);
		
// 		if (!isDisable()){
// 			// 画阴影
// 			Color c =textColor;
// 			c.SetValue(Color::MakeARGB(255, 255-c.GetRed(), 255-c.GetGreen(), 255-c.GetBlue()));
// 			solidBrush.SetColor(c);
// 			rect.OffsetRect(2, 2);
// 			bmpGraphics.DrawString(str, (int)wcslen(str), &font, 
// 				RectF((Gdiplus::REAL)rect.left, (Gdiplus::REAL)rect.top, (Gdiplus::REAL)rect.Width(), (Gdiplus::REAL)rect.Height()), 
// 				&stringFormat, &solidBrush);
// 
// 			rect.OffsetRect(-2,-2);
// 			solidBrush.SetColor(textColor);
// 		}
		
		

		//WCHAR string[] = L"这些文字是垂直居中且是从右到左的阅读次序, 它们是通过  SetFormatFlags和SetAlignment来设置的！";
		bmpGraphics.DrawString(str, (int)wcslen(str), &font, 
			RectF((Gdiplus::REAL)rect.left, (Gdiplus::REAL)rect.top, (Gdiplus::REAL)rect.Width(), (Gdiplus::REAL)rect.Height()), 
			&stringFormat, &solidBrush);
		



	}
	CachedBitmap cachedBmp(&bmp,&graphics);
	graphics.DrawCachedBitmap(&cachedBmp,0,0);
}

/*
void CPngStatic::setImageFile( const CString &szFileName )
{
	if (m_image) {
		delete m_image;
		m_image = NULL;
	}

	m_imageFile = szFileName;
	
	USES_CONVERSION;
	
	m_image = new Image(A2W(m_imageFile));
	
	m_imageWidth = m_image->GetWidth();
	m_imageHeight = m_image->GetHeight();


	Invalidate(FALSE);
}*/

CSize CPngStatic::getImageSize() const
{
	return CSize(m_imageWidth, m_imageHeight);
}

LRESULT CPngStatic::OnMouseLeave( WPARAM,LPARAM )
{
	m_IsInWindow=false;  
	//Add your code
	//TRACE("leave\n");
	if (!isSelected())
		InvalidateParentRect();
	//Invalidate(FALSE);
	return 0;

}

void CPngStatic::OnMouseMove( UINT nFlags, CPoint point )
{

	if (!m_IsInWindow && !isDisable())
	{
		m_IsInWindow = TRUE;
		TRACKMOUSEEVENT trackmouseevent;
		trackmouseevent.cbSize = sizeof(trackmouseevent);
		trackmouseevent.dwFlags = TME_LEAVE;
		trackmouseevent.hwndTrack = GetSafeHwnd();
		trackmouseevent.dwHoverTime = HOVER_DEFAULT;
		_TrackMouseEvent(&trackmouseevent); 

		//handle move in event
		//TRACE("in --- ");
		if (!isSelected() )
			InvalidateParentRect();

		//Invalidate(FALSE);
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CPngStatic::InvalidateParentRect()
{
	CRect rect;
	GetWindowRect(&rect);
	GetParent()->ScreenToClient(&rect);
	GetParent()->InvalidateRect(rect, TRUE);

	if (!m_allPngs) return;
	for (list<CPngStatic*>::iterator i=m_allPngs->begin(); i!=m_allPngs->end();++i)
	{
		(*i)->Invalidate(FALSE);
	}
}

void CPngStatic::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
// 	if (!m_image) {
// 		
// 		delete m_image;
// 		m_image = NULL;
// 	}
// 
// 	if (!m_DecoImage)
// 	{
// 		delete m_DecoImage;
// 		m_DecoImage = NULL;
// 	}

	CStatic::PostNcDestroy();
}

void CPngStatic::OnSelected()
{
	InvalidateParentRect();
	Invalidate(FALSE);
	
}


// void CPngStatic::OnStnClicked()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CWnd *pWnd = GetParent();
// 	CMyLayout *myLayout = dynamic_cast<CMyLayout*>(pWnd);
// 	if (myLayout)
// 		myLayout->OnPngStaticClicked(this);
// }

CString CPngStatic::getCaption() const
{
	return m_caption;
}

void CPngStatic::setCaption( CString val, BOOL bNeedInvalidate )
{
	m_caption = val;
	if (bNeedInvalidate)
	{
		refresh();
	}
	
}

bool CPngStatic::isChecked() const
{
	return m_checked;
}

void CPngStatic::setChecked( bool val )
{
	m_checked = val;

	refresh();
}

// void CPngStatic::setCheckedImage( CString szImage )
// {
// 	if (m_DecoImage)
// 	{
// 		delete m_DecoImage;
// 
// 	}
// 	CStringW wszImage;
// 	USES_CONVERSION;
// 	wszImage = A2W(szImage);
// 	m_DecoImage = new Image(wszImage);
// }

void CPngStatic::setImage( const CImageManager &images, const CString &szKey, BOOL bNeedInvalidate )
{
	if (m_image) delete m_image;
	m_image = images.getImageByKey(szKey);
	if (m_image) {
		m_image = m_image->Clone();
		
		m_imageWidth = m_image->GetWidth();
		m_imageHeight = m_image->GetHeight();
		
		m_imageFile = szKey;

		// 搞一个灰度图片出来
// 		if (m_disableImage){
// 			delete m_disableImage;
// 			
// 			m_disableImage = m_image->Clone();
// 			CImage x;
// 		
// 			
// 		}
	}
	if (bNeedInvalidate){
		refresh();
	}
}

void CPngStatic::setDecoImage( const CImageManager &images, const CString & szKey )
{
	if (m_DecoImage) delete m_DecoImage;
	m_DecoImage = images.getImageByKey(szKey);
	if (m_DecoImage)
		m_DecoImage = m_DecoImage->Clone();
}

void CPngStatic::setSeleImage( const CImageManager &images, const CString & szKey )
{
	if (m_SelectImage) delete m_SelectImage;

	m_SelectImage = images.getImageByKey(szKey);
	if (m_SelectImage)
		m_SelectImage = m_SelectImage->Clone();
}

bool CPngStatic::getShowText() const
{
	return m_showText;
}

void CPngStatic::setShowText( bool val )
{
	m_showText = val;
}

void CPngStatic::setImageAll(const CString & szCaption,  const CImageManager &images, 
	const CString & szKeyDefault, const CString &szKeySele, const CString &szKeyDeco, BOOL bNeedInvalidate/*=TRUE*/ )
{
	setCaption(szCaption);
	setImage(images, szKeyDefault, FALSE);
	setSeleImage(images, szKeySele);
	setDecoImage(images, szKeyDeco);
	if (bNeedInvalidate)
	{
		refresh();
	}

}

void CPngStatic::setDisable( BOOL bDisable/*=TRUE*/, BOOL bNeedInvalidate/*=TRUE*/ )
{
	m_isDisable=bDisable;
	if (bNeedInvalidate)
		refresh();
}

void CPngStatic::refresh()
{
	//GetParent()->LockWindowUpdate();
	InvalidateParentRect();
	Invalidate(FALSE);
	//UpdateWindow();
	//GetParent()->UnlockWindowUpdate();
	
}

BOOL CPngStatic::isDisable()
{
	return m_isDisable;
}


BOOL CPngStatic::PreTranslateMessage( MSG* pMsg )
{
	if (isDisable())
	{
		return TRUE;
	}

	return CStatic::PreTranslateMessage(pMsg);

}

void CPngStatic::setTextColor(COLORREF textColor)
{
	m_textColor = textColor;
}

int CPngStatic::getTextSize() const
{
	return m_textSize;
}

void CPngStatic::setTextSize(int val)
{
	m_textSize = val;
}
