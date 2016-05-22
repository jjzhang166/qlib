#include "stdafx.h"
#include "..\include\MyButtonManager.h"



CMyButtonManager::CMyButtonManager()
{
	m_pJpgCtrl = NULL;
	m_pImgManager = NULL;
}


CMyButtonManager::~CMyButtonManager()
{
}

void CMyButtonManager::Initial(CJpgCtrl *pBackJpg, CImageManager *pImgManager)
{
	m_pJpgCtrl = pBackJpg;
	m_pImgManager = pImgManager;
}

void CMyButtonManager::AddButton(CMyButton *pButton, LPCTSTR szNormal, LPCTSTR szHover, LPCTSTR szPressed, LPCTSTR szTooltips)
{
	ASSERT(m_pJpgCtrl);
	ASSERT(m_pImgManager);
	//pButton->setBackground(m_pJpgCtrl);
	pButton->setButtonManager(this);
	Gdiplus::Image *pTemp = m_pImgManager->getImageByKey(szPressed);
	if (!szPressed || szPressed == "")
		pTemp = NULL;
	pButton->setImage(m_pImgManager->getImageByKey(szNormal), m_pImgManager->getImageByKey(szHover), pTemp);
	if (szTooltips){
		pButton->SetToolTipText(szTooltips);
	}
	m_buttons.push_back(pButton);
}

void CMyButtonManager::AddButton(CMyButton *pButton, LPCTSTR szNormal, LPCTSTR szTooltips /*= NULL*/)
{
	ASSERT(pButton);
	pButton->setButtonManager(this);
	pButton->setImage(m_pImgManager->getImageByKey(szNormal));
	if (szTooltips){
		pButton->SetToolTipText(szTooltips);
	}
	m_buttons.push_back(pButton);
}

void CMyButtonManager::AddButton(CMyButton *pButton, Color colorNormal, Color colorHover, Color colorPress)
{
	ASSERT(pButton);
	pButton->setButtonManager(this);
	pButton->setColorButton(colorNormal, colorHover, colorPress);
	m_buttons.push_back(pButton);
}

void CMyButtonManager::RefeshBackground()
{
	ASSERT(m_pJpgCtrl);
	for (auto i = m_buttons.begin(); i!=m_buttons.end(); ++i){
		(*i)->setBackground(m_pJpgCtrl);
	}
}

void CMyButtonManager::ClearButtons()
{
	m_buttons.clear();
}

CJpgCtrl * CMyButtonManager::getBackJpgCtrl()
{
	return m_pJpgCtrl;
}

void CMyButtonManager::AddCheckButton(CMyButton *pButton, LPCTSTR szCheckOff, LPCTSTR szCheckOn, LPCTSTR szTooltips /*= NULL*/)
{

	ASSERT(pButton);
	pButton->setButtonManager(this);
	pButton->setImage(m_pImgManager->getImageByKey(szCheckOff), m_pImgManager->getImageByKey(szCheckOn));
	if (szTooltips){
		pButton->SetToolTipText(szTooltips);
	}
	m_buttons.push_back(pButton);

}
