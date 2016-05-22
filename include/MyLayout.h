#pragma once
#include "qlibdef.h"
#include <list>

using namespace std;

#define MLT_NONE 0
#define MLT_LEFT 0x01
#define MLT_RIGHT 0x02
#define MLT_TOP  0x04
#define MLT_BOTTOM 0x08

#define MLT_LEFT_RIGHT (MLT_LEFT|MLT_RIGHT)
#define MLT_TOP_BOTTOM (MLT_TOP|MLT_BOTTOM)

#define MLT_ALL (MLT_LEFT|MLT_RIGHT|MLT_TOP|MLT_BOTTOM)

class DLL_EXP_QLIB CMyLayoutItem
{
public:
	int ctrlID;
	int alignType;

	CRect origRect;
	bool needInvalidateOnsize;
};


class DLL_EXP_QLIB CMyLayout
{

public:


	CMyLayout();
	virtual ~CMyLayout(void);

	void initialWndSize(CWnd *pWnd, BOOL fixSkin=FALSE);
	void AddCtrl(int ctrlID, int alignType, bool bNeedInvalidateOnsize=false);
	void AddCtrl(CWnd *pWnd, int alignType, bool bNeedInvalidateOnsize=false);
	void AddCtrl(CWnd &wnd, int alignType, bool bNeedInvalidateOnsize=false);
	void ReCalcMargins();

	void OnSizeChanged(UINT nType, bool bRepaint=true);
	bool OnSizeChangeing(UINT fwSide, LPRECT pRect);

	//
	virtual void OnPngStaticClicked(CWnd* pngStatic);

	BOOL getFixSkin() const;
	void setFixSkin(BOOL val);


protected:

	void SwitchToScreen(CWnd *pCtrl);

private:
	CWnd *m_pWnd;
	CSize m_wndSize;
	BOOL m_bFullScreen;

	list<CMyLayoutItem> m_ctrls;

	void getItemData(CMyLayoutItem &item);
	BOOL m_fixSkin;

	int min_width;
	int min_height;
	
};

