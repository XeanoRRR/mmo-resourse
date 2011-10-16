/*CFaction.h
*���ܣ�������߼����ܴ���
*ժҪ���� ���� ���� ����
*���ߣ�Joe
*�޸��ߣ�����
*�޸�����:2008-11-5
*/

#pragma once

//**************************************************************************************
// ������Ա�� �߼�������
//**************************************************************************************
#include "../../../../Public/Common/OrganizingDef.h"

class CPlayer;
class CMessage;
class CFacMember
{
public:
	// ����ö��
	enum	ENUM_SORT
	{
		SORT_BY_NORMAL,				// ͨ�� �������
		SORT_BY_NAME,				// ���� �������
		SORT_BY_JOB,				// �ٽ� �������
		SORT_BY_TITLE,				// �ƺ� �������
		SORT_BY_LEVEL,				// �ȼ� �������
		SORT_BY_STATE,				// ״̬ �������
		SORT_BY_REGION,				// ���� �������
		SORT_BY_ONLINE,				// ���� �������
		SORT_BY_OCCUPATION,			// ְҵ �������
		SORT_BY_CONTRIBUTE,			// ���� �������
		SORT_BY_SELECTION,			// ѡ�� �������

		MAXNUM_SORT
	};

	// ��Ա��Ϣ�ṹ
	struct	SMemberInfo
	{
		CGUID		guidFac;		// ����GUID
		CGUID		guidMember;		// ��ԱGUID
		string		strName;		// ��Ա����
		string		strTitle;		// ��Ա�ƺ�
		long		lLevel;			// ��Ա�ȼ�
		long		lJob;			// ��Ա�ٽ�
		long		lOccupation;	// ��Աְҵ
		long		lContribute;	// ��Ա����
		long		lAchievement;	// ��Ա��ѫ
		long		lWarFund;		// ����΢��
		long		eState;			// ��ǰ״̬
        long        LastOnlineTime; // ���һ������ʱ��
		bool		bOnline;		// �Ƿ�����
		string		strRegion;		// ���ڳ���
		bool		bSelected;		// �Ƿ�ѡ��
		bool		bWar;			// �Ƿ��ս

		SMemberInfo()	
        { 
            //ZeroMemory(this, sizeof(SMemberInfo)); 
        }
	};

private:
	SMemberInfo			m_stMemberInfo;		// ��Ա��Ϣ

public:
	CFacMember(void);
	~CFacMember(void);
	CFacMember( SMemberInfo* pstMemberInfo );

	// ���ݷ��ʺ���
	void			SetMemberInfo( SMemberInfo* pstMemberInfo )		{ m_stMemberInfo = *pstMemberInfo;				}
	SMemberInfo*	GetMemberInfo()									{ return &m_stMemberInfo;						}
	LPCTSTR			GetOccuText() const;

	void			SetFacGUID( CGUID guidFac )						{ m_stMemberInfo.guidFac = guidFac;				}
	CGUID			GetFacGUID() const								{ return m_stMemberInfo.guidFac;				}

	void			SetGUID( CGUID guid )							{ m_stMemberInfo.guidMember = guid;				}
	CGUID			GetGUID() const									{ return m_stMemberInfo.guidMember;				}

	void			SetName( LPCTSTR szName )						{ m_stMemberInfo.strName = szName;				}
	LPCTSTR			GetName() const									{ return m_stMemberInfo.strName.c_str();		}

	void			SetTitle( LPCTSTR szTitle )						{ m_stMemberInfo.strTitle = szTitle;			}
	LPCTSTR			GetTitle() const								{ return m_stMemberInfo.strTitle.c_str();		}

	void			SetLevel( long lLevel )							{ m_stMemberInfo.lLevel = lLevel;				}
	long			GetLevel() const								{ return m_stMemberInfo.lLevel;					}

	void			SetJob( long lJob )								{ m_stMemberInfo.lJob = lJob;					}
	long			GetJob() const									{ return m_stMemberInfo.lJob;					}

	void			SetOccupation( long lOccupation )				{ m_stMemberInfo.lOccupation = lOccupation;		}
	long			GetOccupation() const							{ return m_stMemberInfo.lOccupation;			}

	void			SetContribute( long lContribute )				{ m_stMemberInfo.lContribute = lContribute;		}
	long			GetContribute() const							{ return m_stMemberInfo.lContribute;			}

