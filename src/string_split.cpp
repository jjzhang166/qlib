#include "stdafx.h"
#include <iostream>   
#include <string>   
#include <vector>   
#include <string_split.h>
using namespace std;   



CMySplit::CMySplit()
{
	m_pos = -1;
}

CMySplit::CMySplit( const string & str, const string &separate_character )
{
	CMySplit();
	doParse(str, separate_character);
}


CMySplit::~CMySplit()
{

}

int CMySplit::doParse( const string & str, const string &separate_character )
{
	m_data = splitEx(str, separate_character);
	if (m_data.size()>0) m_pos = 0;
	else m_pos = -1;

	return m_data.size();
}

bool CMySplit::hasToken() const
{
	return m_pos>=0 && (unsigned)m_pos<m_data.size();
}

std::string CMySplit::getToken() 
{
	string str;
	if (m_pos>=0 && (unsigned)m_pos < m_data.size())
	{
		str = m_data.at(m_pos);
		m_pos ++;
	}
	
	return str;
}

vector<string> splitEx(const string& src, string separate_character)   
{   
	vector<string> strs;   

	int separate_characterLen = separate_character.size();//分割字符串的长度,这样就可以支持如“,,”多字符串的分隔符   
	int lastPosition = 0,index = -1;   
	while (-1 != (index = src.find(separate_character,lastPosition)))   
	{   
		strs.push_back(src.substr(lastPosition,index - lastPosition));   
		lastPosition = index + separate_characterLen;   
	}   
	string lastString = src.substr(lastPosition);//截取最后一个分隔符后的内容   
	if (!lastString.empty())   
		strs.push_back(lastString);//如果最后一个分隔符后还有内容就入队   
	return strs;   
}   