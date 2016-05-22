
// Picture.cpp: implementation of the CPicture class.   
//   
//////////////////////////////////////////////////////////////////////   

#include "stdafx.h"   
#include "Picture.h"   
#include "MyLog.h"

#ifdef _DEBUG   
#undef THIS_FILE   
static char THIS_FILE[]=__FILE__;   
#define new DEBUG_NEW   
#endif   

//////////////////////////////////////////////////////////////////////   
// Construction/Destruction   
//////////////////////////////////////////////////////////////////////   

CPicture::CPicture()   
{   
	m_pPicture  = NULL;   
}   

CPicture::~CPicture()   
{   
	UnLoad();   
}   

bool CPicture::Load(CString sResourceType, CString sResource)   
{   
	bool bResult = false;   

	if (m_pPicture != NULL)   
		UnLoad();   

	if (m_pPicture == NULL)   
	{   
		BYTE*   pBuff = NULL;   
		int     nSize = 0;   
		if (GetResource(sResource.GetBuffer(0), sResourceType.GetBuffer(0), pBuff, nSize))   
		{   
			if (nSize > 0)   
			{   
				pBuff = new BYTE[nSize];   

				if (GetResource(sResource.GetBuffer(0), sResourceType.GetBuffer(0), pBuff, nSize))   
				{   
					if (LoadFromBuffer(pBuff, nSize))   
						bResult = true;   
				}   

				delete [] pBuff;   
			}   
		}   
	}   
	return bResult;   
}   

bool CPicture::Load(CString sFileName)   
{   
	bool bResult = false;   

	if (m_pPicture != NULL)   
		UnLoad();   

	if (m_pPicture == NULL)   
	{   
		CFile           cFile;   
		CFileException  e;   

		if (cFile.Open(sFileName, CFile::modeRead | CFile::typeBinary, &e))   
		{   
			BYTE* pBuff = new BYTE[(unsigned)cFile.GetLength()];   

			if (cFile.Read(pBuff, (unsigned)cFile.GetLength()) > 0)   
			{   
				if (LoadFromBuffer(pBuff, (unsigned) cFile.GetLength()))   
					bResult = true;   
			}   

			delete [] pBuff;   
		}   
	}   
	return bResult;   
}   

void CPicture::UnLoad()   
{   
	if (m_pPicture != NULL)   
	{   
		m_pPicture->Release();   
		m_pPicture = NULL;   
	}   
}   

bool CPicture::Draw(CDC* pDC)   
{   
	if (m_pPicture != NULL)   
	{   
		long hmWidth;   
		long hmHeight;   
		m_pPicture->get_Width(&hmWidth);   
		m_pPicture->get_Height(&hmHeight);   

		int nWidth  = MulDiv(hmWidth,   pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);   
		int nHeight = MulDiv(hmHeight,  pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);   

		return Draw(pDC, 0, 0, nWidth, nHeight);   
	}   
	return false;   
}   

bool CPicture::Draw(CDC* pDC, CPoint Pos)   
{   
	if (m_pPicture != NULL)   
	{   
		long hmWidth;   
		long hmHeight;   
		m_pPicture->get_Width(&hmWidth);   
		m_pPicture->get_Height(&hmHeight);   

		int nWidth  = MulDiv(hmWidth,   pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);   
		int nHeight = MulDiv(hmHeight,  pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);   

		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);   
	}   
	return false;   
}   

bool CPicture::Draw(CDC* pDC, CPoint Pos, CSize Size, bool bFill /* = FALSE */)   
{   
	if (m_pPicture != NULL)   
	{   
		long hmWidth=0;   
		long hmHeight=0;   


		CSize szPic;
		szPic = GetSize(pDC);
		if (bFill) {
			//szPic = Size;
			//���ֱ���

			CSize szTemp(Size); // �õ���������С
			if (!szTemp.cx || !szTemp.cy)	
				return false;

			if ((double)szTemp.cx/szTemp.cy>(double)szPic.cx/szPic.cy)
			{
				szTemp.cx = (long)(Size.cy * (double)szPic.cx/szPic.cy);
				Pos.x += (Size.cx - szTemp.cx) /2;
			} else {

				szTemp.cy = (long) (Size.cx * (double)szPic.cy/szPic.cx);
				Pos.y += (Size.cy - szTemp.cy) /2;
			}
			Size = szTemp;
			return Draw(pDC, Pos.x, Pos.y, Size.cx, Size.cy);   
		}

		//m_pPicture->get_Width(&hmWidth);   
		//m_pPicture->get_Height(&hmHeight);   
		hmWidth = szPic.cx;
		hmHeight = szPic.cy;

		int nWidth  = Size.cx;   
		int nHeight = Size.cy;   

		//LOG_INFO("ԭ��С��%d x %d", hmWidth, hmHeight);

		// ͼƬû����
		if (nWidth>=hmWidth && nHeight>=hmHeight)
		{
			Pos.x = Pos.x+(nWidth-hmWidth)/2;
			Pos.y = Pos.y+(nHeight-hmHeight)/2;
			nWidth = hmWidth;
			nHeight = hmHeight;
		} else  // �߻����
		{
			double ratioH, ratioW;
			ratioH = nHeight*1.0/hmHeight;
			ratioW = nWidth*1.0/hmWidth;

			// ͼƬ�ȽϿ�
			if ( ratioH > ratioW )
			{
				int newHeight = (int)(hmHeight*ratioW);
				Pos.y += (nHeight - newHeight)/2;
				nHeight = newHeight;


			} else {
				int newWidth = (int)(hmWidth * ratioH);
				Pos.x += (nWidth - newWidth)/2;
				nWidth = newWidth;
			}

		}
		//LOG_INFO("�´�С��%d x %d", nWidth, nHeight);
		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);   
	}   
	return false;   
}   

