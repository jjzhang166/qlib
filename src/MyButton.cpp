// MyButton.cpp : 实现文件
//

#include "stdafx.h"

#include "MyButtonManager.h"
#include "MyButton.h"
#include "MemDC.h"
#include "RoundRect.h"
#include <atlconv.h>
#include <MyLog.h>

/************************************************************************/
/* 按下按钮位移的像素数                                                 */
/************************************************************************/


// CMyButton
IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton()
:m_nCurState(0), m_bIsToggle(FALSE), m_bIsHovering(FALSE), m_bIsTracking(FALSE), m_pToolTip(NULL)
{
	//m_pBackJpg = NULL;
	m_stdImage = m_hoverImage = m_pressedImage = m_hoverBackImage= m_disableImage= NULL;
	m_checkOffImage = m_checkOnImage = NULL;
	m_pButtonManager = NULL;
	m_buttonMode = bmThreeImage;
	m_rotateAngle = 0;
	m_rotateTimer = 0;
	m_pressOffset = 2;
	m_borderWidth = 0;	// 没有边框
	m_borderColor = Color(255, 255, 255, 255);
	m_showText = FALSE;
	m_textColor = Color::Black;
	m_textColor2 = Color::White;
	m_textPosition = tpCenter;
	m_flashInteval = 500;
	m_isFlashing = FALSE;
	m_backColor = Color::Black;
}

CMyButton::~CMyButton()
{
	DeleteToolTip();
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CMyButton 消息处理程序




BOOL CMyButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	//return CButton::OnEraseBkgnd(pDC);
	return TRUE;
}



void CMyButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bIsHovering = TRUE;
	Invalidate();
	DeleteToolTip();


	// Create a new Tooltip with new Button Size and Location
	SetToolTipText(m_tooltext);

	if (m_pToolTip != NULL)
	{
		if (::IsWindow(m_pToolTip->m_hWnd))
		{
			//Display ToolTip
			m_pToolTip->Update();
		}
	}

	return;
}


void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = _TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}


void CMyButton::OnMouseLeave()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	Invalidate();
	return;
}

void CMyButton::DeleteToolTip()
{
	// Destroy Tooltip in case the size of the button has changed.
	if (m_pToolTip != NULL)
	{
		delete m_pToolTip;
		m_pToolTip = NULL;
	}
	

}

void CMyButton::SetToolTipText(CString spText, BOOL bActivate /*= TRUE*/)
{
	// We cannot accept NULL pointer
	if (spText.IsEmpty()) return;

	// Initialize ToolTip
	InitToolTip();
	m_tooltext = spText;

	// If there is no tooltip defined then add it
	if (m_pToolTip->GetToolCount() == 0)
	{
		CRect rectBtn;
		GetClientRect(rectBtn);
		m_pToolTip->AddTool(this, m_tooltext, rectBtn, 1);
	}

	// Set text for tooltip
	m_pToolTip->UpdateTipText(m_tooltext, this, 1);
	m_pToolTip->SetDelayTime(2000);
	m_pToolTip->Activate(bActivate);
}

void CMyButton::SetToolTipText(UINT nId, BOOL bActivate /*= TRUE*/)
{
	// load string resource
	m_tooltext.LoadString(nId);

	// If string resource is not empty
	if (m_tooltext.IsEmpty() == FALSE)
	{
		SetToolTipText(m_tooltext, bActivate);
	}
}

void CMyButton::InitToolTip()
{
	if (m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl;
		// Create ToolTip control
		m_pToolTip->Create(this);
		m_pToolTip->Activate(TRUE);
	}
}

void CMyButton::PreSubclassWindow()
{
	// Set control to owner draw
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();

}


