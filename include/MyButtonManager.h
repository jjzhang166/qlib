#pragma once
#include "ImageManager.h"
#include "MyButton.h"
#include <list>

using namespace std;
class DLL_EXP_QLIB CMyButtonManager
{
public:
	CMyButtonManager();;
	virtual ~CMyButtonManager();

	/************************************************************************/
	/* ��ť�ı����ؼ�����ͼƬ�Ĺ�����                                       */
	/************************************************************************/
	void Initial(CJpgCtrl *pBackJpg, CImageManager *pImgManager);

	/************************************************************************/
	/* ���һ��Button��ͬʱ���ø�Button�ı���������״̬��ͼƬKey��
	   �� Key ������ m_pImgManager �����ͼƬʱ�Զ�����ַ��� */
	/************************************************************************/
	void AddButton(CMyButton *pButton, LPCTSTR szNormal, LPCTSTR szHover, LPCTSTR szPressed, LPCTSTR szTooltips=NULL);
	void AddButton(CMyButton *pButton, LPCTSTR szNormal, LPCTSTR szTooltips = NULL);
	void AddCheckButton(CMyButton *pButton, LPCTSTR szCheckOff, LPCTSTR szCheckOn, LPCTSTR szTooltips = NULL);

	void AddButton(CMyButton *pButton, Color colorNormal, Color colorHover, Color colorPress);

	/************************************************************************/
	/* ˢ�����а�ť����ͼƬ�������ı����ؼ��е�ͼƬʱ����                   */
	/************************************************************************/
	void RefeshBackground();

	/************************************************************************/
	/* �����ť���а�ť                                                     */
	/************************************************************************/
	void ClearButtons();

	/************************************************************************/
	/* ��ñ���ͼƬ�ؼ�                                                     */
	/************************************************************************/
	CJpgCtrl *getBackJpgCtrl();
private:
	CJpgCtrl *m_pJpgCtrl;
	CImageManager *m_pImgManager;
	list<CMyButton*> m_buttons;

};

