// D:\qhb\share\DlgJpgPreview.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgJpgPreview.h"
#include "afxdialogex.h"


// CDlgJpgPreview �Ի���

IMPLEMENT_DYNAMIC(CDlgJpgPreview, CDialogEx)

CDlgJpgPreview::CDlgJpgPreview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgJpgPreview::IDD, pParent)
{
	

}

CDlgJpgPreview::~CDlgJpgPreview()
{
}

void CDlgJpgPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_JPG, m_jpg);
}


BEGIN_MESSAGE_MAP(CDlgJpgPreview, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgJpgPreview ��Ϣ�������



void CDlgJpgPreview::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	CMyLayout::OnSizeChanged(nType);
	// TODO: �ڴ˴������Ϣ����������
}

void CDlgJpgPreview::doMyInitial()
{
	CMyLayout::AddCtrl(&m_jpg, MLT_ALL, true);
}
