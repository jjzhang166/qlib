#include "StdAfx.h"
#include "MyPacketLib.h"
#include "pcap.h"

#pragma comment(lib, "wpcap.lib")
#pragma comment(lib, "Packet.lib")


CMyPacketLib::CMyPacketLib(void)
{
}


CMyPacketLib::~CMyPacketLib(void)
{
}

/* 将数字类型的IP地址转换成字符串类型的 */
#define IPTOSBUFFERS 12
char *iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3*4+3+1];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}
CString convertSockAddr(struct pcap_addr *addr)
{
	CString str;
	for (pcap_addr_t * a = addr; a; a = addr->next)
	{
		if (!a->addr) continue;
		if (a->addr->sa_family  == AF_INET) {
			str = iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr);
			break;
		}

	}
	return str;
}
void CMyPacketLib::loadCardsInfo()
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int i=0;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* 获取本地机器设备列表 */
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs_ex: %s\n", errbuf);
		
	}


	m_cards.clear();
	/* 打印列表 */
	for(d= alldevs; d != NULL; d= d->next)
	{
		CNetCardItem item;
		CString str;
		item.name = d->name;
		item.ip = convertSockAddr(d->addresses);

		if (d->description) {
			item.desc = d->description;
		}
		m_cards.push_back(item);

	}

	if (i == 0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return;
	}

	/* 不再需要设备列表了，释放它 */
	pcap_freealldevs(alldevs);
}

std::string CMyPacketLib::findIpByDesc( LPCTSTR cardDesc )
{
	for (auto i=m_cards.begin(); i!=m_cards.end(); ++i)
	{
		if (i->desc == cardDesc)
		{
			return i->ip;

		}
	}
	return "";
}
