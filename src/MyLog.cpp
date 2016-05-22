/************************************************************************/
/* author: qhb                                                          */
/* author: quhuabo@gmail.com                                            */
/************************************************************************/
#include "stdafx.h"

#pragma warning(disable: 4995)


#include <afxmt.h>
#include <shlwapi.h>
#include <Strsafe.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>



#include "FileUtil.h"


#include "MyLog.h"


using namespace std;

#define MAX_LOG_NAME_LEN 100
static CCriticalSection *csLog = NULL;
static char m_pFileName[MAX_LOG_NAME_LEN] = "";

static int logRefCount = 0;
CMyLog::CMyLog(const char *AFileName)
{
	//ASSERT(csLog==NULL);
	if (!csLog) csLog =  new CCriticalSection;
	setFileName(AFileName);

	logRefCount ++;
	//LOG_INFO("------------------ Go,Go,Go -------------------------");
}

CMyLog::~CMyLog(void)
{
	logRefCount--;
	if (logRefCount==0)
	{
		delete csLog;
		csLog = NULL;
		ZeroMemory(m_pFileName, MAX_LOG_NAME_LEN);
	}

//	delete m_pFileName;
	
}

void CMyLog::setFileName( const char *szFileName )
{
	if (!m_pFileName[0]) {
		strncpy_s<MAX_LOG_NAME_LEN>( m_pFileName, szFileName, MAX_LOG_NAME_LEN-1);
		m_pFileName[MAX_LOG_NAME_LEN - 1] = '\0';

	}
}

void CMyLog::LogInfo( const char *szFormat, ... )
{
	DWORD oldError = GetLastError();

	time_t t = time(NULL);
	struct tm tmtime;
	localtime_s(&tmtime, &t);
	CString szDatetime;
	szDatetime.Format("[%4u-%02u-%02u %02u:%02u:%02u] ", tmtime.tm_year+1900, tmtime.tm_mon+1, tmtime.tm_mday,
		tmtime.tm_hour, tmtime.tm_min, tmtime.tm_sec);

	CString logPath = "c:\\log";
	CSingleLock lock(csLog, TRUE);
	char buff[255];
	sprintf_s(buff, 255,"failed to create dir: %s", (LPCSTR)logPath);
	CFileUtil::CheckDir((LPCSTR)logPath, buff);

	
	//CString str;
	va_list   vlist;
	va_start(   vlist,   szFormat  ); 

#define MAX_STR_BUFF 64*1014
	static char str[MAX_STR_BUFF]={0};
	vsprintf_s(str, MAX_STR_BUFF, szFormat, vlist);

	//str.Format(_T(szFormat), vlist);
	if (strcmp(m_pFileName,"")==0)
		logPath += "\\fscoped.log";
	else
		logPath += CString("\\") + m_pFileName + ".log";

	checkLogFileSize((LPCSTR)logPath);
	std::ofstream of(logPath, std::ios::app);

	//TRACE("%s\n", str);

	of << (LPCSTR)szDatetime <<str << std::endl;
	
	SetLastError(oldError);

}

void CMyLog::LogBuff( void *buffer, INT64 nLen, const char *str )
{
	if (nLen<=0) {
		LOG_INFO("%s: bad buff size: %d", str, nLen);
	} else {
		// 记录16进制
		BYTE *p = (BYTE*)buffer;
		stringstream ss;
		ss <<"[" <<str <<"] buff(size: "<<nLen <<"): ";
		for (INT64 i=0; i<nLen; ++i)
		{
			ss <<setw(2)<<setfill('0')<< hex << (int)p[i]<< " ";
		}
	
		LOG_INFO(ss.str().c_str());	
	}
}

// #ifdef _DEBUG
// #define MAX_FILE_SIZE 20	// 文件最大为1M
// const int MAX_LOG_BAKCUP = 100;
// 
// #else
#define MAX_FILE_SIZE 1024*1024	// 文件最大为1M
const int MAX_LOG_BAKCUP = 20;	// 文件备份的个数为20个

//#endif // _DEBUG

