#include "StdAfx.h"
#include "MyTool.h"
#include <ws2tcpip.h>


CMyTool::CMyTool(void)
{
}


CMyTool::~CMyTool(void)
{
}

std::string CMyTool::toIPText( DWORD ip )
{
	in_addr in;
	in.S_un.S_addr = htonl(ip);

	char buff[256]="";
	inet_ntop( AF_INET, &in, buff, 256 );
	//return inet_ntoa(in);
	return buff;

}

DWORD CMyTool::toIPDWord( const string& ip )
{
	//return ntohl(inet_addr(ip.c_str()));

	in_addr in;
	inet_pton( AF_INET, ip.c_str(), &in );
	return in.S_un.S_addr;
}