	void			SetAchievement( long lAchievement )				{ m_stMemberInfo.lAchievement = lAchievement;	}
	long			GetAchievement() const							{ return m_stMemberInfo.lAchievement;			}

	void			SetWarFund( long lWarFund )						{ m_stMemberInfo.lWarFund = lWarFund;			}
	long			GetWarFund() const								{ return m_stMemberInfo.lWarFund;				}

	void			SetState( long eState )							{ m_stMemberInfo.eState = eState;				}
	long			GetState() const								{ return m_stMemberInfo.eState;					}

    void            SetLastTime(long eLastTime)                     { m_stMemberInfo.LastOnlineTime = eLastTime;    }
    long            GetLastTime()const                              { return m_stMemberInfo.LastOnlineTime;         }

	void			SetOnline( bool bOnline )						{ m_stMemberInfo.bOnline = bOnline;				}
	bool			GetOnline() const								{ return m_stMemberInfo.bOnline;				}

	void			SetRegion( LPCTSTR szRegion )					{ m_stMemberInfo.strRegion = szRegion;			}
	LPCTSTR			GetRegion() const								{ return m_stMemberInfo.strRegion.c_str();		}

	void			SetSelected( bool bSelected )					{ m_stMemberInfo.bSelected = bSelected;			}
	bool			GetSelected() const								{ return m_stMemberInfo.bSelected;				}

	void			SetWar( bool bWar )								{ m_stMemberInfo.bWar = bWar;					}
	bool			GetWar() const									{ return m_stMemberInfo.bWar;					}

	// ����ȽϺ���
	static int		CompareForceRule( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );

