// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "stdafx.h"
#include "..\Container\CContainer.h"
#include "..\BaseObject.h"
#include "CContainerListener.h"



//--------------------------------------------------------------------------------------
// ����:	�������һ������
// ����:	��
// ����ֵ:	��
// ������	����TRUE����������ɷ�ִ��,FALSE����ֹ�ɷ�
//--------------------------------------------------------------------------------------
//##ModelId=41A4308B01B5
BOOL CContainerListener::OnObjectAdded(CContainer* pContainer, CBaseObject* pObj, LPVOID pVoid)
{
	return TRUE;
}

//--------------------------------------------------------------------------------------
// ����:	����ɾ��һ������
// ����:	��
// ����ֵ:	��
// ������	����TRUE����������ɷ�ִ��,FALSE����ֹ�ɷ�
//--------------------------------------------------------------------------------------
//##ModelId=41A430B10232
BOOL CContainerListener::OnObjectRemoved(CContainer* pContainer, CBaseObject* pObj, LPVOID pVoid)
{
	return TRUE;
}

//--------------------------------------------------------------------------------------
// ����:	��������һ������
// ����:	��
// ����ֵ:	��
// ������	����TRUE����������ɷ�ִ��,FALSE����ֹ�ɷ�
//--------------------------------------------------------------------------------------
//##ModelId=41A43146003E
BOOL CContainerListener::OnTraversingContainer(CContainer* pContainer, CBaseObject* pObject)
{
	return TRUE;
}

