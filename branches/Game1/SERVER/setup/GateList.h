#pragma once
#include "..\..\PUBLIC\BaseDef.h"
#include "..\..\PUBLIC\statedef.h"
class CGateList
{
public:
	CGateList();
	~CGateList();
public:
	struct tagGate
	{

		char		cOrigName[50];				//ԭʼ��
		char		cName[50];					//����
		WORD		wLenth;				    	//����
		WORD		wWidth;						//���
		WORD		wLevel;						//�ȼ�
		DWORD		dwModelId;					//ģ��ID
		WORD		wMaxHp;						//���HP
		bool		bIsunbeatable;				//�Ƿ����޵�״̬
		BYTE		byBlockType;				//�赲����
		long		lDef;						//�������
		long		lMagicDef;					//ħ������
		WORD		wNameColor;					//������ɫ
		char		cHitSound[MAX_PATH];		//������Ч
		char		cDestorySound[MAX_PATH];	//�����Ч
		char		cOpenSound[MAX_PATH];		//������Ч
		char		cCloseSound[MAX_PATH];		//������Ч
	};
private:
	static map<string, tagGate>m_mapGateList;
public:
	static BOOL LoadGateList(const char* filename);
	static BOOL AddToByteArray(vector<BYTE>* pByteArray);
	static BOOL DecordFromByteArray(BYTE* pByte, long& pos);
	static tagGate*	GetPropertyByOrigName(const char* strName);
};