	static bool		CompareNormal( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
	static bool		CompareByName( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
	static bool		CompareByJob( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
	static bool		CompareByTitle( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
	static bool		CompareByLevel( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
	static bool		CompareByState( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
	static bool		CompareByRegion( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
	static bool		CompareByOnline( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
	static bool		CompareByOccupation( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
	static bool		CompareByContribute( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
	static bool		CompareBySelection( const CFacMember* pFacMember1, const CFacMember* pFacMember2 );
};




//**************************************************************************************
// ����� �߼�������
//**************************************************************************************

class CFaction
{
public:
	// ����ö��
	enum	ENUM_SORT
	{
		SORT_BY_FACIOTN,			// ���� �������
		SORT_BY_CHAIRMAN,			// �峤 �������
		SORT_BY_ONLINE,				// ���� �������
		SORT_BY_LEVEL,				// �ȼ� �������

		MAXNUM_SORT
	};

	// ���幦��ö��
	enum	ENUM_FUN
	{
		FUN_FACTION,									// ����
		FUN_WELFARE,									// ����
		FUN_STORAGE,									// רҵ
		FUN_SMELT,										// ��ҵ
		FUN_FORGING,									// ս��
		FUN_SEWING,										// �Թ�

		MAXNUM_FUN
	};

	// ������Ϣ�ṹ
	struct	SFacInfo
	{
		CGUID		guidFac;							// ���� GUID
		CGUID		guidUnion;							// ���� GUID
		CGUID		guidMaster;							// �᳤ GUID
		string		strFacName;							// ���� ����
		string		strMasterName;						// �᳤ ����
		string		strPlacard;							// ���� ����
		string		strMemo;							// ���� ����
		long		idCountry;							// ���� ����
		long		resFac;								// ���� ��Դ
		long		expFac;								// ���� ����
		bool		bWar;								// �Ƿ� ս��
		bool		bTrade;								// �Ƿ� ó��
		long		lUnionJob;							// ���� �ٽ�
		bool		bRecruiting;						// �Ƿ� ��ļ
		bool		bOnline;							// �峤 ����
		long		arrLevel[ MAXNUM_FUN ];				// ���� �ȼ�
		string		arrJobName[ MAXNUM_JobLvl ];		// �ٽ� ����
		long		arrJobPurview[ MAXNUM_JobLvl ];		// �ٽ� Ȩ��
		long		lWarFund;							// ���� ΢��
		long		lAchievement;						// ������ѫֵ

		long		numMembers;							// ��Ա ����
		long		numOnlineMembers;					// ���� ����

		time_t		tMasterLastOnlineTime;				// �峤�������ʱ��
		time_t		tLessMemNumTime;					// ��Ա����ʱ��

		DWORD		dwIconCRC;							// ͼ��У����

		SFacInfo()	{ ZeroMemory(this, sizeof(SFacInfo)); }
	};

private:
    std::list<CFacMember*>		m_listApply;			// ������Ϣ��
	std::list<CFacMember*>		m_listMembers;			// ��Ա��Ϣ��
	std::vector<tagEnemy>		m_vecWarInfo[ eMaxWar ];// ս����Ϣ���� 
    SFacInfo					m_stFacInfo;			// ������Ϣ

public:
	CFaction(void);
	~CFaction(void);
	CFaction( SFacInfo* pstFacInfo);
    CFaction( const CFaction& fac);

	void	ClearMembers();
	void	ClearApply();

	// ���ݷ��ʺ���
	std::list<CFacMember*> *	GetListOfMembers()						{ return &m_listMembers;							}
	std::list<CFacMember*> *	GetListOfApply()						{ return &m_listApply;								}

	void				SetFacInfo( SFacInfo* pstFacInfo )				{ m_stFacInfo = *pstFacInfo;						}
	SFacInfo*			GetFacInfo()									{ return &m_stFacInfo;								}

	void				SetGUID( CGUID guidFac )						{ m_stFacInfo.guidFac = guidFac;					}
	CGUID				GetGUID() const									{ return m_stFacInfo.guidFac;						}

	void				SetUnionGUID( CGUID guidUnion )					{ m_stFacInfo.guidUnion = guidUnion;				}
	CGUID				GetUnionGUID() const							{ return m_stFacInfo.guidUnion;						}

	void				SetMasterGUID( CGUID guidMaster )				{ m_stFacInfo.guidMaster = guidMaster;				}
	CGUID				GetMasterGUID() const							{ return m_stFacInfo.guidMaster;					}

	void				SetName( LPCTSTR szFacName )					{ m_stFacInfo.strFacName = szFacName;				}
	LPCTSTR				GetName() const									{ return m_stFacInfo.strFacName.c_str();			}

	void				SetMasterName( LPCTSTR szMasterName )			{ m_stFacInfo.strMasterName = szMasterName;			}
	LPCTSTR				GetMasterName() const							{ return m_stFacInfo.strMasterName.c_str();			}

	void				SetPlacard( LPCTSTR szPlacard )					{ m_stFacInfo.strPlacard = szPlacard;				}
	LPCTSTR				GetPlacard() const								{ return m_stFacInfo.strPlacard.c_str();			}

	void				SetMemo( LPCTSTR szMemo )						{ m_stFacInfo.strMemo = szMemo;						}
	LPCTSTR				GetMemo() const									{ return m_stFacInfo.strMemo.c_str();				}

	void				SetRes( long res )								{ m_stFacInfo.resFac = res;							}
	long				GetRes() const									{ return m_stFacInfo.resFac;						}

	void				SetExp( long exp )								{ m_stFacInfo.expFac = exp;							}
	long				GetExp() const									{ return m_stFacInfo.expFac;						}

	void				SetLevel( long level )							{ m_stFacInfo.arrLevel[ FUN_FACTION ] = level;		}
	long				GetLevel() const								{ return m_stFacInfo.arrLevel[ FUN_FACTION ];		}

	void				SetCountry( long idCountry )					{ m_stFacInfo.idCountry = idCountry;				}
	long				GetCountry() const								{ return m_stFacInfo.idCountry;						}

	void				SetRecruiting( bool bRecruiting )				{ m_stFacInfo.bRecruiting = bRecruiting;			}
	bool				GetRecruiting() const							{ return m_stFacInfo.bRecruiting;					}

	void				SetWar( bool bWar )								{ m_stFacInfo.bWar = bWar;							}
	bool				GetWar() const									{ return m_stFacInfo.bWar;							}

	void				SetTrade( bool bTrade )							{ m_stFacInfo.bTrade = bTrade;						}
	bool				GetTrade() const								{ return m_stFacInfo.bTrade;						}

	void				SetIconCRC( DWORD dwIconCRC )					{ m_stFacInfo.dwIconCRC = dwIconCRC;				}
	DWORD				GetIconCRC() const								{ return m_stFacInfo.dwIconCRC;						}

	void				SetUnionJob( long lUnionJob )					{ m_stFacInfo.lUnionJob = lUnionJob;				}
	long				GetUnionJob() const								{ return m_stFacInfo.lUnionJob;						}

	void				SetWarFund( long lWarFund )						{ m_stFacInfo.lWarFund = lWarFund;					}
	long				GetWarFund() const								{ return m_stFacInfo.lWarFund;						}

	void				SetAchievement( long lAchievement )				{ m_stFacInfo.lAchievement = lAchievement;			}
	long				GetAchievement() const							{ return m_stFacInfo.lAchievement;					}

	void				SetNumOfMembers( long numMembers )				{ m_stFacInfo.numMembers = numMembers;				}
	void				SetNumOfOnlineMembers( long numOnlineMembers )	{ m_stFacInfo.numOnlineMembers = numOnlineMembers;	}

	long				GetNumOfMembers() const;
	long				GetNumOfOnlineMembers() const;

	void				SetLevel( ENUM_FUN eFun, long level );
	long				GetLevel( ENUM_FUN eFun ) const;

	void				SetOnline( bool bOnline );
	bool				GetOnline() const;

	void				SetJobName( long lJobLvl, LPCTSTR szJobName );
	LPCTSTR				GetJobName( long lJobLvl ) const;

	void				SetJobPurview( long lJobLvl, long lJobPurview );
	bool				GetJobPurview( long lJobLvl, eFactionPurviewIndex ePV ) const;

	void				SetEnemy( eBattleType eBT, tagEnemy* pEnemy );
	vector<tagEnemy>*	GetEnemy( eBattleType eBT );

	// ��������
	long				GetNumOfApply()									{ return (long)m_listApply.size();					}
	long				GetMaxNumOfMembers();
	long				GetRequireNumOfMembers();

	bool				AddMember( CFacMember::SMemberInfo* pstMemberInfo );
	bool				DelMember( CGUID guid );
	bool				HasMember( CGUID guid ) const;
	CFacMember*			GetMember( CGUID guid ) const;
	CFacMember*			GetMember( LPCTSTR szName ) const;

	bool				AddApply( CFacMember::SMemberInfo* pstMemberInfo );
	bool				DelApply( CGUID guid );
	bool				HasApply( CGUID guid ) const;
	CFacMember*			GetApply( CGUID guid ) const;
	CFacMember*			GetApply( LPCTSTR szName ) const;

	void				SortMembers( CFacMember::ENUM_SORT eSort );
	void				SortApply( CFacMember::ENUM_SORT eSort );

	// ����ȽϺ���
	static int		CompareForceRule( const CFaction* pFaction1, const CFaction* pFaction2 );

	static bool		CompareByFaction( const CFaction* pFaction1, const CFaction* pFaction2 );
	static bool		CompareByChairman( const CFaction* pFaction1, const CFaction* pFaction2 );
	static bool		CompareByOnline( const CFaction* pFaction1, const CFaction* pFaction2 );
	static bool		CompareByLevel( const CFaction* pFaction1, const CFaction* pFaction2 );
};

//**************************************************************************************
// �����߼�������
//**************************************************************************************
class CFactionManager
{
	std::list<CFaction*>			    m_listFactions;				// �����
	std::list<tagBaseInfo>				m_listBeachheads;			// �ݵ��б�
    std::list<tagApplyInfoList>         m_listApplyInfo;            // �����б�

public:
	CFactionManager();
	virtual ~CFactionManager();

	void	ClearFactions();

	// ���ݷ��ʺ���
	long			GetFacLvlForPV( eFactionPurviewIndex ePVIndex );
	long			GetMaxNumByFacLv( long lFacLvl );
	long			GetRequireNumByFacLv( long lFacLvl );
	long			GetNumOfMyBeachheads();

	CFaction*		GetMyFaction();
	bool			GetMyRights( eFactionPurviewIndex ePV );

	CFacMember*		GetMyConfrere( CGUID guid );
	CFacMember*		GetMyConfrere( LPCTSTR szName );
	bool			AddMyConfrere( CFacMember::SMemberInfo* pstMemberInfo );

	long			GetMyJob();
	bool			SetMyJob( long lJob );

	LPCTSTR			GetMyPlacard();
	bool			SetMyPlacard( LPCTSTR szPlacard );

	LPCTSTR			GetMyMemo();
	bool			SetMyMemo( LPCTSTR szMemo );

	string			GetFacIconPath( CGUID guidFac );
	DWORD			GetFacIconCRC( CGUID guidFac );
	bool			CheckFacIconCRC( CGUID guidFac, DWORD dwNewCRC );

	list<CFaction*>*		GetListOfFactions()		{ return &m_listFactions;	}
	list<tagBaseInfo>*		GetListOfBeachheads()	{ return &m_listBeachheads;	}
    list<tagApplyInfoList>*	GetListOfApplyInfo()	{ return &m_listApplyInfo;	}

	// ��⺯��
	bool			AmIChairman();
	bool			IsPlayerChairman( CPlayer* pPlayer );

	bool			AmIJoinedFaction();
	bool			AmIJoinedFaction( CGUID guidFaction );

	bool			IsPlayerJoinedFaction( CPlayer* pPlayer );
	bool			IsPlayerJoinedFaction( CPlayer* pPlayer, CGUID guidFaction );

	bool			CanUpgradeMyFun( CFaction::ENUM_FUN eFun );

	long			GetFunMaxLevel( CFaction::ENUM_FUN eFun );

	bool			HasRes_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	bool			HasExp_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	bool			HasGold_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	bool			HasProp_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	bool			HasFacLvl_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	bool			HasMasterLvl_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	bool			HasBeachhead_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );

	long			NeedRes_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	long			NeedExp_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	long			NeedGold_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	long			NeedProp_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	long			NeedFacLvl_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	long			NeedMasterLvl_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );
	long			NeedBeachhead_ForUpgradeMyFun( CFaction::ENUM_FUN eFun );

	// ��������
    bool			AddFaction( CFaction::SFacInfo* pstFacInfo );
	bool			DelFaction( CGUID guid );
	bool			HasFaction( CGUID guid );
	CFaction*		GetFaction( CGUID guid );

	// ����\���� ������Ϣ
	void			SendCreateMsg( LPCTSTR strFactionName ,LPCTSTR Placard);
	void			RecvCreateNotifyMsg( CMessage* pMsg );

	void			RecvUpdateFacIconNotifyMsg( CMessage* pMsg );
	void			RecvUpdateAllFacIconMsg( CMessage* pMsg );

	void			RecvInitMsg( CMessage* pMsg );
	void			RecvFacPublicDataMsg( CMessage* pMsg );
	void			RecvBaseInfoMsg( CMessage* pMsg );
	void			RecvPlacardInfoMsg( CMessage* pMsg );
	void			RecvMemoInfoMsg( CMessage* pMsg );
	void			RecvWarInfoMsg( CMessage* pMsg );
	void			RecvMemberAchievementMsg( CMessage* pMsg );
	void			RecvMemberWarFundMsg( CMessage* pMsg );

	void			RecvMemberInfoMsg( CMessage* pMsg );
	void			RecvRequisitionInfoMsg( CMessage* pMsg );
	void			RecvBeachheadInfoMsg( CMessage* pMsg );

	void			SendQuitMsg();
	void			SendDisbandMsg();
	void			SendKickMsg( CGUID guidMember );

	void			SendBaseInfoRequestMsg();
	void			SendUpgradeMsg( CFaction::ENUM_FUN eFun );

	void			SendSetTitleMsg( CGUID guidMember, LPCTSTR szTitle );
	void			SendSetChairmanMsg( CGUID guidMember );
	void			SendSetPlacardMsg( LPCTSTR szPlacard );
	void			SendSetMemoMsg( LPCTSTR szMemo );
	void			SendSetJobNameMsg( long idJob, LPCTSTR szJobName );
	void			SendSetJobPurviewMsg( long idJob, long lPurview );
	void			SendSetMemberJobMsg( CGUID guidMember, long idJob );
	void			SendCheckRequisitionMsg( long bAgree, CGUID guid );
	void			SendCheckRequisitionMsg( long bAgree );

	void			SendInviteRequestMsg( LPCTSTR szInvitePlayerName );
	void			SendInviteAnswerMsg( CGUID guidInviter, long bAgree );

	void			SendMemberWarMsg( CGUID guidMember, BYTE byWar );
	void			RecvMemberWarMsg( CMessage* pMsg );

	void			SendUploadFacIconMsg( LPCTSTR strFacIconFileName );
	void			SendUpdateFacIconMsg( CGUID guidFac );
	void			SendUpdateAllFacIconMsg();

    /// 
    void            SendFactionList();
    void            RecvFactionList(CMessage* pMsg);

	void			HandleErrorMsg( CMessage* pMsg );
};