void CMyLog::checkLogFileSize(const char *pathfile)
{
	ULONGLONG nSize=0;
	if( CFileUtil::GetSizeOfFile(pathfile, &nSize))
	{
		if (nSize>MAX_FILE_SIZE)
		{
// 			string szBak1(pathfile);
// 			szBak1 += ".1";
// 			if (PathFileExists(szBak1.c_str()))
// 			{
// 				string szBak2(pathfile);
// 				szBak2 += ".2";
// 				if (PathFileExists(szBak2.c_str()))
// 				{
// 					string szBak3(pathfile);
// 					szBak3 += ".3";
// 					if (PathFileExists(szBak3.c_str()))
// 					{
// 						string szBak4(pathfile);
// 						szBak4 += ".4";
// 						if (PathFileExists(szBak4.c_str()))
// 						{
// 							DeleteFile(szBak4.c_str());
// 						}
// 						rename(szBak3.c_str(), szBak4.c_str());
// 					}
// 					rename(szBak2.c_str(), szBak3.c_str());
// 				}
// 				rename(szBak1.c_str(), szBak2.c_str());
// 			}
// 			rename(pathfile, szBak1.c_str());
// 		
		
			int i;
			CString szBak;
			for (i=1; i<MAX_LOG_BAKCUP; ++i)
			{
				szBak.Format("%s.%d.log", pathfile, i);
				if (!PathFileExists(szBak)) {
					break;
				}
			}
			if (i==MAX_LOG_BAKCUP) { 
				DeleteFile(szBak); 
				--i; 
			}

			for (int k=i-1; k>0; --k)
			{
				szBak.Format("%s.%d.log", pathfile, k);
				CString szBak2;
				szBak2.Format("%s.%d.log", pathfile, k+1);
				rename(szBak, szBak2);
			}
			CString szBak2;
			szBak2.Format("%s.1.log", pathfile);
			rename(pathfile, szBak2);
		}
	}
	
}



void ErrorMessageBox(DWORD last_error) 
{ 
	CString str = ErrorMessageStr(last_error);
	MessageBox(NULL, (LPCTSTR)str, TEXT("错误提示"), MB_OK); 


}

CString ErrorMessageStr(DWORD errorNo /* = 0 */) 
{ 
	// Retrieve the system error message for the last-error code
	CString result;

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = errorNo;
	if (!dw) dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf)+1024)*sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf),
		TEXT("系统错误（%d）: %s"), 
		dw, lpMsgBuf); 
	//MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("错误提示"), MB_OK); 
	result = (LPTSTR)lpDisplayBuf;

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	//	ExitProcess(dw); 
	return result;
}


string & TrimLeft( string &str, char ch )
{
	// 去掉前的空格
	str.erase(0, str.find_first_not_of(ch));
	return str;
}

string & TrimRight( string &str , char ch)
{
	string::size_type k = str.find_last_not_of(ch);
	if (k!=string::npos)
		str.erase(k+1);
	else
		str.clear();

	return str;
}

string & Trim( string &str, char ch )
{
	return TrimLeft(TrimRight(str, ch), ch);
}


std::string NumToStr(int x, int width, char fillwith)
{
	stringstream ss;
	ss.fill(fillwith);
	if (width>0) ss << setw(width);
	ss << x;
	return ss.str();
}

string NumToStrHex( int x, int width/*=0*/, char fillwith/*='0'*/ )
{
	stringstream ss;
	ss.fill(fillwith);
	if (width>0) ss  << setw(width);
	ss << hex << x;
	return ss.str();
}


static CMyLog *pMyLog=NULL;
DLL_EXP_QLIB void MyLogInit( LPCTSTR szFilename )
{
	if (pMyLog == NULL) pMyLog = new CMyLog( szFilename );
}

DLL_EXP_QLIB void MyLogRelease( void )
{
	if (pMyLog) {
		delete pMyLog;
		pMyLog = NULL;
	}
}

DLL_EXP_QLIB void MyLogInfo( const char *szFormat, ... )
{
	/// 没有初始化，无法进行日志输出
	if (!pMyLog)
		return;

	va_list   vlist;
	va_start( vlist, szFormat );

#define MAX_STR_BUFF 64*1014
	static char str[MAX_STR_BUFF] = { 0 };
	vsprintf_s( str, MAX_STR_BUFF, szFormat, vlist );

	LOG_INFO( str );
}

void MyLogBuff( const byte *szBuff, int buffLen, const char *szMsg )
{
	LOG_BUFF( (void*)szBuff, buffLen, szMsg );
}

