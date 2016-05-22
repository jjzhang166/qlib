#pragma once
#include "qlibdef.h"
class DLL_EXP_QLIB CFileUtil
{
public:
	CFileUtil(void);
	virtual ~CFileUtil(void);

public:
	 
	/************************************************************************/
	/* 仅删除目录及其目录下的所有文件（含目录），不支持删除文件                   */
	/* 删除文件请使用：DeleteFile												*/
	/************************************************************************/
	static bool delete_directory(LPCTSTR szPath);

	/************************************************************************/
	/* 检测目录是否存在，必要时创建                                            */
	/************************************************************************/
	static BOOL  CheckDir(const char *path, const char*msg="");


	/************************************************************************/
	/* true: 文件存在, false：文件不存在									*/
	/*                                                                      */
	/************************************************************************/
	static bool  GetSizeOfFile(const char *pathfile, ULONGLONG *nFileSize);



	static CString  GetFileVersion(void);
	static CString  GetAppPath(void);
	static LPCTSTR GetAppPath_C(char *buff, int buff_size);


	// 判断目录是否存在
	bool FolderExists(CString s);

#define IsFolderExists FolderExists

};

