#pragma once
#include "jpgctrl.h"
#include "MyLayout.h"
#include <afxdialogex.h>

// CDlgJpgPreview �Ի���

class CDlgJpgPreview : public CDialogEx, public CMyLayout
{
	DECLARE_DYNAMIC(CDlgJpgPreview)

public:
	CDlgJpgPreview(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgJpgPreview();

// �Ի�������
	enum { IDD = IDD_DLG_JPG_PREVIEW };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CJpgCtrl m_jpg;
	void doMyInitial();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