bool CPicture::Draw(CDC* pDC, double nSizeRatio)   
{   
	if (m_pPicture != NULL)   
	{   
		long hmWidth;   
		long hmHeight;   
		m_pPicture->get_Width(&hmWidth);   
		m_pPicture->get_Height(&hmHeight);   

		int nWidth  = int(MulDiv(hmWidth,   pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH) * nSizeRatio);   
		int nHeight = int(MulDiv(hmHeight,  pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH) * nSizeRatio);   

		return Draw(pDC, 0, 0, nWidth, nHeight);   
	}   
	return false;   
}   

bool CPicture::Draw(CDC* pDC, CPoint Pos, double nSizeRatio)   
{   
	if (m_pPicture != NULL)   
	{   
		long hmWidth;   
		long hmHeight;   
		m_pPicture->get_Width(&hmWidth);   
		m_pPicture->get_Height(&hmHeight);   

		int nWidth  = int(MulDiv(hmWidth,   pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH) * nSizeRatio);   
		int nHeight = int(MulDiv(hmHeight,  pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH) * nSizeRatio);   

		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);   
	}   
	return false;   
}   

bool CPicture::Draw(CDC* pDC, int x, int y, int cx, int cy)   
{   
	long hmWidth;   
	long hmHeight;   
	m_pPicture->get_Width(&hmWidth);   
	m_pPicture->get_Height(&hmHeight);   

	if (m_pPicture->Render(pDC->m_hDC, x, y, cx, cy, 0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)   
		return true;   

	return false;   
}   

bool CPicture::LoadFromBuffer(BYTE* pBuff, int nSize)   
{   
// 	bool bResult = false;   
// 
// 	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);   
// 	void* pData = GlobalLock(hGlobal);   
// 	memcpy(pData, pBuff, nSize);   
// 	GlobalUnlock(hGlobal);   
// 
// 	IStream* pStream = NULL;   
// 
// 	if (CreateStreamOnHGlobal(pData, TRUE, &pStream) == S_OK)   
// 	{   
// 		HRESULT hr;   
// 		if ((hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_pPicture)) == S_OK)   
// 			bResult = true;   
// 
// 		pStream->Release();   
// 	}   
// 
// 	return bResult;   

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);

	if (hGlobal != NULL)

	{

		void* pData = GlobalLock(hGlobal);

		if (pData != NULL)

		{

			memcpy(pData, pBuff, nSize);

			GlobalUnlock(hGlobal);



			IStream* pStream = NULL;

			LOG_INFO("before create stream.");
			if (CreateStreamOnHGlobal(hGlobal, TRUE/**fDeleteOnRelease*/, &pStream) == S_OK)

			{
				LOG_INFO("end create stream.");
				// Not sure what the 'KeepOriginalFormat' property is really used for. But if 'OleLoadPicture'

				// is invoked with 'fRunmode=FALSE' the function always creates a temporary file which even

				// does not get deleted when all COM pointers were released. It eventually gets deleted only

				// when process terminated. Using 'fRunmode=TRUE' does prevent this behaviour and does not

				// seem to have any other side effects.

				VERIFY( OleLoadPicture(pStream, nSize, TRUE/**FALSE*/, IID_IPicture, (LPVOID*)&m_pPicture) == S_OK );

				pStream->Release();

			}

			else

				GlobalFree(hGlobal);

		}

		else

			GlobalFree(hGlobal);

	}
	return m_pPicture!=NULL;
}   

bool CPicture::GetResource(LPSTR lpName, LPSTR lpType, void* pResource, int& nBufSize)   
{    
	HRSRC       hResInfo;   
	HANDLE      hRes;   
	HMODULE     hInst   = NULL;    
	LPSTR       lpRes   = NULL;    
	int         nLen    = 0;   
	bool        bResult = FALSE;   

	// Find the resource   
	hResInfo = FindResource(hInst, lpName, lpType);   

	if (hResInfo == NULL)    
		return false;   

	// Load the resource   
	hRes = LoadResource(hInst, hResInfo);   

	if (hRes == NULL)    
		return false;   

	// Lock the resource   
	lpRes = (char*)LockResource(hRes);   

	if (lpRes != NULL)   
	{    
		if (pResource == NULL)   
		{   
			nBufSize = SizeofResource(hInst, hResInfo);   
			bResult = true;   
		}   
		else   
		{   
			if (nBufSize >= (int)SizeofResource(hInst, hResInfo))   
			{   
				memcpy(pResource, lpRes, nBufSize);   
				bResult = true;   
			}   
		}    

		UnlockResource(hRes);     
	}   

	// Free the resource   
	FreeResource(hRes);   

	return bResult;   
}   

CSize CPicture::GetSize(CDC* pDC)   
{   
	CSize rcResult = CSize(0,0);   

	if (m_pPicture != NULL)   
	{   
		long hmWidth;   
		long hmHeight;   
		
		m_pPicture->get_Width(&hmWidth);   
		m_pPicture->get_Height(&hmHeight);   

		rcResult.cx = MulDiv(hmWidth,   pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);   
		rcResult.cy = MulDiv(hmHeight,  pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);   
	}   

	return rcResult;   
}   



