#pragma once
#include "Picture.h"
#include "SingleSelect.h"
#include <ximage.h>
//#define WM_JPG_CLICK (WM_USER+20)

#include "qlibdef.h"

// CJpgCtrl
class DLL_EXP_QLIB CJpgCtrl : public CStatic, /* public CPicture, */public CSingleSelectItem
{
	DECLARE_DYNAMIC(CJpgCtrl)

public:
	CJpgCtrl();
	virtual ~CJpgCtrl();
	
	void OnSelected();


	void SetPicture(LPCTSTR szFileName, int kIndex=0, LPARAM AData=NULL);
	bool SetPictureBuff(byte *buff, int buffLen, int nFormat=CXIMAGE_FORMAT_JPG);
	bool LoadFromResource(int resID, LPCTSTR pType, HMODULE hInst=NULL);
	bool LoadFromResource(LPCTSTR pName, LPCTSTR pType, HMODULE hInst=NULL);

	bool hasPicture() const {return m_hasPicture; }
	
	LPARAM getData() const;
	CString getFileName() const;

	int getShowIndex() const { return m_ShowIndex;}
	void closePicture();

	bool	getShowFocus() const;
	void	setShowFocus(bool val);
	CxImage &getImage() { return m_image; }
	const CxImage &getImage() const { return m_image; }
private:
	LPARAM	m_data;
	bool	m_hasPicture;
	CString m_text;
	bool	m_isMouseIn;
	int		m_ShowIndex;
	CString m_pictureFile;
	bool	m_lockRatio;
	bool	m_flood;	// Æ½ÆÌÄ£Ê½
	bool	m_fill;
	COLORREF m_backColor;
	
	//byte*	m_buff;
	//int		m_buffLen;
	CxImage m_image;
	bool	m_showFocus;
protected:
	DECLARE_MESSAGE_MAP()

	void DrawLockRatio(CDC* pDC, CPoint Pos, CSize Size, bool bFill /* = FALSE */);
	void DrawLockRatio(CDC* pDC, CRect rect, bool bFill /* = FALSE */);
public:
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	//	afx_msg void OnStnClicked();
	//	afx_msg void OnStnDblclick();
	afx_msg void OnPaint();
	void SetText( CString str );
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	afx_msg void OnMouseLeave();


	bool isLockRatio() const { return m_lockRatio; }
	void setLockRatio(bool val) { m_lockRatio = val; }

	bool isFlood() const { return m_flood; }
	void setFlood(bool val) { m_flood = val; }
	
	bool isFill() const { return m_fill; }
	void setFill(bool val) { m_fill = val; }
	
	COLORREF getBackColor() const { return m_backColor; }
	void setBackColor(COLORREF val) { m_backColor = val; }
};


