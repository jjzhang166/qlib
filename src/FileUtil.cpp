#include "StdAfx.h"
#include "MyLog.h"

//#define _AFXEXT
#include "FileUtil.h"

CFileUtil::CFileUtil(void)
{
}


CFileUtil::~CFileUtil(void)
{
}

bool CFileUtil::delete_directory( LPCTSTR szPath )
{
	SHFILEOPSTRUCT FileOp={0}; 
	//FileOp.fFlags = FOF_ALLOWUNDO |   //允许放回回收站
	//	FOF_NOCONFIRMATION; //不出现确认对话框

	FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI |FOF_NOCONFIRMMKDIR;   //无需要放回收站


	CString str = szPath;
	if (str.Right(1) != "\\")
	{
		str += "\\";
	}

	str += "\0";
	FileOp.pFrom = str; 
	FileOp.pTo = NULL;      //一定要是NULL
	FileOp.wFunc = FO_DELETE;    //删除操作

	LOG_INFO("delete file(or directory): %s", szPath);
	int bRet = SHFileOperation(&FileOp) ; 
	LOG_INFO("DELETE return: 0x%04x",bRet);
	LOG_INFO(bRet==0?"delete Ok":"delete failed(maybe not exist)");

	return bRet == 0;
}

BOOL CFileUtil::CheckDir( const char *path, const char*msg/*=""*/ )
{
	if (!PathFileExists(path))
	{
		if (!CreateDirectory(path, 0))
		{
			LOG_INFO(msg);	
			return FALSE;
		}
	}
	return TRUE;
}

bool CFileUtil::GetSizeOfFile( const char *pathfile, ULONGLONG *nFileSize )
{
	CFileStatus fileStatus;

	if (CFile::GetStatus(pathfile, fileStatus))
	{
		*nFileSize = fileStatus.m_size;
		return true;
	} 
	return false;
}

CString CFileUtil::GetFileVersion( void )
{
	/* 
	// 会报一个内在访问的错误，因此该函数，不再推荐使用 
	CString strVersion = _T("");  
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);  
	if (hRsrc != NULL)  
	{  
		HGLOBAL hGlobalMemory = LoadResource(NULL, hRsrc);  
		if (hGlobalMemory != NULL)  
		{  
			CString rVersion;  
			LPVOID pVersionResouece = LockResource(hGlobalMemory);  
			LPVOID pVersion = NULL;  
			DWORD uLength,langD;  
			BOOL retVal;  
			retVal = VerQueryValue(pVersionResouece, _T("\\VarFileInfo\\Translation"), (LPVOID*)&pVersion, (UINT*)&uLength);  
			if (retVal && uLength == 4)   
			{  
				memcpy(&langD,pVersion,4);   
				rVersion.Format(_T("\\StringFileInfo\\%02X%02X%02X%02X\\ProductVersion"),  
					(langD & 0xff00)>>8,langD & 0xff,(langD & 0xff000000)>>24,   
					(langD & 0xff0000)>>16);  
			}  
			else  
			{  
				rVersion.Format(_T("\\StringFileInfo\\%04X04B0\\ProductVersion"), GetUserDefaultLangID());  
			}  

			LPVOID pVersion2 = NULL; 
			DWORD uLength2 = 0;
			if( VerQueryValueA(pVersionResouece, rVersion.GetBuffer(0), (LPVOID*)&pVersion2, (UINT *)&uLength2) != 0 )  
			{  
				strVersion.Format(_T("%s"), pVersion2);  
			}  
		}  
		FreeResource(hGlobalMemory);  
	}  
	return (LPCTSTR)strVersion;  
	*/
	return "1.1.1";
}

CString CFileUtil::GetAppPath( void )
{
	TCHAR tszBuf[MAX_PATH] = {'\0'};
	GetModuleFileName( NULL, tszBuf, MAX_PATH);
	CString strDir, tmpDir;
	tmpDir = tszBuf;
	return (LPCTSTR)(tmpDir.Left( tmpDir.ReverseFind('\\') ));

}

LPCTSTR CFileUtil::GetAppPath_C( char *buff, int buff_size )
{
	CString szAppPath = GetAppPath();
	strcpy_s(buff, buff_size, szAppPath);

	return buff;
}

bool CFileUtil::FolderExists( CString s )
{
	DWORD attr; 
	attr = GetFileAttributes(s); 
	return (attr != (DWORD)(-1) ) &&
		( attr & FILE_ATTRIBUTE_DIRECTORY);
}
