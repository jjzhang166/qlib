#pragma once
#include "jpgctrl.h"
#include "MyLayout.h"
#include <afxdialogex.h>

// CDlgJpgPreview 对话框

class CDlgJpgPreview : public CDialogEx, public CMyLayout
{
	DECLARE_DYNAMIC(CDlgJpgPreview)

public:
	CDlgJpgPreview(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgJpgPreview();

// 对话框数据
	enum { IDD = IDD_DLG_JPG_PREVIEW };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CJpgCtrl m_jpg;
	void doMyInitial();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
