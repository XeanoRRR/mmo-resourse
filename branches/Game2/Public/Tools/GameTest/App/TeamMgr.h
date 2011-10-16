


#ifndef _TeamMgr_H_
#define _TeamMgr_H_
#pragma once

class CTeamMgr
{
public:
	CTeamMgr(void);
	~CTeamMgr(void);

	void Release();

	void DecodeFromDataBlock(DBReadSet& dbRead);

	// ��ú�������ļ״̬
	bool IsRecruitingTeam(){return m_bIsRecruitingTeam;}
	void SetRecruitingTeam(bool b){m_bIsRecruitingTeam=b;}

	// ��ú����ö����Ա������
	long GetTeammateAmount(){return m_lTeammateAmount;}
	void SetTeammateAmount(long l){m_lTeammateAmount=l;}

	// ��ú����ö��������
	const char * GetTeamname(){return m_strTeamname.c_str();}
	void SetTeamname(const char *strName){m_strTeamname=strName;}

	// ��ú����ö����Ƿ��������
	bool IsTeamHasPassword(){return m_bTeamHasPassword;}
	void SetTeamHasPassword(bool b){m_bTeamHasPassword=b;}
	
	//��ú����ö���ӳ�
	void SetIsTeamLeader(bool b)						{ m_bIsTeamLeader = b; }
	bool IsTeamLeader()									{ return m_bIsTeamLeader; }

	//��ú�����Ҫ��GUID
	void SetTeamRequesterGuid(const CGUID Rguid)		{RequesterGuid = Rguid;}
	CGUID GetTeamRequesterGuid()						{return RequesterGuid;}
 
private:
	bool	m_bIsRecruitingTeam;
	long	m_lTeammateAmount;
	string	m_strTeamname;
	bool	m_bTeamHasPassword;
	bool    m_bIsTeamLeader;
	CGUID   RequesterGuid;
};
#endif//_TeamMgr_H_