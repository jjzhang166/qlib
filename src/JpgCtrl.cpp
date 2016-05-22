// JpgCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "JpgCtrl.h"
#include "MyLog.h"
#include <gdiplus.h>


// CJpgCtrl

IMPLEMENT_DYNAMIC(CJpgCtrl, CStatic)

CJpgCtrl::CJpgCtrl():m_data(NULL), m_hasPicture(false)
{
	m_isMouseIn = false;
	m_lockRatio = true;
	m_flood = false;
	m_fill = true;
	m_showFocus = false;
	m_backColor = RGB(128,128,128);
}

CJpgCtrl::~CJpgCtrl()
{
	closePicture();
}


BEGIN_MESSAGE_MAP(CJpgCtrl, CStatic)
	//ON_CONTROL_REFLECT(STN_CLICKED, &CJpgCtrl::OnStnClicked)
	//ON_CONTROL_REFLECT(STN_DBLCLK, &CJpgCtrl::OnStnDblclick)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CJpgCtrl 消息处理程序



BOOL CJpgCtrl::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

/*
void CJpgCtrl::OnStnClicked()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (!hasPicture()) return;

	::SendMessage(GetParent()->m_hWnd, WM_JPG_CLICK, 0, (LPARAM)this);
}*/

/*
void CJpgCtrl::OnStnDblclick()
{
	//if (!hasPicture()) return;

	::SendMessage(GetParent()->m_hWnd, WM_JPG_CLICK, 1, (LPARAM)this);
}*/

void CJpgCtrl::SetPicture( LPCTSTR szFileName, int kIndex, LPARAM AData )
{
	if (szFileName ) {
		
		//CPicture::Load(szFileName);
		m_hasPicture = m_image.Load(szFileName);
		//m_hasPicture = true;
		m_pictureFile = szFileName;
	}
	else {
		closePicture();
		
	}

// 	if (m_copyTo)
// 	{
// 		m_copyTo->SetPicture(szFileName, kIndex, AData);
// 	}
	Invalidate(FALSE);
}

void CJpgCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 __super::OnPaint()
	CRect rect, origRect;
	GetClientRect(&rect);
	origRect = rect;
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CDC *pDC (&memDC);

	CBitmap bmp, *pOldBmp;

	
	bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	pOldBmp = memDC.SelectObject(&bmp);

	// 画鼠标移动在上面的效果
	CBrush blackBrush(RGB(0, 0, 0));
	CBrush *old1 = pDC->SelectObject(&blackBrush);

	// 清除背景
	pDC->FillSolidRect(&rect, m_backColor);
	pDC->SelectObject(old1);

	
	if (m_isMouseIn && !isSelected() && getShowFocus())
	{
		// 在图片下画一条线
		CRect focusRect =rect;
		focusRect.top= focusRect.bottom - 5;
		pDC->FillSolidRect(&focusRect, RGB(255,255,0));
		rect.DeflateRect(0,0,0,5);

	}


	// 画焦点
	if (isSelected() )
	{
		CRect focusRect = rect;
		focusRect.DeflateRect(1,1,1,1);
		CPen pen;
		pen.CreatePen(PS_SOLID, 3, RGB(255,255,0));
		CPen *oldPen = pDC->SelectObject(&pen);
		CBrush *oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		
		CPoint p(2,2);
		pDC->RoundRect(&focusRect, p);


		// put back the old objects
		pDC->SelectObject(oldBrush);
		pDC->SelectObject(oldPen);

		rect.DeflateRect(4,4,4,4);

	}
	
	if (m_hasPicture)  {
	
		// shrink our rect 20 pixels in each direction
		//rect.DeflateRect(1,1,0,0);
	
		if (isFlood())
		{
			rect = origRect;
			//CSize szPic = CPicture::GetSize(pDC);
			CSize szPic;
			szPic.cx = m_image.GetWidth();
			szPic.cy = m_image.GetHeight();

			int nw = rect.Width()/szPic.cx;
			int nh = rect.Height()/szPic.cy;
			if (rect.Width()%szPic.cx) nw++;
			if (rect.Height()%szPic.cy) nh++;
			for (int i=0; i<nw; ++i)
				for (int j=0; j<nh; ++j)
				{
					//CPicture::Draw(pDC, i*szPic.cx,j*szPic.cy, szPic.cx, szPic.cy );
					m_image.Draw(pDC->m_hDC, i*szPic.cx,j*szPic.cy, szPic.cx, szPic.cy );
				}
		}
		else if (m_lockRatio){
			//CPicture::Draw(pDC, rect.TopLeft(), rect.Size(), true);
			DrawLockRatio(pDC, rect, m_fill);
		}else {
			//CPicture::Draw(pDC, rect.left, rect.top, rect.right, rect.bottom);
			m_image.Draw(pDC->m_hDC, rect);
		}

		CFont font;
		VERIFY(font.CreatePointFont(90,"宋体", pDC));//创建字体 Arial格式 180为字高
		CFont* def_font = pDC->SelectObject(&font); //选择该字体进入PDC
	
		//m_size =pDC->GetTextExtent(tmp_lpBuf,len);//计算buf中的字体总宽为多少个像素

		CString str;
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
	
		pDC->TextOut(2,2, m_text);
		pDC->SelectObject(def_font);
		font.DeleteObject();
	}

	dc.BitBlt(0, 0, origRect.Width(), origRect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	bmp.DeleteObject();
	memDC.DeleteDC();

}

