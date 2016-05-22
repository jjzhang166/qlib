#include "StdAfx.h"
#include "SelectDir.h"


static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)  
{
	if(uMsg == BFFM_INITIALIZED)
	{  
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;  
}


CSelectDir::CSelectDir(void)
{
}


CSelectDir::~CSelectDir(void)
{
}

bool CSelectDir::Show(HWND hParent, CString & szInitDir )
{
	char szPath[MAX_PATH];     //存放选择的目录路径 
	
	
	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = hParent;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择目录：";   
	bi.ulFlags = BIF_EDITBOX|BIF_NEWDIALOGSTYLE;//BIF_USENEWUI;   
	bi.lpfn = BrowseCallbackProc;   
	bi.lParam = (LPARAM)szInitDir.GetBuffer(szInitDir.GetLength());   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	bool bOk = false;
	if(lp)
	{
		if (SHGetPathFromIDList(lp, szPath)) {
			//str.Format("选择的目录为 %s",  szPath);
			szInitDir = szPath;

			bOk = true;
		} else {
			AfxMessageBox("无效的目录，请重新选择");   
		}

		IMalloc * imalloc = 0;
		if ( SUCCEEDED(SHGetMalloc( &imalloc)))
		{
			imalloc->Free (lp);
			imalloc->Release();
		}
	}

	return bOk;
}
