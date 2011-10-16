


#ifndef _PlayerCollect_H_
#define _PlayerCollect_H_
#pragma once


class CPlayerCollect
{
public:
	CPlayerCollect(void);

	~CPlayerCollect(void);

	void Release();

	void DecodeFromDataBlock(DBReadSet& dbRead);


	DWORD   GetCollectSkillLevel()			  {return m_dwCollectLevel;}			//�ɼ��ȼ�
	void	SetCollectSkillLevel(DWORD dwLevel) {m_dwCollectLevel = dwLevel;}

	DWORD	GetCollectSkillExp()			{return m_dwCollectExp;}			//�ɼ�����
	void	SetCollectSkillExp(DWORD dwExp)	{m_dwCollectExp = dwExp;}

	DWORD	GetFactureSkillExp()			{return m_dwFactureExp;}			//��������
	void	SetFactureSkillExp(DWORD dwExp) {m_dwFactureExp = dwExp;}			

	DWORD	GetFactureSkillLevel()			 {return m_dwFactureLevel;}			//�����ȼ�
	void	SetFactureSkillLevel(DWORD dwLevel){m_dwFactureLevel = dwLevel;}


private:
	DWORD						m_dwCollectExp;					//�ɼ�����
	DWORD						m_dwCollectLevel;					//�ɼ��ȼ�
	DWORD						m_dwFactureExp;					//��������
	DWORD						m_dwFactureLevel;					//�����ȼ�
	DWORD						m_dwToolIndex;						//��ǰʹ�õĹ������ID
	vector<DWORD>				m_vecFactureEntrys;				//ѧ���������Ŀ�б�
};
#endif//_PlayerCollect_H_