/************************************************************************/
/* author: qhb                                                          */
/* author: quhuabo@gmail.com                                            */
/************************************************************************/
#ifndef _DATEWIN__WIN__HPP_
#define _DATEWIN__WIN__HPP_

#include <windows.h>
#include <string>
#include "qlibdef.h"
#include <afxstr.h>
using namespace std;

#define MyString string

#define SEC_IN_MIN		(INT64)60
#define SEC_IN_HOUR		(INT64)3600
#define SEC_IN_DAY		(INT64)24*3600
#define SEC_IN_WEEK		(INT64)7*24*3600

class DLL_EXP_QLIB CDateTime
{
public:
	static CDateTime Now( void );

	/************************************************************************/
	/* ����x�������                                                          */
	/************************************************************************/
	static INT64 Day(int x);


public:
	void FromWinTime(const SYSTEMTIME & _dtFrom);
	void ToWinTime(  SYSTEMTIME & _dtTo) const;	

	void FromTimeT(const time_t *t);
	void FromTimeT2(int t);
	time_t ToTimeT() const;

public:
	explicit CDateTime(int nSec);
	explicit CDateTime(bool bNow = false);
	CDateTime(const char *str);
	CDateTime(WORD AYear, BYTE AMon, BYTE ADay, BYTE AHour = 0, BYTE AMin = 0, BYTE ASec = 0);

	virtual ~CDateTime();

	MyString toStr(char delimiter=':') const ;
	
	/************************************************************************/
	/* ��ʽ���£�2015-01-01 00:00:00                                          */
	/*         ����ǰ���0����ȥ��                                             */
	/************************************************************************/
	CDateTime &fromStr(const std::string &str);

	CDateTime &TruncTime(void);

	CDateTime &AddSeconds(int nSec);

	CDateTime & operator += (int nSec);
	CDateTime & operator -= (int nSec);


public:
	WORD year;              // �꣺2011
	BYTE mon,day;           // �£�1..12), �գ�1..31)
	BYTE hour,min,sec;      // ʱ��0..23), �֣�0..59), �루0..59��

};


 DLL_EXP_QLIB CDateTime operator - (const CDateTime &a ,int nSec);
 DLL_EXP_QLIB CDateTime operator + (const CDateTime &a ,int nSec);

 /** ��������ʱ��������  */
 DLL_EXP_QLIB INT64 operator - (const CDateTime &a, const CDateTime &b);

 DLL_EXP_QLIB bool operator == (const CDateTime &a, const CDateTime &b);
 DLL_EXP_QLIB bool operator > (const CDateTime &a, const CDateTime &b);
 DLL_EXP_QLIB bool operator < (const CDateTime &a, const CDateTime &b);
 DLL_EXP_QLIB bool operator >= (const CDateTime &a, const CDateTime &b);
 DLL_EXP_QLIB bool operator <= (const CDateTime &a, const CDateTime &b);

#endif