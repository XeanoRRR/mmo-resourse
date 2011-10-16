#pragma once
#include "info.h"

class CNPC :
	public CInfo
{
public:
	CNPC(void);
public:
	~CNPC(void);

public:
	// ��NPC�ļ� "*.npc"
	friend ifstream &operator>>(ifstream &rstream, CNPC &rNPC); 
	friend ofstream &operator<<(ofstream &rstream, const CNPC &rNPC); 

// ���ݶ�д
	const inline string& GetName()const						{return m_strName;}
	const inline unsigned GetPicID()const					{return m_uQuantity;}
	const inline string& GetScript()const					{return m_strScript;}

	inline void SetName(const string &strName)				{m_strName = strName;}
	inline void SetPicID(const unsigned uPicID)				{m_uPicID = uPicID;}
	inline void SetScript(const string &strScript)			{m_strScript = strScript;}

	void NPC2String(CString &str);

// NPC��Ϣ
	string		m_strName;			// NPC����
	unsigned	m_uPicID;			// ͼƬID
	string		m_strScript;		// �����ű�
};

ifstream &operator>>(ifstream &rstream, CNPC &rNPC); 
ofstream &operator<<(ofstream &rstream, const CNPC &rNPC); 