void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* _pDC = CDC::FromHandle(lpDIS->hDC);
	BOOL bIsPressed = (lpDIS->itemState & ODS_SELECTED);
	BOOL bIsFocus = (lpDIS->itemState & ODS_FOCUS);

	// 画背景
	CRect rect;
	GetClientRect(rect);
	CRect clientRect(rect);
	MemDC dc(_pDC, rect);

	Graphics graphics(dc->m_hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	if (m_imageBack.IsValid()){
		m_imageBack.Draw(dc->m_hDC, rect);
	}else {
		SolidBrush brush(m_backColor);
		graphics.FillRectangle(&brush, Rect(rect.left-1, rect.top-1, rect.Width()+1, rect.Height()+1));
	}

	RectF textRect;
	if (m_showText){
		getTextRect(&graphics, rect, &textRect);
	}


	// 三色按钮模式
	if (m_buttonMode == bmThreeColor) {
		Color currColor;
		if (!m_bIsHovering && !bIsPressed) {
			currColor = m_color1;
			}
		else if (bIsPressed) {
			currColor = m_color3;
		}
		else {
			currColor = m_color2;
		}

		SolidBrush brush(currColor);
		CRoundRect rr;
		rr.FillRoundRect(&graphics, Rect(rect.left, rect.top, rect.right, rect.bottom), currColor, 5);
		//graphics.FillRectangle(&brush, Rect(rect.left, rect.top, rect.right, rect.bottom));
	}
	else {

		// 图片按钮模式

		Gdiplus::Image *pCurImage = m_stdImage;
		//LOG_DEBUG("%d, %d, (%d, %d)", lpDIS->rcItem.left, lpDIS->rcItem.top, lpDIS->rcItem.right, lpDIS->rcItem.bottom);
		CMyGdi::DrawMode drawMode = CMyGdi::dmNormal;

		if (m_showText){
			switch (m_textPosition)
			{
			case tpBottom:
				rect.DeflateRect(0, 0, 0, textRect.Height+5);
				break;
			case tpRight:
				rect.DeflateRect(0, 0, textRect.Width+5, 0);
				break;
			
			}
		}
		if (m_buttonMode == bmOneImage) {		// 单图模式

			if (!IsWindowEnabled()){
				drawMode = CMyGdi::dmDisable;
				rect.DeflateRect(0, 0, m_pressOffset, m_pressOffset);

			}
			else {
				if (bIsPressed) {
					rect.DeflateRect(m_pressOffset, m_pressOffset, 0, 0);
				}
				else {
					rect.DeflateRect(0, 0, m_pressOffset, m_pressOffset);
				}

				if (m_bIsHovering || bIsPressed) drawMode = CMyGdi::dmLighten;
			}
		}
		else if (m_buttonMode == bmThreeImage) // 三图模式
		{
			if (IsWindowEnabled()) {
				if (bIsPressed) {
					pCurImage = m_pressedImage;
					if (pCurImage == NULL) {
						pCurImage = m_hoverImage;
						drawMode = CMyGdi::dmDarken;
					}
				}
				else if (m_bIsHovering)
					pCurImage = m_hoverImage;
			}
			else {
				pCurImage = m_stdImage;
				drawMode = CMyGdi::dmDisable;
			}

		}
		else if (m_buttonMode == bmCheckbox || m_buttonMode == bmLedLight)	// checkbox 模式
		{

			pCurImage = m_bIsToggle ? m_checkOnImage : m_checkOffImage;
			if (!IsWindowEnabled()) {
				drawMode = CMyGdi::dmDisable;
			}
			else {
				if (m_bIsHovering || bIsPressed) drawMode = CMyGdi::dmLighten;
			}
		}


		if (m_bIsHovering && m_hoverBackImage) {
			CMyGdi::DrawPng(m_hoverBackImage, &graphics, clientRect, CMyGdi::dmNormal, 0);
		}

		if (m_pressOffset>0 && bIsPressed){
			rect.OffsetRect(m_pressOffset, m_pressOffset);
		}

		if (!IsWindowEnabled() && m_disableImage!=NULL){
			CMyGdi::DrawPng(m_disableImage, &graphics, rect, CMyGdi::dmNormal, 0);
		} else {

			OSVERSIONINFO osVer;
			CString sTemp;
			osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			::GetVersionEx( &osVer );
			//--------------------------------
			//	根据OSVERSIONINFO的成员 判断。
			//	Windows 2000:       dwMajorVersion = 5 dwMinorVersion = 0 
			//	Windows XP:         dwMajorVersion = 5 dwMinorVersion = 1 
			//	Windows Vista:      dwMajorVersion = 6

			if (osVer.dwMajorVersion<6){
				drawMode = CMyGdi::dmNormal;
			}
			CMyGdi::DrawPng(pCurImage, &graphics, rect, drawMode, m_rotateAngle);
		}
	}

	DrawBorder(&graphics, clientRect);

	// 绘制文字
	if (m_showText) {
		// 设置文字颜色
		Color textColor;
		if (!IsWindowEnabled())
			textColor = Color::Gray;
		else if (!m_bIsHovering && !bIsPressed) {
			textColor = m_textColor;
		}
		else {
			textColor = m_textColor2;
		}

		clientRect.InflateRect(-5, -5);
		if (bIsPressed ) {
			clientRect.OffsetRect(m_pressOffset, m_pressOffset);
		}
		DrawText(&graphics, clientRect, textColor);
	}
	
	if (bIsFocus){
		DrawFocus(&dc, lpDIS->rcItem);
	}


}




