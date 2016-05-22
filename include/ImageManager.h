#pragma once
#include "../stdafx.h"
#include <list>
#include <map>

#include <gdiplus.h>
#include "qlibdef.h"
using namespace  std;

class DLL_EXP_QLIB CImageItem
{
public:
	CImageItem();
	~CImageItem();

	CString fileName;
	Gdiplus::Image *image;
};

class DLL_EXP_QLIB CImageManager
{
public:
	static CImageManager* Initial();
	static void Release();

	~CImageManager(void);

	bool AddImageFromResource(LPCTSTR szKey, int resID, LPCTSTR resType, HINSTANCE hInst=NULL);

	void AddImage(const CString &szKey, const CString &szImageFile);
	Gdiplus::Image *getImage(const CString &szImageFile) const;
	Gdiplus::Image *getImageByKey(const CString &szKey) const;

	/** ������� gdiplus::shutdown ֮ǰ����, �������� */
	void Clear();
protected:
	CImageManager(void);

private:
	map<CString, CImageItem> m_images;
};


DLL_EXP_QLIB CImageManager  &  globalImageManager();
