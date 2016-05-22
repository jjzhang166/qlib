/************************************************************************/
/* author: qhb                                                          */
/* author: quhuabo@gmail.com                                            */
/************************************************************************/
#pragma once

// 禁止显示无名称成员，及 deprecated 函数警告
#pragma warning(disable: 4201 4995)

#include "qlibdef.h"
#include <string>
using namespace std;

class DLL_EXP_QLIB CMyLog
{
public:

	// @AFileName 无路径的文件名称，路径固定为：c:\log

	CMyLog(const char *AFileName="log");

public:
	~CMyLog(void);
	
	static void LogInfo(const char *szFormat, ...);
	static void LogBuff(void *buffer, INT64 nLen, const char *str);

	static void setFileName(const char *szFileName);
protected:
	/************************************************************************/
	/* 检查文件大小，如果超出，则新建文件                                   */
	/************************************************************************/
	static void checkLogFileSize(const char *pathfile);
	
};


DLL_EXP_QLIB void  ErrorMessageBox(DWORD errorNo=0);
DLL_EXP_QLIB CString  ErrorMessageStr(DWORD errorNo=0) ;



#define LOG_INFO(...) CMyLog::LogInfo(__VA_ARGS__)
#define LOG_WARN(...) CMyLog::LogInfo("[Warn]"); CMyLog::LogInfo(__VA_ARGS__)

#ifdef _DEBUG
	#define LOG_DEBUG LOG_INFO
#else
#define LOG_DEBUG(...) NULL
#endif

#define LOG_BUFF(buff, len, text) CMyLog::LogBuff(buff, len, text)

DLL_EXP_QLIB string &  TrimLeft(string &str, char ch=' ');
DLL_EXP_QLIB string &  TrimRight(string &str, char ch=' ');
DLL_EXP_QLIB string &  Trim(string &str, char ch=' ');


DLL_EXP_QLIB string  NumToStr(int x, int width=0, char fillwith='0');
DLL_EXP_QLIB string  NumToStrHex(int x, int width=0, char fillwith='0');


// for c# invoke
/************************************************************************/
/* szFilename 不含目录，不含 .log 扩展名                                */
/************************************************************************/
extern "C" {
	DLL_EXP_QLIB void MyLogInit( LPCTSTR szFilename );
	DLL_EXP_QLIB void MyLogRelease( void );
	DLL_EXP_QLIB void MyLogInfo( const char *szFormat, ... );
	DLL_EXP_QLIB void MyLogBuff( const byte *szBuff, int buffLen, const char *szMsg );

}