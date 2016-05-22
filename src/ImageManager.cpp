#include "StdAfx.h"
#include "ImageManager.h"
#include <atlconv.h>

static CImageManager *g_imageManager;

CImageManager & globalImageManager()
{
	return *g_imageManager;
}

CImageItem::CImageItem():image(NULL)
{

}

CImageItem::~CImageItem()
{
	
}

CImageManager::CImageManager(void)
{
}


CImageManager::~CImageManager(void)
{
	TRACE("\nclear Imagemanger resource....");
	Clear();	
}

void CImageManager::AddImage( const CString &szKey, const CString &szImageFile )
{
	CImageItem item;
	item.fileName = szImageFile;
USES_CONVERSION;
	CStringW wstr= A2W(szImageFile);
	item.image = new Gdiplus::Image(wstr);

	m_images.insert(make_pair(szKey, item));
}

Gdiplus::Image * CImageManager::getImage( const CString &szImageFile ) const
{
	for (auto i=m_images.begin(); i!=m_images.end(); ++i)
	{
		if (szImageFile == i->second.fileName)
			return i->second.image;
	}
	return NULL;
}

Gdiplus::Image * CImageManager::getImageByKey( const CString &szKey ) const
{
	auto iFind = m_images.find(szKey);
	if (iFind!=m_images.end())
	{
		return iFind->second.image;
	}
	return NULL;
}

void CImageManager::Clear()
{

	for (auto i=m_images.begin(); i!=m_images.end(); ++i)
	{
		if (i->second.image) {
			//i->second.image->
			delete i->second.image;
		}
  	}
	m_images.clear();
}

bool CImageManager::AddImageFromResource( LPCTSTR szKey, int resID, LPCTSTR resType, HINSTANCE hInst/*=NULL*/ )
{
	HRSRC hResource = ::FindResource(hInst, MAKEINTRESOURCE(resID), resType);
	if (!hResource)
		return false;

	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)
		return false;

	const void* pResourceData = ::LockResource(::LoadResource(hInst, 
		hResource));
	if (!pResourceData)
		return false;


	HGLOBAL hBuffer = ::GlobalAlloc(GMEM_FIXED, imageSize);
	if (NULL == hBuffer)
		return false;

	void* pBuffer = ::GlobalLock(hBuffer);
	if (pBuffer) {

		CopyMemory(pBuffer, pResourceData, imageSize);

		CImageItem item;
		item.fileName.Format("%s-%d", resType, resID);

		IStream* pStream = NULL;
		if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
		{
			item.image = Gdiplus::Image::FromStream(pStream);
			pStream->Release();

			m_images.insert(make_pair(szKey, item));
		}
		::GlobalUnlock(hBuffer);
	}
	::GlobalFree(hBuffer);

	return true;
}

CImageManager* CImageManager::Initial()
{
	ASSERT(!g_imageManager);
	g_imageManager = new CImageManager();
	return g_imageManager;
}

void CImageManager::Release()
{
	if (g_imageManager) {
		delete g_imageManager;
		g_imageManager = NULL;
	}
}
