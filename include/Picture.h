// Picture.h: interface for the CPicture class. 
// 
////////////////////////////////////////////////////////////////////// 

#if !defined(AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_) 
#define AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_ 
#include "qlibdef.h"
#if _MSC_VER > 1000 
#pragma once 
#endif // _MSC_VER > 1000 

class DLL_EXP_QLIB CPicture   
{ 
public: 
	CPicture(); 
	virtual ~CPicture(); 

	bool			Load(CString sResourceType, CString sResource); 
	bool			Load(CString sFileName); 
	void			UnLoad(); 

	bool			Draw(CDC* pDC); 
	bool			Draw(CDC* pDC, CPoint Pos); 
	bool			Draw(CDC* pDC, CPoint Pos, CSize Size, bool bFill = false); 
	

	bool			Draw(CDC* pDC, double nSizeRatio); 
	bool			Draw(CDC* pDC, CPoint Pos, double nSizeRatio); 
	CSize			GetSize(CDC* pDC); 
protected:
	static	bool	GetResource(LPSTR lpName, LPSTR lpType, void* pResource, int& nBufSize); 
	bool			LoadFromBuffer(BYTE* pBuff, int nSize); 
	bool			Draw(CDC* pDC, int x, int y, int cx, int cy); 
	IPicture*		m_pPicture; 
	enum 
	{ 
		HIMETRIC_INCH	= 2540 
	}; 
}; 

#endif // !defined(AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_) 


