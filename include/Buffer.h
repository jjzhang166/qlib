/************************************************************************/
/* author: qhb                                                          */
/* author: quhuabo@gmail.com                                            */
/************************************************************************/
#pragma once
#include "qlibdef.h"
typedef unsigned char BYTE;

#include <vector>
#include <string>
#include <cassert>
using namespace std;

class DLL_EXP_QLIB CBuffer
{
public:
	CBuffer(int n=0, void *srcBuffer=NULL);
	CBuffer(const CBuffer &, int start=0, int len = -1);
	CBuffer &operator=(const CBuffer &);
	
	~CBuffer(void);

	const BYTE* Buffer()const;
	BYTE* Buffer();

	void Create(int n);
	int Size() const;

	bool IsEmpty() const;
	void Clear();

	CBuffer &Copy(const void *srcBuffer, int n);
	CBuffer &Copy(const CBuffer &src);
	CBuffer &Copy(const CBuffer &src, int start, int len=-1);

	CBuffer &Append(const void *srcBuffer, int n);
	CBuffer &Append(const CBuffer &src);
	CBuffer &Append(char ch);

	CBuffer &PopFront(int cnt);
	bool ReadFile(const string &fireToLoad);
	bool WriteFile(const string &fileToWrite);

	unsigned char operator[](int index);
	//vector<BYTE>::iterator begin();
	//vector<BYTE>::iterator end();
protected:

private:
	vector<BYTE> m_buffer;
};

