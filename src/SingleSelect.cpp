#include "StdAfx.h"
#include "SingleSelect.h"
#include "MyLog.h"


CSingleSelectItem::CSingleSelectItem()
{
	m_selected = false;
	
}

CSingleSelectItem::~CSingleSelectItem()
{

}

void CSingleSelectItem::setSelected( bool val )
{
	if (m_selected != val) { 
		m_selected = val; 
		OnSelected();
	}
}


CSingleSelect::CSingleSelect(void)
{
	m_selectedIndex = -1;	
}

CSingleSelect::~CSingleSelect(void)
{
}

void CSingleSelect::AddItem( CSingleSelectItem *item )
{
	item->setSelected(false);
	m_list.push_back(item);
}

void CSingleSelect::AddItem( CSingleSelectItem *itemBegin, CSingleSelectItem *itemEnd )
{
	for (CSingleSelectItem *i=itemBegin; i!=itemEnd; ++i)
	{
		AddItem(i);
	}
}

void CSingleSelect::SelectItem( CSingleSelectItem *item )
{
	item->setSelected(true);
	int k=0;
	for (SingleSelectList::iterator i=m_list.begin();i!=m_list.end(); ++i, ++k)
	{
		if ((*i)->isSelected() && (*i!=item)) (*i)->setSelected(false);
		if ((*i)==item)
			m_selectedIndex = k;
	}

	//LOG_INFO("select %d", m_selectedIndex);
}

int CSingleSelect::getSelectIndex() const
{
	return m_selectedIndex;
}