void CJpgCtrl::OnSelected()
{
	Invalidate(TRUE);
}

LPARAM CJpgCtrl::getData() const
{
	//return ((CSnapItem*)m_data)->frameNum;
	return m_data;
}

void CJpgCtrl::SetText( CString str )
{
	m_text = str;
}

void CJpgCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_isMouseIn){
		TRACKMOUSEEVENT tme; 
		tme.cbSize = sizeof(tme); 
		tme.hwndTrack = m_hWnd; 
		tme.dwFlags = TME_LEAVE ; 
		tme.dwHoverTime = HOVER_DEFAULT; 
		_TrackMouseEvent(&tme); 
		m_isMouseIn = TRUE;

		//LOG_INFO("Enter %s", (LPCTSTR)m_text);
		Invalidate(TRUE);
	}
	__super::OnMouseMove(nFlags, point);
}



void CJpgCtrl::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_isMouseIn) {
		

		m_isMouseIn = false;
		Invalidate(TRUE);
	}
	__super::OnMouseLeave();
}

CString CJpgCtrl::getFileName() const
{
	return m_pictureFile;
}

bool CJpgCtrl::SetPictureBuff( byte *buff, int buffLen, int nFormat )
{
	closePicture();
	bool bOk = false;
	//m_hasPicture = LoadFromBuffer(buff, buffLen);
	if (buff && buffLen>0)
	{
		bOk = m_image.Decode(buff, buffLen, nFormat);
		if (!bOk){
			TRACE1("image decode failed: %s", m_image.GetLastError());
		}
		m_hasPicture = bOk;
	}	

	Invalidate();

	return bOk;

}

void CJpgCtrl::closePicture()
{
	m_image.Clear();
//	CPicture::UnLoad();
	m_hasPicture = false;
	m_pictureFile = "";

// 	if (m_copyTo)
// 	{
// 		m_copyTo->closePicture();
// 	}

// 	if (m_buff)
// 	{
// 		delete[] m_buff;
// 		m_buff = NULL;
// 		m_buffLen = 0;
// 	}
}

bool CJpgCtrl::LoadFromResource( LPCTSTR pName, LPCTSTR pType, HMODULE hInst )
{

	HRSRC hResource = ::FindResource(hInst, pName, pType);
	if (!hResource)
		return false;

	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)
		return false;

	const void* pResourceData = ::LockResource(::LoadResource(hInst, 
		hResource));
	if (!pResourceData)
		return false;
	int nFormat = CXIMAGE_FORMAT_JPG;
	if (stricmp(pType, "jpg")==0)
		nFormat = CXIMAGE_FORMAT_JPG;
	else if (stricmp(pType, "png")==0)
		nFormat = CXIMAGE_FORMAT_PNG;
	else 
		nFormat = CXIMAGE_FORMAT_BMP;

	return SetPictureBuff((byte*)pResourceData, imageSize, nFormat);

