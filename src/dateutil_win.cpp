/************************************************************************/
/* author: qhb                                                          */
/* author: quhuabo@gmail.com                                            */
/************************************************************************/
#include "stdafx.h"

#include "dateutil_win.h"


#pragma warning(disable: 4996)


CDateTime::CDateTime( bool bNow  )
{
	if (bNow)
		*this = Now();
	else
		year = mon = day = hour = min = sec = 0;
	
}

CDateTime::CDateTime( const char *str )
{
	fromStr(str);
}

CDateTime::CDateTime(WORD AYear, BYTE AMon, BYTE ADay, BYTE AHour /*= 0*/, BYTE AMin /*= 0*/, BYTE ASec /*= 0*/)
	:year(AYear), mon(AMon), day(ADay), hour(AHour), /*min(AMin),*/sec(ASec)
{
	min = AMin;
}

CDateTime::CDateTime(int nSec)
{
	FromTimeT2(nSec);
}

MyString CDateTime::toStr( char delimiter/*=':'*/ ) const
{
	char buff[64]="";
    sprintf_s(buff, 64, "%04d-%02d-%02d %02d%c%02d%c%02d", 
		year, mon, day, hour, delimiter, min, delimiter, sec); 
	string str = buff;
    return str;

}


CDateTime & CDateTime::fromStr( const std::string &str )
{
	int n_year=0,n_mon=1,n_day=1, n_hour=0,n_min=0, n_sec=0;
	sscanf_s(str.c_str(),  "%04d-%02d-%02d %02d:%02d:%02d", &n_year, &n_mon, &n_day, &n_hour, &n_min, &n_sec); 
	year = (WORD)n_year; mon = (BYTE)n_mon; day = (BYTE)n_day; hour = (BYTE)n_hour; min = (BYTE)n_min; sec = (BYTE)n_sec;
 
	
	return *this;
}

CDateTime CDateTime::Now( void )
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CDateTime dt;
	dt.FromWinTime(sys);
	return dt;
}


void CDateTime::FromWinTime( const SYSTEMTIME & _dtFrom ) 
{
	this->year = _dtFrom.wYear;                
	this->mon = (BYTE)_dtFrom.wMonth;                
	this->day = (BYTE)_dtFrom.wDay;                  
	this->hour = (BYTE)_dtFrom.wHour;                
	this->min = (BYTE)_dtFrom.wMinute;               
	this->sec = (BYTE)_dtFrom.wSecond;               
	
}

void CDateTime::ToWinTime( SYSTEMTIME & _dtTo ) const
{
	_dtTo.wYear     = this->year;             
	_dtTo.wMonth    = this->mon;            
	_dtTo.wDay      = this->day;              
	_dtTo.wHour     = this->hour;             
	_dtTo.wMinute   = this->min;           
	_dtTo.wSecond   = this->sec;           
	_dtTo.wMilliseconds = 0;        
}

CDateTime & CDateTime::TruncTime( void )
{
	this->hour = 0;
	this->min = 0;
	this->sec = 0;
	return *this;
}

CDateTime & CDateTime::AddSeconds( int nSec )
{
	FILETIME ft;
	SYSTEMTIME st;
	__int64* pi; 
	ToWinTime(st);
	SystemTimeToFileTime (&st,&ft); 
	pi = (__int64*)&ft; 
	(*pi) += nSec*(__int64)10000000;	// 100 个纳秒 

	/*convert FAILETIME to SYSTEMTIME*/
	FileTimeToSystemTime (&ft,&st); 
	FromWinTime(st);
	return *this;

}

void CDateTime::FromTimeT( const time_t *t )
{
	struct tm p;
	localtime_s(&p, t);
	this->year = (WORD)p.tm_year + 1900;
	this->mon = (BYTE)p.tm_mon + 1;
	this->day = (BYTE)p.tm_mday;
	this->hour = (BYTE)p.tm_hour;
	this->min = (BYTE)p.tm_min;
	this->sec = (BYTE)p.tm_sec;

}

void CDateTime::FromTimeT2( int t )
{
	time_t tt(t);
	FromTimeT(&tt);
}

time_t CDateTime::ToTimeT( ) const
{
	struct tm p;
	
	p.tm_year = this->year - 1900;
	p.tm_mon = this->mon - 1;
	p.tm_mday = this->day ;
	p.tm_hour = this->hour;
	p.tm_min = this->min ;
	p.tm_sec = this->sec;

	return mktime(&p);
}

CDateTime::~CDateTime()
{

}

DLL_EXP_QLIB CDateTime& CDateTime::operator+=(int nSec)
{
	return AddSeconds(nSec);
}

DLL_EXP_QLIB CDateTime & CDateTime::operator-=(int nSec)
{
	return AddSeconds(-nSec);
}

INT64 CDateTime::Day(int x)
{
	return x*SEC_IN_DAY;
}

INT64 operator-( const CDateTime &a, const CDateTime &b )
{
	FILETIME ft1, ft2;
	SYSTEMTIME st1, st2;

	a.ToWinTime(st1);
	b.ToWinTime(st2);
	SystemTimeToFileTime(&st1, &ft1);
	SystemTimeToFileTime(&st2, &ft2);

	/** 返回两个时间差的秒数  */
	return (*(__int64*)&ft1 - *(__int64*)&ft2)/10000000;
}

bool operator>( const CDateTime &a, const CDateTime &b )
{
	return (a.year > b.year)
		|| ((a.year == b.year) && (a.mon > b.mon))
		|| ((a.year == b.year) && (a.mon == b.mon) && (a.day > b.day))
		|| ((a.year == b.year) && (a.mon == b.mon) && (a.day == b.day) && (a.hour > b.hour))
		|| ((a.year == b.year) && (a.mon == b.mon) && (a.day == b.day) && (a.hour == b.hour) && (a.min > b.min))
		|| ((a.year == b.year) && (a.mon == b.mon) && (a.day == b.day) && (a.hour == b.hour) && (a.min == b.min) && (a.sec > b.sec));

}

bool operator<( const CDateTime &a, const CDateTime &b )
{
	return (a.year < b.year)
		|| ((a.year == b.year) && (a.mon < b.mon))
		|| ((a.year == b.year) && (a.mon == b.mon) && (a.day < b.day))
		|| ((a.year == b.year) && (a.mon == b.mon) && (a.day == b.day) && (a.hour < b.hour))
		|| ((a.year == b.year) && (a.mon == b.mon) && (a.day == b.day) && (a.hour == b.hour) && (a.min < b.min))
		|| ((a.year == b.year) && (a.mon == b.mon) && (a.day == b.day) && (a.hour == b.hour) && (a.min == b.min) && (a.sec < b.sec));

}

bool operator == (const CDateTime &a, const CDateTime &b)
{
	return a.year == b.year 
		&& a.mon == b.mon 
		&& a.day == b.day
		&& a.hour == b.hour 
		&& a.min == b.min 
		&& a.sec == b.sec;

}

 CDateTime operator-( const CDateTime &a ,int nSec )
{
	CDateTime ret(a);
	ret.AddSeconds(-nSec);
	return ret;
	
}

 CDateTime operator + (const CDateTime &a ,int nSec)
{
	CDateTime ret(a);
	ret.AddSeconds(nSec);
	return ret;
 }

 bool operator>=(const CDateTime &a, const CDateTime &b)
 {
	 //return a - b >= 0;
	 return a == b || a > b;
 }

 bool operator<=(const CDateTime &a, const CDateTime &b)
 {
	 return a == b || a < b;
 }

