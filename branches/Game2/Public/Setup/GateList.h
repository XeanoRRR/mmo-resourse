//======================================================================================
/**
*	@file		GateList.h
*	@data		
*	@author		
*/
//======================================================================================

#pragma once

/**
*	@class GateList
*
*	@brief Gate������
*/

class GateList
{
public:
	/// ����
	GateList();

	/// ����
	~GateList();

public:
	/// Gate�ṹ
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
		uchar		byBlockType;				//�赲����
		WORD		wDef;						//�������
		WORD		wMagicDef;					//ħ������
		WORD		wNameColor;					//������ɫ
		char		cHitSound[MAX_PATH];		//������Ч
		char		cDestorySound[MAX_PATH];	//�����Ч
		char		cOpenSound[MAX_PATH];		//������Ч
		char		cCloseSound[MAX_PATH];		//������Ч
	};

private:
	/// Gate�б�
	static map<string, tagGate>m_mapGateList;

public:
	/// ���ļ�����Gate�б�
	static bool LoadGateList(const char* filename);

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);

	/// ����ԭʼ��ȡ��Gate����
	static tagGate*	GetPropertyByOrigName(const char* strName);
};