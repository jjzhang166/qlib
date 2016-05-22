#include "StdAfx.h"
#include "MyLayout.h"


CMyLayout::CMyLayout()
{
	m_fixSkin = FALSE;	
	m_bFullScreen = FALSE;
	m_pWnd = NULL;
	min_width = min_height = 0;
}


CMyLayout::~CMyLayout(void)
{
}


void CMyLayout::AddCtrl( int ctrlID, int alignType, bool bNeedInvalidateOnsize )
{
	CMyLayoutItem item;
	item.ctrlID = ctrlID;
	item.alignType = alignType;
	item.needInvalidateOnsize = bNeedInvalidateOnsize;

	ASSERT(m_pWnd);

	getItemData(item);
	
	
	if ((item.alignType & MLT_LEFT_RIGHT) == MLT_LEFT_RIGHT)
	{
		int x = m_wndSize.cx-item.origRect.Width();
		if (min_width<x)
			min_width = x;
	}
	
	if ((item.alignType & MLT_TOP_BOTTOM) == MLT_TOP_BOTTOM)
	{
		int y = m_wndSize.cy -item.origRect.Height();
		if (min_height<y)
		{
			min_height=y;
		}
	}

	m_ctrls.push_back(item);


}

void CMyLayout::AddCtrl( CWnd *pWnd, int alignType, bool bNeedInvalidateOnsize )
{
	AddCtrl(pWnd->GetDlgCtrlID(), alignType, bNeedInvalidateOnsize);
}

void CMyLayout::AddCtrl( CWnd &wnd, int alignType, bool bNeedInvalidateOnsize )
{
	AddCtrl(&wnd, alignType, bNeedInvalidateOnsize);
}

void CMyLayout::OnSizeChanged( UINT nType, bool bRepaint )
{
	int cx, cy;
	CRect rect;
	if (!m_pWnd || !m_pWnd->m_hWnd) return ;

	m_pWnd->LockWindowUpdate();
	m_pWnd->GetWindowRect(&rect);
	cx = rect.Width();
	cy = rect.Height();

	

	for (list<CMyLayoutItem>::iterator i = m_ctrls.begin();i!=m_ctrls.end(); ++i)
	{
		bool bNeedMove = false;
		CRect newRect = i->origRect;
		if (i->alignType == 0 || i->alignType==MLT_LEFT || i->alignType==MLT_TOP)
		{
			// É¶Ò²²»¸É
		} else {
			bNeedMove = true;
			
			if (i->alignType & MLT_RIGHT)
			{
				

				newRect.right = cx - (m_wndSize.cx - i->origRect.right);
				if (i->alignType & MLT_LEFT)
				{
					//
				} else {
					newRect.left = newRect.right - i->origRect.Width();

				}
			}
			
			if (i->alignType & MLT_BOTTOM)
			{
				newRect.bottom = cy - (m_wndSize.cy - i->origRect.bottom);
				if (i->alignType & MLT_TOP)
				{

				} else {
					newRect.top = newRect.bottom - i->origRect.Height();
				}
			}

			CWnd * wnd = m_pWnd->GetDlgItem(i->ctrlID);
			wnd->MoveWindow(&newRect, bRepaint);
			if (i->needInvalidateOnsize)
			{
				wnd->Invalidate(FALSE);
			}
		}
	}

	m_pWnd->UnlockWindowUpdate();
}

void CMyLayout::getItemData( CMyLayoutItem &item )
{
	CWnd * pWnd = m_pWnd->GetDlgItem(item.ctrlID);
	pWnd->GetWindowRect(item.origRect);
	m_pWnd->ScreenToClient(&item.origRect);


}

void CMyLayout::initialWndSize( CWnd *pWnd, BOOL fixSkin )
{
	setFixSkin(fixSkin);

	m_pWnd = pWnd;
	CRect r;
	pWnd->GetWindowRect(&r);

	int xdiff = m_fixSkin?14:0;
	int ydiff = m_fixSkin?16:0;

	m_wndSize.cx = r.Width()-xdiff;
	m_wndSize.cy = r.Height()-ydiff;

}

void CMyLayout::OnPngStaticClicked( CWnd* pngStatic )
{

}

void CMyLayout::ReCalcMargins()
{
	initialWndSize(m_pWnd, 0);
	list<CMyLayoutItem> temp;
	temp = m_ctrls;
	m_ctrls.clear();

	for (auto i=temp.begin(); i!=temp.end(); ++i)
	{
		AddCtrl(i->ctrlID, i->alignType, i->needInvalidateOnsize);
	}
}

void CMyLayout::SwitchToScreen( CWnd *pCtrl )
{
// 	m_bFullScreen=!m_bFullScreen;
// 	if(m_bFullScreen)//
// 	{
// 
// 		//::ShowWindow((CWnd*)this,SW_HIDE);   
// 		((CWnd*)this)->ShowWindow(SW_HIDE);
// 		pCtrl->SetParent(NULL);
// 		//this->ShowWindow(SW_SHOWMAXIMIZED);
// 		pCtrl->ShowWindow(SW_SHOWMAXIMIZED);
// 
// 		int   nScreenWidth=GetSystemMetrics(SM_CXSCREEN);   
// 		int   nScreenHeight=GetSystemMetrics(SM_CYSCREEN);
// 		::SetWindowPos(pCtrl->m_hWnd,HWND_TOPMOST,0,0, nScreenWidth,nScreenHeight, SWP_FRAMECHANGED);
// 
// 	}
// 	else
// 	{
// 		//::SetParent(m_hWnd,m_hWndParent);   
// 		pCtrl->SetParent((CWnd*)this);
// 		//::ShowWindow(m_hWndParent,SW_SHOW);  
// 		((CWnd*)this)->ShowWindow(SW_SHOW);
// 
// 	}
}

BOOL CMyLayout::getFixSkin() const
{
	return m_fixSkin;
}

void CMyLayout::setFixSkin( BOOL val )
{
	m_fixSkin = val;
}

bool CMyLayout::OnSizeChangeing(UINT fwSide, LPRECT pRect)
{
	if (pRect->right-pRect->left<min_width && min_width>0)
	{
		pRect->right = pRect->left+min_width;
	}
	
	if (pRect->bottom-pRect->top<min_height && min_height>0)
	{
		pRect->bottom = pRect->bottom+min_height;
	}
	
// 
// 	RECT r;
// 	for (auto i = m_ctrls.begin();i!=m_ctrls.end(); ++i)
// 	{
// 		//bool bNeedMove = false;
// 		//CRect newRect = i->origRect;
// 		CWnd *pCtrl = m_pWnd->GetDlgItem(i->ctrlID);
// 		if (!pCtrl) continue;
// 
// 		
// 		pCtrl->GetWindowRect(&r);
// 
// 		if (i->alignType&MLT_LEFT_RIGHT)
// 		{
// 			if (r.left >= r.right)
// 			{
// 				pRect->right++;
// 			}
// 		}
// 
// 		if (i->alignType&MLT_TOP_BOTTOM)
// 		{
// 			if (r.top >= r.bottom)
// 				pRect->bottom ++;
// 		}
// 
// 
// 	}
	return true;
}
