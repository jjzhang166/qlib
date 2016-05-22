#pragma once
#include "qlibdef.h"
class DLL_EXP_QLIB CFileUtil
{
public:
	CFileUtil(void);
	virtual ~CFileUtil(void);

public:
	 
	/************************************************************************/
	/* ��ɾ��Ŀ¼����Ŀ¼�µ������ļ�����Ŀ¼������֧��ɾ���ļ�                   */
	/* ɾ���ļ���ʹ�ã�DeleteFile												*/
	/************************************************************************/
	static bool delete_directory(LPCTSTR szPath);

	/************************************************************************/
	/* ���Ŀ¼�Ƿ���ڣ���Ҫʱ����                                            */
	/************************************************************************/
	static BOOL  CheckDir(const char *path, const char*msg="");


	/************************************************************************/
	/* true: �ļ�����, false���ļ�������									*/
	/*                                                                      */
	/************************************************************************/
	static bool  GetSizeOfFile(const char *pathfile, ULONGLONG *nFileSize);



	static CString  GetFileVersion(void);
	static CString  GetAppPath(void);
	static LPCTSTR GetAppPath_C(char *buff, int buff_size);


	// �ж�Ŀ¼�Ƿ����
	bool FolderExists(CString s);

#define IsFolderExists FolderExists

};

