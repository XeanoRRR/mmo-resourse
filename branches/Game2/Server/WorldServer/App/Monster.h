#pragma once
#include "MoveShape.h"
#include "../../../Public/Common/MonsterDef.h"

class CMonster :
	public CMoveShape
{
public:
	CMonster(void);
	virtual ~CMonster(void);

public:

	// ����
	/* del by chenaly..........07.4.6.......
	struct stProperty
	{
		string			strOrginName;		//	ԭʼ����
		DWORD			dwHP;				//	��ǰ����ֵ

		WORD			wSign;				//  ���ڵ�ͼ��Ψһ��ʶ
		WORD			wLeaderSign;		//  �쵼��Ψһ��ʶ
		long			lLeaderType;		//	�쵼��type
		long			lLeaderID;			//	�쵼��id
		WORD			wLeaderDistance;	//  ���쵼�߱��ֵ�������
		long			lLiveTime;			//	����ʱ�䣨-1����һֱ���棬����ʱ����λ���룩
		bool			bDiedRemove;		//	������ɾ����������
	};
	//*///end del ....................................

private:
	//chaneged by chenaly......07.4.6........
	//stProperty		m_Property;	       // ����			
	tagMPro  m_Property ;       // ����
	tagMFPro m_FightProperty ;   // ս������

	//end change..................................

public:
	//changed by chenaly............07.4.6...........
	//const CMonsterList::tagMonster*		GetBaseProperty()	{return CMonsterList::GetPropertyByOrginName(m_Property.strOrginName.c_str());}
	//stProperty*	GetProperty()	{return &m_Property;}

	//virtual BYTE GetFigure()	{return GetBaseProperty()->dwFigure;}	// ����
	//end change......................................
	
};
