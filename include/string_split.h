/************************************************************************/
/* author: qhb                                                          */
/* author: quhuabo@gmail.com                                            */
/************************************************************************/
#ifndef _STRING_SPLIT_H_
#define _STRING_SPLIT_H_
#include <string>   
#include <vector>   
#include "qlibdef.h"
using namespace std;   

typedef vector<string> VectorString;


/** 分隔字符串到数组中 */
DLL_EXP_QLIB vector<string> splitEx(const string& src, string separate_character);


class DLL_EXP_QLIB CMySplit
{
public:
	CMySplit();
	CMySplit(const string & str, const string &separate_character);
	~CMySplit();

	int doParse(const string & str, const string &separate_character);
	bool hasToken() const;
	string getToken();
private:
	vector<string> m_data;
	int m_pos;
};

#endif