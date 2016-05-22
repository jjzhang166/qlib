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
	char szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	
	
	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = hParent;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "��ѡ��Ŀ¼��";   
	bi.ulFlags = BIF_EDITBOX|BIF_NEWDIALOGSTYLE;//BIF_USENEWUI;   
	bi.lpfn = BrowseCallbackProc;   
	bi.lParam = (LPARAM)szInitDir.GetBuffer(szInitDir.GetLength());   
	bi.iImage = 0;   
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	bool bOk = false;
	if(lp)
	{
		if (SHGetPathFromIDList(lp, szPath)) {
			//str.Format("ѡ���Ŀ¼Ϊ %s",  szPath);
			szInitDir = szPath;

			bOk = true;
		} else {
			AfxMessageBox("��Ч��Ŀ¼��������ѡ��");   
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
