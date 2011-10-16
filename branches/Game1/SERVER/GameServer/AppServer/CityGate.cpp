////////////////////////////////////////////////////////////////////////////
//CityGate.cpp/////////////////////////////////////////////////////////////
//������
//Author:Joe
//Create Time:2004.721
////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\citygate.h"
#include "Player.h"
#include "Monster.h"
#include "ServerRegion.h"
#include "script/Script.h"



/***********************************
/��
/
/**********************************/
CGate::CGate()
{
	m_lType=TYPE_CITYGATE;
}
CGate::~CGate()
{
	DelectAllScripByShape(GetExID());
}
BOOL CGate::InitProperty(CGateList::tagGate *p)
{
	SetName(p->cName);//����
	SetLenth(p->wLenth);//��		
	SetWidth(p->wWidth);//��
	SetLevel(p->wLevel);//�ȼ�
	SetGraphicsID(p->dwModelId);//ģ��ID
	SetMaxHP(p->wMaxHp);//���HP
	SetHP(p->wMaxHp);	//��ǰHP
	SetUnAttack(p->bIsunbeatable);//�Ƿ��޵�
	SetBlockType(p->byBlockType);//�赲����
	SetDef(p->lDef);//���	
	SetMagicDef(p->lMagicDef);//ħ��
	SetNameColor(p->wNameColor);//������ɫ
	SetHitSound(string(p->cHitSound));//������Ч
	SetDestorySound(string(p->cDestorySound));//�����Ч
	SetOpenSound(string(p->cOpenSound));//����Ч
	SetCloseSound(string(p->cCloseSound));//�ر���Ч
	return TRUE;
}

BOOL CGate::InitSetProperty(CServerRegion::tagGateProperty *p)
{
	SetIndex(p->lIndex);//���
	CShape::SetPosXY( (float)(p->lxPos + 0.5),(float)(p->lyPos + 0.5) );
	SetDir(p->lDir);//����
	CShape::SetState(p->wOpenState);//��״̬
	CShape::SetAction(p->wOpenState);//����
	SetAttackScp(p->strAttack);//�ܹ����ű�
	
	SetOpen(p->strOpen);//�򿪽ű�
	SetClose(p->strClose);//�رսű�
	SetDestoryScp(p->strDestory);//��ٽű�	
	SetRefreshScp(p->strRefresh);//��ˢ�ű�
	return TRUE;
}

//// ��ӵ�CByteArray
bool CGate::CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData)
{
	CBuild::CodeToDataBlock(setWriteDB,bExData);
	setWriteDB.AddToByteArray(m_strOpenSound.c_str());
	setWriteDB.AddToByteArray(m_strCloseSound.c_str());
	return true;
}

// ����
bool CGate::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData )
{
	CBuild::DecodeFromDataBlock(setReadDB, bExData);
	char strVal[256];
	setReadDB.GetStringFromByteArray( strVal,256);
	m_strOpenSound=strVal;
	setReadDB.GetStringFromByteArray( strVal,256);
	m_strCloseSound=strVal;
	return true;
}

void CGate::SetGate(WORD l)
{
	CBuild::SetBuild(l);
	switch (l)
	{
	case STATE_CLOSE:			//����Ϊ�ر�״̬	
		{
			//��ǰ״̬��Ϊ���
			if (m_wState!=STATE_DIED)
			{
				MoveAroundPlayer();
				//�����赲
				SetAttribute("Action",ACT_IDLE);
				SetAttribute("State",STATE_FIGHT);
				SetAction(ACT_IDLE);				
			}
		}
		break;
	case STATE_OPEN:			//����Ϊ��״̬	
		{
			if (m_wState!=STATE_DIED)
			{
				SetAttribute("Action",ACT_OPEN);
				SetAttribute("State",STATE_PEACE);
				SetAction(ACT_OPEN);			
			}
		}
		break;
	default:
		break;
	}
}

// �ɷ񹥻�
bool CGate::IsAttackAble(CMoveShape* pAttackShape)
{
	if(!GetUnAttack()) return false;
	if(pAttackShape == NULL)	return false;

	if(GetState() == STATE_DIED || IsDied()) return false;
	//���Ŵ�ʱ���ɹ���
	if(GetAction()==ACT_OPEN) return false;

	CServerRegion* pRegion = (CServerRegion*)GetFather();
	if(pRegion == NULL) return false;

	//�����ڷ�ս��ʱ�䲻�ܱ�����
	bool bResult = false;

	switch( pAttackShape -> GetType() )
		{
		case TYPE_PLAYER:
			{
			
			}
			break;


		}

	return bResult;
}

void CGate::OnDied()
{
	CBuild::OnDied();
}




