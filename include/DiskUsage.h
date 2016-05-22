/************************************************************************/
/* author: qhb                                                          */
/* author: quhuabo@gmail.com                                            */
/************************************************************************/
#pragma once
#include "qlibdef.h"
class DLL_EXP_QLIB CDiskUsage
{
public:
	CDiskUsage(void);
	~CDiskUsage(void);


	static CString getDriveUsage(LPCTSTR szPart, ULARGE_INTEGER* TotalBytes=NULL, ULARGE_INTEGER* FreeBytes=NULL);
	static CString splitForDrive(LPCTSTR szDirPath);
	
};