BOOL CMyButton::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	if (m_pToolTip != NULL)
	{
		if (::IsWindow(m_pToolTip->m_hWnd))
		{
			m_pToolTip->RelayEvent(pMsg);
		}
	}

	if (pMsg->message == WM_LBUTTONUP || (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE))
	{
		if (m_buttonMode == bmCheckbox)
		{
			if (CanToggle())
				SetChecked(!m_bIsToggle, FALSE);
		}
	}
	return CButton::PreTranslateMessage(pMsg);;
}

void CMyButton::setBackground(CJpgCtrl *pBackJpg)
{
	if (!pBackJpg) return;

	CRect r1, r2;
	
	pBackJpg->GetWindowRect(&r1);
	this->GetWindowRect(&r2);
	r2.OffsetRect(-r1.left, -r1.top);
	m_imageBack.Clear();
	pBackJpg->getImage().Crop(r2, &m_imageBack);

}

void CMyButton::setBackcolor(Gdiplus::Color color)
{
	m_backColor = color;
}

void CMyButton::setImage(Gdiplus::Image *pStdImage, Gdiplus::Image *pHoverImage, Gdiplus::Image *pPressImage)
{
	m_stdImage = pStdImage;
	m_hoverImage = pHoverImage;
	m_pressedImage = pPressImage;
	ASSERT(m_stdImage && m_hoverImage);


	m_buttonMode = bmThreeImage;
}

void CMyButton::setImage(Gdiplus::Image *pStdImage)
{
	m_buttonMode = bmOneImage;
	ASSERT(pStdImage);

	m_stdImage = pStdImage;

	//m_hoverImage = m_stdImage->Clone();
	//m_pressedImage = m_stdImage->Clone();

	
}

void CMyButton::setImage(Gdiplus::Image *pCheckOffImage, Gdiplus::Image *pCheckOnImage)
{
	m_buttonMode = bmCheckbox;
	ASSERT(pCheckOnImage && pCheckOffImage);
	m_checkOffImage = pCheckOffImage;
	m_checkOnImage = pCheckOnImage;

}


void CMyButton::setHoverBackImage(Gdiplus::Image *pImage)
{
	m_hoverBackImage = pImage;
}

void CMyButton::setDisableImage(Gdiplus::Image *pImage)
{
	m_disableImage = pImage;
}

void CMyButton::DrawFocus(CDC *pDC, const CRect &rect)
{
//	CRect r(rect);
//	r.DeflateRect(1, 1);
//
//	pDC->DrawFocusRect(r);
	
}

void CMyButton::setButtonManager(CMyButtonManager *pButtonManager)
{
	ASSERT(pButtonManager);
	m_pButtonManager = pButtonManager;
	CJpgCtrl *pJpg = pButtonManager->getBackJpgCtrl();
	ASSERT(pJpg);
	this->setBackground(pJpg);
}

void CMyButton::SetChecked(BOOL val, BOOL bInvalidate)
{
	m_bIsToggle = val;
	
	if (bInvalidate) Invalidate(FALSE);
}

bool CMyButton::CanToggle()
{
	return true;
}

void CMyButton::OnTimer(UINT nIDEvent)
{
	CButton::OnTimer(nIDEvent);

	if (m_buttonMode == bmLedLight){
		m_bIsToggle = !m_bIsToggle;
	}
	else {
		m_rotateAngle += 5;
		if (m_rotateAngle >= 360) m_rotateAngle = 0;
	}
	Invalidate(FALSE);
}

#define MY_TIMER_ID 99
void CMyButton::startRotate()
{
	stopRotate();
	m_rotateTimer = SetTimer(MY_TIMER_ID, 10, NULL);
}

void CMyButton::stopRotate()
{
	if (m_rotateTimer) {
		KillTimer(m_rotateTimer);
		m_rotateTimer = 0;
	}
}

void CMyButton::setPressOffset(UINT nOffset /*= 1*/)
{
	if (nOffset > 10)
		nOffset = 10;
	m_pressOffset = nOffset;
}

void CMyButton::Rotating(BOOL bRotating /*= TRUE*/)
{
	if (bRotating)
		startRotate();
	else
		stopRotate();
}

