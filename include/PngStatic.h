#pragma once
#include "atlimage.h"
#include <list>
#include "SingleSelect.h"
#include "ImageManager.h"
using namespace std;
//using namespace Gdiplus;
// CPngStatic
#include "qlibdef.h"

class DLL_EXP_QLIB CPngStatic : public CStatic, public CSingleSelectItem
{
	DECLARE_DYNAMIC(CPngStatic)

public:
	CPngStatic();
	virtual ~CPngStatic();
	
	void setImage(const CImageManager &images, const CString & szKey, BOOL bNeedInvalidate=TRUE);
	void setDecoImage(const CImageManager &images, const CString & szKey);
	void setSeleImage(const CImageManager &images, const CString & szKey);

	// add by qhb at: 2014-08-08
	void setImageAll(const CString & szCaption, const CImageManager &images, const CString & szKeyDefault, 
		const CString &szKeySele, const CString &szKeyDeco, BOOL bNeedInvalidate=TRUE);

	CSize getImageSize() const;
	void setAllPngs(list<CPngStatic*> * allPngs){m_allPngs = allPngs;}
	virtual void OnSelected();
	void setDisable(BOOL bDisable=TRUE, BOOL bNeedInvalidate=TRUE);
	BOOL isDisable(); 
	void refresh();

	void setTextColor(COLORREF textColor);
protected:
	DECLARE_MESSAGE_MAP()
	//void setImageFile(const CString &szFileName);
	//void setCheckedImage(CString szImage);
public:
	afx_msg void OnPaint();

	int m_imageWidth;
	int m_imageHeight;

	virtual BOOL PreTranslateMessage(MSG* pMsg); 
private:

	CString m_imageFile;
	bool m_IsInWindow;
	Gdiplus::Image *m_image;
	Gdiplus::Image *m_disableImage;
	Gdiplus::Image *m_DecoImage;
	Gdiplus::Image *m_SelectImage;

	afx_msg LRESULT OnMouseLeave(WPARAM,LPARAM);
	void OnMouseMove(UINT nFlags, CPoint point);

	void InvalidateParentRect();

	list<CPngStatic*> *m_allPngs;
	CString m_caption;
	bool m_checked;
	bool m_showText;
	BOOL m_isDisable;
	COLORREF m_textColor;
	int m_textSize;
	//CImageManager *m_imagesManager;
protected:
	virtual void PostNcDestroy();
public:
	
	CString getCaption() const;
	void setCaption(CString val, BOOL bNeedInvalidate=FALSE);
	
	bool isChecked() const;
	void setChecked(bool val);

	bool getShowText() const;
	void setShowText(bool val);

	int getTextSize() const;
	void setTextSize(int val);


};


