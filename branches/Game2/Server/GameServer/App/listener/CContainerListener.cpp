// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "StdAfx.h"
#include "../Container/CContainer.h"
#include "../BaseObject.h"
#include "CContainerListener.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------------
// ����:	�������һ������
// ����:	��
// ����ֵ:	��
// ������	����true����������ɷ�ִ��,FALSE����ֹ�ɷ�
//--------------------------------------------------------------------------------------
//##ModelId=41A4308B01B5
bool CContainerListener::OnObjectAdded(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid)
{
	return true;
}

//--------------------------------------------------------------------------------------
// ����:	����ɾ��һ������
// ����:	��
// ����ֵ:	��
// ������	����true����������ɷ�ִ��,FALSE����ֹ�ɷ�
//--------------------------------------------------------------------------------------
//##ModelId=41A430B10232
bool CContainerListener::OnObjectRemoved(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid)
{
	return true;
}

//--------------------------------------------------------------------------------------
// ����:	��������һ������
// ����:	��
// ����ֵ:	��
// ������	����true����������ɷ�ִ��,FALSE����ֹ�ɷ�
//--------------------------------------------------------------------------------------
//##ModelId=41A43146003E
bool CContainerListener::OnTraversingContainer(CContainer* pContainer, CBaseObject* pObject)
{
	return true;
}