//	HGLOBAL m_hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
//	if (m_hBuffer)
//	{
//		void* pBuffer = ::GlobalLock(m_hBuffer);
//		if (pBuffer)
//		{
// 			CopyMemory(pBuffer, pResourceData, imageSize);
// 
// 			IStream* pStream = NULL;
// 			if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
// 			{
// 				m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
// 				pStream->Release();
// 				if (m_pBitmap)
// 				{ 
// 					if (m_pBitmap->GetLastStatus() == Gdiplus::Ok)
// 						return true;
// 
// 					delete m_pBitmap;
// 					m_pBitmap = NULL;
// 				}
// 			}
//			m_pBitmap = NULL;
//			::GlobalUnlock(m_hBuffer);
//		}
//		::GlobalFree(m_hBuffer);
//		m_hBuffer = NULL;
//	}
	return true;
}

bool CJpgCtrl::LoadFromResource( int resID, LPCTSTR pType, HMODULE hInst/*=NULL*/ )
{
	return LoadFromResource(MAKEINTRESOURCE(resID), pType, hInst);
}

void CJpgCtrl::DrawLockRatio( CDC* pDC, CPoint Pos, CSize Size, bool bFill /* = FALSE */ )
{
	long hmWidth=0;   
	long hmHeight=0;   


	CSize szPic;
	szPic.cx= m_image.GetWidth();
	szPic.cy=m_image.GetHeight();
	if (szPic.cx<=0 || szPic.cy<=0) return;

	// 扩充到
	if (bFill) {
		//szPic = Size;
		//保持比例

		CSize szTemp(Size); // 得到矩形区大小
		if (!szTemp.cx || !szTemp.cy)	
			return ;

		if ((double)szTemp.cx/szTemp.cy>(double)szPic.cx/szPic.cy)
		{
			szTemp.cx = Size.cy * (double)szPic.cx/szPic.cy;
			Pos.x += (Size.cx - szTemp.cx) /2;
		} else {

			szTemp.cy = Size.cx * (double)szPic.cy/szPic.cx;
			Pos.y += (Size.cy - szTemp.cy) /2;
		}
		Size = szTemp;
		m_image.Draw(pDC->m_hDC, Pos.x, Pos.y, Size.cx, Size.cy);   
		return;
	}

	//m_pPicture->get_Width(&hmWidth);   
	//m_pPicture->get_Height(&hmHeight);   
	hmWidth = szPic.cx;
	hmHeight = szPic.cy;

	int nWidth  = Size.cx;   
	int nHeight = Size.cy;   

	//LOG_INFO("原大小：%d x %d", hmWidth, hmHeight);

	// 图片没超界
	if (nWidth>=hmWidth && nHeight>=hmHeight)
	{
		Pos.x = Pos.x+(nWidth-hmWidth)/2;
		Pos.y = Pos.y+(nHeight-hmHeight)/2;
		nWidth = hmWidth;
		nHeight = hmHeight;
	} else  // 高或宽超界
	{
		double ratioH, ratioW;
		ratioH = nHeight*1.0/hmHeight;
		ratioW = nWidth*1.0/hmWidth;

		// 图片比较宽
		if ( ratioH > ratioW )
		{
			int newHeight = hmHeight*ratioW;
			Pos.y += (nHeight - newHeight)/2;
			nHeight = newHeight;


		} else {
			int newWidth = hmWidth * ratioH;
			Pos.x += (nWidth - newWidth)/2;
			nWidth = newWidth;
		}

	}
	//LOG_INFO("新大小：%d x %d", nWidth, nHeight);
	m_image.Draw(pDC->m_hDC, Pos.x, Pos.y, nWidth, nHeight);   
	return;
}

void CJpgCtrl::DrawLockRatio( CDC* pDC, CRect rect, bool bFill /* = FALSE */ )
{
	DrawLockRatio(pDC, rect.TopLeft(), rect.BottomRight(), bFill);
}

bool CJpgCtrl::getShowFocus() const
{
	return m_showFocus;
}

void CJpgCtrl::setShowFocus( bool val )
{
	m_showFocus = val;
}

