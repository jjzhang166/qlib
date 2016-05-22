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
	/* 按钮的背景控件，和图片的管理器                                       */
	/************************************************************************/
	void Initial(CJpgCtrl *pBackJpg, CImageManager *pImgManager);

	/************************************************************************/
	/* 添加一个Button，同时设置该Button的背景和三个状态的图片Key，
	   该 Key 来自于 m_pImgManager 中添加图片时自定义的字符串 */
	/************************************************************************/
	void AddButton(CMyButton *pButton, LPCTSTR szNormal, LPCTSTR szHover, LPCTSTR szPressed, LPCTSTR szTooltips=NULL);
	void AddButton(CMyButton *pButton, LPCTSTR szNormal, LPCTSTR szTooltips = NULL);
	void AddCheckButton(CMyButton *pButton, LPCTSTR szCheckOff, LPCTSTR szCheckOn, LPCTSTR szTooltips = NULL);

	void AddButton(CMyButton *pButton, Color colorNormal, Color colorHover, Color colorPress);

	/************************************************************************/
	/* 刷新所有按钮背景图片，当更改背景控件中的图片时调用                   */
	/************************************************************************/
	void RefeshBackground();

	/************************************************************************/
	/* 清除按钮所有按钮                                                     */
	/************************************************************************/
	void ClearButtons();

	/************************************************************************/
	/* 获得背景图片控件                                                     */
	/************************************************************************/
	CJpgCtrl *getBackJpgCtrl();
private:
	CJpgCtrl *m_pJpgCtrl;
	CImageManager *m_pImgManager;
	list<CMyButton*> m_buttons;

};

