#pragma once
#include <list>
#include "qlibdef.h"

using namespace std;
class DLL_EXP_QLIB CSingleSelectItem
{
public:
	CSingleSelectItem();
	~CSingleSelectItem();

	bool isSelected() const { return m_selected; }
	void setSelected(bool val);

	virtual void OnSelected() = NULL;
private:
	bool m_selected;
};

typedef list<CSingleSelectItem*> SingleSelectList;
class DLL_EXP_QLIB CSingleSelect
{
	
public:
	CSingleSelect(void);
	~CSingleSelect(void);

	void AddItem(CSingleSelectItem *item);
	void AddItem(CSingleSelectItem *itemBegin, CSingleSelectItem *itemEnd);

	void SelectItem(CSingleSelectItem *item);
	int getSelectIndex() const;

private:
	SingleSelectList m_list;
	int m_selectedIndex;
};
