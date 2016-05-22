#pragma once
#include "stdafx.h"

#include "JpgCtrl.h"
#include "MyGdi.h"
// CMyButton

class CMyButtonManager;
class DLL_EXP_QLIB CMyButton : public CButton
{
	DECLARE_DYNAMIC(CMyButton)

public:
	CMyButton();
	virtual ~CMyButton();


	// 按钮的当前状态
	enum MouseStates
	{
		msNormal=0,
		msHover,
		msPressed,
	};
	enum ButtonMode
	{
		bmThreeImage,	// 三张图
		bmOneImage,		// 单图，图像增量，位移
		bmCheckbox,		// 模拟CheckBox，第一张图和第二张图切换，off 第一张图，on 第二张图
		bmThreeColor,	// 三色按钮
		bmLedLight,		// LED 指示灯模式，彩色灯与灰度切换，并提供定时闪烁的功能
	};

	enum TextPosition
	{
		tpCenter=0,
		tpBottom,
		tpRight,
	};
protected:
	DECLARE_MESSAGE_MAP()

	int		m_nCurState;
	BOOL	m_bIsToggle;
	BOOL	m_bIsHovering;
	BOOL	m_bIsTracking;

	CToolTipCtrl*	m_pToolTip;
	CString m_tooltext;
	//CJpgCtrl	*m_pBackJpg;

	CMyButtonManager *m_pButtonManager;
	
	CxImage	m_imageBack;

	Gdiplus::Image *m_stdImage;
	Gdiplus::Image *m_hoverImage;
	Gdiplus::Image *m_pressedImage;
	Gdiplus::Image *m_hoverBackImage;
	Gdiplus::Image *m_disableImage;

	Gdiplus::Image *m_checkOffImage;
	Gdiplus::Image *m_checkOnImage;

	Gdiplus::Color m_backColor;

	ButtonMode m_buttonMode;
public:
	void InitToolTip();
	void DeleteToolTip();
	void SetToolTipText(CString spText, BOOL bActivate = TRUE);
	void SetToolTipText(UINT nId, BOOL bActivate = TRUE);

	void setBackground(CJpgCtrl *pBackJpg);
	/************************************************************************/
	/* 设置当没有背景图片时的背景纯色                                       */
	/************************************************************************/
	void setBackcolor(Gdiplus::Color color);

	void setButtonManager(CMyButtonManager *pButtonManager);

	void setImage(Gdiplus::Image *pStdImage);
	void setImage(Gdiplus::Image *pCheckOffImage, Gdiplus::Image *pCheckOnImage);
	void setHoverBackImage(Gdiplus::Image *pImage);
	void setDisableImage(Gdiplus::Image *pImage);

	// 设置按钮的三张图
	// pStdImage: 正常状态
	// pHoverImage: 鼠标移到按钮上面的图片
	// pPressImage: 按下时的图片, 当为NULL时，由 pHoverImage 自动生成（变暗）)
	void setImage(Gdiplus::Image *pStdImage, Gdiplus::Image *pHoverImage, Gdiplus::Image *pPressImage);

	// 是否选中
	BOOL IsChecked() const { return m_bIsToggle; }

	// 设置是否选中状态
	void SetChecked(BOOL val, BOOL bInvalidate=FALSE);
	// 设置单图模式的按下效果的像素偏移量
	void setPressOffset(UINT nOffset);

public:

	// 是否允许用户点击以后自动切换状态
	// true：允许自动切换，false 不允许
	// 当需要禁止自动切换 checkOn or checkOff 时，在此加入代码
	virtual bool CanToggle();

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnTimer(UINT nIDEvent);

	virtual void PreSubclassWindow() override;


	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
protected:
	void DrawFocus(CDC *pDC, const CRect &rect);
	void DrawBorder(Graphics *pGraphics, const CRect &rect);

	void DrawText(Graphics *pGraphics, const CRect &rect, Color textColor);
	void getTextRect(Graphics *pGraphics, const CRect &rect, RectF *textRect);
private:
	double m_rotateAngle;
	UINT_PTR m_rotateTimer;
	UINT m_pressOffset;
	UINT m_borderWidth;

	Color m_borderColor;
	BOOL m_showText;
	Color m_textColor;
	Color m_textColor2;
	Color m_color1, m_color2, m_color3;
	TextPosition m_textPosition;
	int m_flashInteval;	// LED 闪动的间隔（毫秒）
	int m_isFlashing;	// 是否在闪动
public:

	void startRotate();
	void stopRotate();
	void Rotating(BOOL bRotating = TRUE);
	UINT getBorderWidth() const { return m_borderWidth; }
	void setBorderWidth(UINT val) { m_borderWidth = val; }


	Color getBorderColor() { return m_borderColor;  }
	void setBorderColor(Color aColor){ m_borderColor = aColor; }

	void setBorder(UINT nWidth, Color aColor);
	BOOL getShowText() const { return m_showText; }
	void setShowText(BOOL val) { m_showText = val; }


	void setColorButton(Color colorNormal, Color colorHover, Color colorPress);
	void setTextColor(Color textColor, Color textColorHover);
	void setTextColor(Color textColor);

	CMyButton::TextPosition getTextPosition() const { return m_textPosition; }
	void setTextPosition(CMyButton::TextPosition val) { m_textPosition = val; }

	CMyButton::ButtonMode getButtonMode() const { return m_buttonMode; }
	void setButtonMode(CMyButton::ButtonMode val) { m_buttonMode = val; }

	void FlashLedLight(BOOL bFlash = TRUE);
	int getFlashInteval() const { return m_flashInteval; }
	void setFlashInteval(int val) { m_flashInteval = val; }

	BOOL getToggle() const { return m_bIsToggle; }
	void setToggle(BOOL val);

	int isFlashing() const { return m_isFlashing; }


};


