/************************************************************************/
/* author: qhb                                                          */
/* author: quhuabo@gmail.com                                            */
/************************************************************************/
#include "StdAfx.h"

#include "DiskUsage.h"


CDiskUsage::CDiskUsage(void)
{
}


CDiskUsage::~CDiskUsage(void)
{
}

CString CDiskUsage::getDriveUsage( LPCTSTR strDriver, ULARGE_INTEGER* pTotalBytes, ULARGE_INTEGER* pFreeBytes )
{
	CString strAllInfo;

	ULARGE_INTEGER FreeAv, TotalBytes, FreeBytes;
	FreeAv.QuadPart = TotalBytes.QuadPart = FreeBytes.QuadPart = 0;
	//获得磁盘空间信息
	
	if (GetDiskFreeSpaceEx(strDriver,&FreeAv,&TotalBytes,&FreeBytes))
	{
		//格式化信息，并显示出来
		CString strTotalBytes,strFreeBytes;
		//strTotalBytes.Format("Total:%u bytes",TotalBytes.QuadPart);
		//strFreeBytes.Format("Left:%u bytes",FreeBytes.QuadPart);

		strTotalBytes.Format("%.2fG",TotalBytes.QuadPart/(1024*1024*1024.0));
		strFreeBytes.Format("%.2fG",(TotalBytes.QuadPart - FreeBytes.QuadPart)/(1024*1024*1024.0));

		strAllInfo.Format("[%s] 已用 %s / %s",strDriver,strFreeBytes, strTotalBytes);

		if (pTotalBytes)
			*pTotalBytes = TotalBytes;
		if (pFreeBytes)
			*pFreeBytes = FreeBytes;
		
	}
	return strAllInfo;
}

CString CDiskUsage::splitForDrive( LPCTSTR szDirPath )
{
	char drive[255];
	
	_splitpath(szDirPath, drive, NULL, NULL, NULL);
	return drive;
}
