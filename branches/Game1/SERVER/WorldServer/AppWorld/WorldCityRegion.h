///////////////////////////////////////////////////////////////
//worldregion.h///////////////////////////////////////////////
//����ս��ͼ�࣬���𹥳�ս�Ĵ���
//Author��Joe
//Create Date:2004.7.19
#pragma once
#include "worldwarregion.h"

//��ǰ���е�״̬

class CWorldCityRegion :
	public CWorldWarRegion
{
public:
	CWorldCityRegion(void);
	~CWorldCityRegion(void);

	//Build
	struct tagBuild
	{
		long lID;				// ���ŵı��
		long lPicID;			// ͼƬID
		long lDir;				// ����
		long lAction;			// ��ʼAction
		long lMaxHP;			// �����������
		long lDef;				// ������
		long lElementResistant;	//Ԫ�ؿ���
		long lTitleX;			// ����
		long lTitleY;
		long lWidthInc;			// ����
		long lHeightInc;
		
		string strName;			// ����
		string strScript;		// ����ִ�нű�
	};


private:
	list<tagBuild>		m_CityGates;				//����
	tagRegionSetup		m_DefenceSideRS;			//���ط���������
//////////////////////////////////////////////////////////////////////////
//	�ӿ�
//////////////////////////////////////////////////////////////////////////
public:
	BOOL Load();	// ��ȡ
	BOOL Save();	// ��ȡ

	BOOL LoadCitySetup();//��ȡ��������
	virtual void SetEnterPosXY(CPlayer* pPlayer);

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
};
