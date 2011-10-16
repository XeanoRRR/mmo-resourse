////////////////////////////////////////////////////////////////////////////
//build.h///////////////////////////////////////////////////////////////////
//�����࣬�������������ͷ�������AI
//Author:Joe
//Create Time:2004.721
////////////////////////////////////////////////////////////////////////////
#pragma once
#include "MoveShape.h"

class CBuild :
	public CMoveShape
{
public:
	CBuild(void);
	~CBuild(void);

	// ��ʼ����������ֵ��Ӱ��
	void InitNameValueMap();
public:
	// ����
	struct tagProperty
	{
		//long			lMaxHP;				//  �������ֵ 
		//long			lHP;				//	��ǰ����ֵ
		//long			lDef;				//  ������
		//long			lElementResistant;	//	Ԫ�ؿ���
		//long			lWidthInc;			//  ����
		//long			lHeightInc;			 
		long			lIndex;				//���

		long			lMaxHP;				//  �������ֵ 
		long			lHP;				//	��ǰ����ֵ
		long			lDef;				//  ������
		long			lMagicDef;			//  ħ��������

		long			lNameColor;			//������ɫ
		long			lWidthInc;			//����
		long			lHeightInc;			//���
		WORD		    wLevel;				//�ȼ�
	//string			strHitSound;		//������Ч
	//string			strDestorySound;	//�����Ч
	};

private:
	tagProperty			m_Property;			// ����
	long                m_Switch;           // �޵п���

	string			strHitSound;		//������Ч
	string			strDestorySound;	//�����Ч
	string          strOpenSound;       //������Ч
	string          strCloseSound;      //������Ч

public:

	virtual void SetAction(WORD l);

	virtual	bool Display(void);
	virtual void AI(void);
	long AI_Stand();			// վAI
	long AI_BeAttack();			// ������AI
	long AI_Died();				// ����AI
	long AI_Open();				// ����AI

	virtual void OnActionChange();

	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);

	virtual DWORD	GetHp()							{return m_Property.lHP;		}
	virtual VOID	SetHp(DWORD l)					{m_Property.lHP=(long)l;	}
	virtual VOID    SetMaxHp(DWORD l)               {m_Property.lMaxHP=(long)l; }
	virtual DWORD	GetMaxHp()						{return m_Property.lMaxHP;	}
	virtual DWORD	GetLevel()						{return m_Property.wLevel;	} 
	VOID  SetSwitch(bool l)                        { m_bUnAttack=(long)l;   }
	bool GetSwitch()                               {return m_bUnAttack;}
	long  GetNameColor()                            {return m_Property.lNameColor;}
	virtual	DWORD	SetValue(const char* strName, DWORD dwValue);
	void  UpdateProperty(CMessage* pMsg);
	void  DecordChangeProperty(BYTE* pByte, long& pos);
	void  DecodeChangePropertyFromDataBlock(DBReadSet& setReadDB);

	virtual BYTE GetFigure(eDIR dir=DIR_UP);										// ���ݷ���õ�����
};
