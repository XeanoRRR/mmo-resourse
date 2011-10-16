#pragma once
#ifndef _INC_CVARLIST_H
#define _INC_CVARLIST_H

class CVarList
{
private:
	CVarList(void) {};
	virtual ~CVarList(void) {};

public:
	static CVarList* GetInstance();
	static void Release();

private:
	static CVarList *instance;

public:
	/*static*/ CVariableList* GetGeneralVariableList()	{return m_pVariableList;}

private:
	/*static*/ CVariableList *m_pVariableList;		// ȫ�ֽű�����
};


#endif // _INC_CVARLIST_H
