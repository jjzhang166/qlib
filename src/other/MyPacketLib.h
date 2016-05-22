#pragma once
#include <string>
#include <vector>

class CNetCardItem
{
public:
	std::string name;
	std::string desc;
	std::string ip;
};


class CMyPacketLib
{
public:
	CMyPacketLib(void);
	~CMyPacketLib(void);
	
	std::vector<CNetCardItem> m_cards;
	void loadCardsInfo();
	std::string findIpByDesc(LPCTSTR cardDesc);
};

