/************************************************************************/
/* author: qhb                                                          */
/* author: quhuabo@gmail.com                                            */
/************************************************************************/


#include "stdafx.h"
#include <iterator>
#include <fstream>


#include "Buffer.h"

CBuffer::CBuffer( const CBuffer &src, int start, int len)
{
	Copy(src, start, len);
}

CBuffer::CBuffer( int n/*=0*/, void *srcBuffer/*=NULL*/ )
{
	if (n && srcBuffer) 
		Copy(srcBuffer, n);
	else if (n>0)
		Create(n);
}


CBuffer::~CBuffer(void)
{
	
}

void CBuffer::Create( int n )
{
	if (n>0)
		m_buffer.resize(n);
	else
		m_buffer.clear();
}

CBuffer &CBuffer::Copy( const void *srcBuffer, int n )
{
	if (n>0)
	{
		Create(n);
		copy((BYTE*)srcBuffer, (BYTE*)srcBuffer + n, m_buffer.begin());
	}
	
	return *this;
}

CBuffer & CBuffer::Copy( const CBuffer &src )
{
	return Copy(src.Buffer(), src.Size());
	
}

CBuffer & CBuffer::Copy( const CBuffer &src, int start, int len/*=-1*/ )
{
	int copyLen=len;
	if (len<0 || start+len>src.Size())
	{
		copyLen = (int)src.Size() - start;
	}
	
	return Copy(src.Buffer()+start, copyLen);
}

CBuffer& CBuffer::operator=( const CBuffer & src)
{
	if (this == &src)
		return *this;

	Copy(src);
	return *this;
}

CBuffer & CBuffer::Append( const void *srcBuffer, int n )
{
	if (n>0)
		copy((BYTE*)srcBuffer, (BYTE*)srcBuffer+n, back_inserter<vector<BYTE> >(m_buffer));
	return *this;
}

CBuffer & CBuffer::Append( const CBuffer &src )
{
	copy(src.m_buffer.begin(), src.m_buffer.end(), back_inserter<vector<BYTE> >(m_buffer))	;
	return *this;
}

CBuffer & CBuffer::Append( char ch )
{
	m_buffer.push_back((BYTE)ch);
	return *this;
}

const BYTE* CBuffer::Buffer() const
{
	return m_buffer.size()>0?&m_buffer[0]:(const BYTE*)NULL;;
}

BYTE* CBuffer::Buffer()
{
	return m_buffer.size()>0?&m_buffer[0]:(BYTE*)NULL;
}

bool CBuffer::IsEmpty() const
{
	return m_buffer.size()==0;
}

int CBuffer::Size() const
{
	return (int)m_buffer.size();
}

void CBuffer::Clear()
{
	m_buffer.clear();	
}

bool CBuffer::ReadFile( const string &filename )
{
	Clear();

	ifstream f(filename.c_str(), ios::binary);
	if (!f.good()) return false;
	f.seekg(0, ios::end);
	streamoff length = f.tellg();
	if (length>0){
		f.seekg(0, ios::beg);
		Create((int)length);
		if (this->Size()==length)
			f.read((char*)this->Buffer(), length);
		else
			return false;
	}
	return true;
}

CBuffer & CBuffer::PopFront( int cnt )
{
	if (cnt>=Size()) {
		m_buffer.clear();
		return *this;
	}

	for (int i=cnt; i<Size(); ++i)
	{
		m_buffer[i-cnt] = m_buffer[i];
	}
	m_buffer.resize(Size() - cnt);
	return *this;
}

unsigned char CBuffer::operator[]( int index )
{
	assert(index<(int)m_buffer.size());
	return m_buffer[index];
}

bool CBuffer::WriteFile( const string &fileToWrite )
{
	ofstream of(fileToWrite, ofstream::binary|ofstream::ate);
	of.write((const char*)Buffer(), Size());
	return of.good();
}

// vector<BYTE>::iterator CBuffer::begin()
// {
// 	return m_buffer.begin();
// }
// 
// vector<BYTE>::iterator CBuffer::end()
// {
// 	return m_buffer.end();
// }
