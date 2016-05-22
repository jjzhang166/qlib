#pragma once
#include <string>
#include "qlibdef.h"
using namespace std;

class DLL_EXP_QLIB CMyTool
{
public:

static string toIPText(DWORD ip);
static DWORD toIPDWord(const string& ip);

public:
	CMyTool(void);
	~CMyTool(void);


};

