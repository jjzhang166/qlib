// D:\qhb\share\DlgJpgPreview.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgJpgPreview.h"
#include "afxdialogex.h"


// CDlgJpgPreview 对话框

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


// CDlgJpgPreview 消息处理程序



void CDlgJpgPreview::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	CMyLayout::OnSizeChanged(nType);
	// TODO: 在此处添加消息处理程序代码
}

void CDlgJpgPreview::doMyInitial()
{
	CMyLayout::AddCtrl(&m_jpg, MLT_ALL, true);
}
