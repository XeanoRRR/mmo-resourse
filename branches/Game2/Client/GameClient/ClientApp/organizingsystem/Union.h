/*Union.h
*���ܣ������߼����ܴ���
*ժҪ���� �л�ͬ�� ���� ����
*���ߣ�Joe
*�޸��ߣ�����
*�޸�����:2008-11-5
*/
#pragma once
#include "Faction.h"
class CFaction;
      
//**************************************************************************************
// �����߼���
//**************************************************************************************
class CUnion
{
public:
	// ������Ϣ�ṹ
	struct	SUnionInfo
	{
		CGUID		guidUnion;							// ���� GUID
		CGUID		guidMaster;							// ���� GUID
		CGUID		guidMasterFac;						// ���� GUID
		string		strUnionName;						// ���� ����
		string		strMasterName;						// ���� ����
		string		strPlacard;							// ���� ����
		string		strBeachhead;						// ���� �ݵ�

		long		lCountryID;							// ���� ID
		long		lLevel;								// ���� �ȼ�

		string		arrJobName[ MAXNUM_JobLvl ];		// �ٽ� ����
		long		arrJobPurview[ MAXNUM_JobLvl ];		// �ٽ� Ȩ��

		DWORD		dwIconCRC;							// ͼ��У����

		SUnionInfo()	{  }
	};

private:
	SUnionInfo								m_stUnionInfo;				// ���˻�����Ϣ
	std::list<CFaction*>					m_listFactions;				// �����Ա����

public:
	CUnion();
	virtual ~CUnion();
	CUnion( SUnionInfo* pstUnionInfo );

	// ���ݷ��ʺ���
	CGUID				GetID() const							{ return m_stUnionInfo.guidUnion;					}
	void				SetID( CGUID id )						{ m_stUnionInfo.guidUnion = id;						}

	LPCTSTR				GetName() const							{ return m_stUnionInfo.strUnionName.c_str();		}
	void				SetName( LPCTSTR szName )				{ m_stUnionInfo.strUnionName = szName;				}

	CGUID				GetMasterID() const						{ return m_stUnionInfo.guidMaster;					}
	void				SetMasterID( CGUID id )					{ m_stUnionInfo.guidMaster = id;					}

	CGUID				GetMasterFacID() const					{ return m_stUnionInfo.guidMasterFac;				}
	void				SetMasterFacID( CGUID id )				{ m_stUnionInfo.guidMasterFac = id;					}

	LPCTSTR				GetMasterName() const					{ return m_stUnionInfo.strMasterName.c_str();		}
	void				SetMasterName( LPCTSTR szName )			{ m_stUnionInfo.strMasterName = szName;				}

	void				SetPlacard( LPCTSTR szPlacard )			{ m_stUnionInfo.strPlacard = szPlacard;				}
	LPCTSTR				GetPlacard() const						{ return m_stUnionInfo.strPlacard.c_str();			}

	void				SetBeachhead( LPCTSTR szBeachhead )		{ m_stUnionInfo.strBeachhead = szBeachhead;			}
	LPCTSTR				GetBeachhead() const					{ return m_stUnionInfo.strBeachhead.c_str();		}

	long				GetCountryID() const					{ return m_stUnionInfo.lCountryID;					}
	void				SetCountryID( long id )					{ m_stUnionInfo.lCountryID = id;					}

	long				GetLevel() const						{ return m_stUnionInfo.lLevel;						}
	void				SetLevel( long lLevel )					{ m_stUnionInfo.lLevel = lLevel;					}


	SUnionInfo*			GetUnionInfo()							{ return &m_stUnionInfo;							}
	list<CFaction*>*	GetListOfFactions()						{ return &m_listFactions;							}
	long				GetNumOfFactions() const				{ return (long)m_listFactions.size();				}
	long				GetMaxNumOfFactions() const				{ return 10;				}

	long				GetNumOfMembers() const;
	long				GetNumOfOnlineMembers() const;

	// ��������
	bool				Release();
	bool				SetUnionInfo( SUnionInfo* pstUnionInfo );

	bool				AddFaction( CFaction* pFaction );
	bool				DelFaction( CGUID guid );
	bool				HasFaction( CGUID guid );
	CFaction*			GetFaction( CGUID guid );
	CFaction*			GetFaction( LPCTSTR szName );

	long				GetFacPurview( CGUID guid );
	bool				SetFacPurview( CGUID guid, long lPurview );

	void				SortFactions( CFaction::ENUM_SORT eSort );
};



//**************************************************************************************
// �����߼�������
//**************************************************************************************
class CUnionManager
{
	std::map<CGUID,CUnion*>			m_mapUnions;			// ���˱�

public:
	CUnionManager();
	virtual ~CUnionManager();

	void		Release();

	// ��⺯��
	bool		AmIChairman();
	bool		IsPlayerChairman( CPlayer* pPlayer );
	bool		IsPlayerChairman( CGUID guidUnion, CGUID guidPlayer );

	bool		AmIJoinedUnion();
	bool		AmIJoinedUnion( CGUID guidUnion );

	bool		IsPlayerJoinedUnion( CPlayer* pPlayer );
	bool		IsPlayerJoinedUnion( CPlayer* pPlayer, CGUID guidUnion );

	// ��������
	bool		AddUnion( CUnion::SUnionInfo* pstUnionInfo );
	bool		DelUnion( CGUID guid );
	bool		HasUnion( CGUID guid );
	CUnion*		GetUnion( CGUID guid );

	long		GetMyJob();
	bool		SetMyJob( long lJob );

	LPCTSTR		GetMyPlacard();
	bool		SetMyPlacard( LPCTSTR szPlacard );

	LPCTSTR		GetMyBeachhead();
	bool		SetMyBeachhead( LPCTSTR szBeachhead );

	CUnion*		GetMyUnion();
	CUnion*		GetFacUnion( CGUID guidFaction );
	bool		GetMyRights( eFactionPurviewIndex ePV );

	std::map<CGUID,CUnion*>*		GetListOfUnions()	{ return &m_mapUnions;	}

	// ����\���� ������Ϣ
	void		RecvScriptCreationMsg( CMessage* pMsg );
	void		RecvJoinMsg( CMessage* pMsg );
	void		RecvUnionInfoMsg( CMessage* pMsg );
	void		RecvUnionMemberMsg( CMessage* pMsg );

	void		SendCreationMsg( LPCTSTR szName );
	void		RecvCreationMsg( CMessage* pMsg );
	void		RecvCreationNotifyMsg( CMessage* pMsg );

	void		SendDisbandMsg();
	void		RecvDisbandMsg( CMessage* pMsg );

	void		SendQuitMsg();
	void		RecvQuitMsg( CMessage* pMsg );

	void		SendKickMsg( CGUID guidFaction );
	void		RecvKickMsg( CMessage* pMsg );

	void		SendSetChairmanMsg( CGUID guidChairman );
	void		RecvSetChairmanMsg( CMessage* pMsg );

	void		SendSetRightsMsg( CGUID guidFaction );
	void		RecvSetRightsMsg( CMessage* pMsg );
	void		RecvGetRightsMsg( CMessage* pMsg );

	void		SendInviteMsg( LPCTSTR szName );
	void		RecvInviteMsg( CMessage* pMsg );

	void		SendResponseMsg( CGUID guidFaction );
	void		RecvResponseMsg( CMessage* pMsg );
};