void CMyButton::DrawBorder(Graphics *pGraphics, const CRect &rect)
{
	CRoundRect rr;
	if (m_borderWidth > 0) {
		rr.DrawRoundRect(pGraphics, Rect(rect.left, rect.top, rect.right, rect.bottom), m_borderColor, 4, m_borderWidth);
	}
}

void CMyButton::setBorder(UINT nWidth, Color aColor)
{
	setBorderWidth(nWidth);
	setBorderColor(aColor);
}

void CMyButton::DrawText(Graphics *pGraphics, const CRect &rect, Color textColor)
{
	if (m_showText) {
		//rect.top = rect.bottom + 2;
		//rect.bottom = rect.top + TEXT_RECT_HEIGHT - 2;
	

		USES_CONVERSION;
		CString strCaption;
		GetWindowText(strCaption);
		CStringW str = A2W(strCaption);


		FontFamily fontFamily(L"宋体");
		Gdiplus::Font font(&fontFamily, 9, FontStyleRegular, UnitPoint);
		StringFormat stringFormat;

		stringFormat.SetFormatFlags(StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
		switch (m_textPosition)
		{
		case CMyButton::tpCenter:
			stringFormat.SetAlignment(StringAlignmentCenter);
			stringFormat.SetLineAlignment(StringAlignmentCenter);
			break;
		case CMyButton::tpBottom:
			stringFormat.SetAlignment(StringAlignmentCenter);
			stringFormat.SetLineAlignment(StringAlignmentFar);
			break;
		case CMyButton::tpRight:
			stringFormat.SetAlignment(StringAlignmentFar);
			stringFormat.SetLineAlignment(StringAlignmentCenter);
			break;
		default:
			break;
		}

		SolidBrush solidBrush(textColor);

		//WCHAR string[] = L"这些文字是垂直居中且是从右到左的阅读次序, 它们是通过  SetFormatFlags和SetAlignment来设置的！";
		pGraphics->DrawString(str, (int)wcslen(str), &font,
			RectF((Gdiplus::REAL)rect.left, (Gdiplus::REAL)rect.top, (Gdiplus::REAL)rect.Width(), (Gdiplus::REAL)rect.Height()),
			&stringFormat, &solidBrush);

	}
}

void CMyButton::setColorButton(Color colorNormal, Color colorHover, Color colorPress)
{
	m_buttonMode = bmThreeColor;
	m_color1 = colorNormal;
	m_color2 = colorHover;
	m_color3 = colorPress;
}

void CMyButton::setTextColor(Color textColor, Color textColorHover)
{
	m_textColor = textColor;
	m_textColor2 = textColorHover;
}

void CMyButton::setTextColor(Color textColor)
{
	setTextColor(textColor, textColor);
}

void CMyButton::getTextRect(Graphics *pGraphics, const CRect &rect, RectF *textRect)
{

	USES_CONVERSION;
	CString strCaption;
	GetWindowText(strCaption);
	CStringW str = A2W(strCaption);


	FontFamily fontFamily(L"宋体");
	Gdiplus::Font font(&fontFamily, 9, FontStyleRegular, UnitPoint);
	StringFormat stringFormat;

	stringFormat.SetFormatFlags(StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
	switch (m_textPosition)
	{
	case CMyButton::tpCenter:
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(StringAlignmentCenter);
		break;
	case CMyButton::tpBottom:
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(StringAlignmentFar);
		break;
	case CMyButton::tpRight:
		stringFormat.SetAlignment(StringAlignmentFar);
		stringFormat.SetLineAlignment(StringAlignmentCenter);
		break;
	default:
		break;
	}


	pGraphics->MeasureString(str, -1, &font, RectF(rect.left, rect.top, rect.Width(), rect.Height()), &stringFormat, textRect);
	//LOG_DEBUG("%f, %f, width: %f, height: %f", textRect->X, textRect->Y, textRect->Width, textRect->Height);

}

void CMyButton::FlashLedLight(BOOL bFlash /*= TRUE*/)
{

	if (m_rotateTimer) {
		KillTimer(m_rotateTimer);
		m_rotateTimer = 0;
	}

	if (bFlash){
		m_rotateTimer = SetTimer(MY_TIMER_ID, m_flashInteval, NULL);
	}

	m_isFlashing = bFlash;
}

void CMyButton::setToggle(BOOL val)
{
	m_bIsToggle = val;
	Invalidate(FALSE);
}

