

#pragma once

const LONG STR_INFO_SIZE = 256;
const LONG STR_TXT_SIZE = 1024 * 15;

class GoodsAllPurposeOpt
{
public:
	GoodsAllPurposeOpt(void);
	~GoodsAllPurposeOpt(void);

public:
	//! ��������ֵ
	enum eOptResult
	{
		//! ʧ��
		eOR_Failed,
		//! �ɹ�
		eOR_Succeed,
		//! �Ƿ�����Ҫ�������
		eOR_Lawless,
	};

	//!					��ʼ��Ʒ����
	BOOL				BeginOpt(
							const CGUID	&OwnerGuid,			//! �������GUID
							DWORD		dwTitleID,			//! ����ID
							DWORD		dwButtonID,			//! ��ťID
							LPCSTR		pText,				//! ����˵������
							LPCSTR		pOnPushScript,		//! ��Ӧ��Ʒ����Ľű���
							LPCSTR		pOnFinishScript,	//! ��Ӧ��ť���µĽű���
							LONG		lRadius,			//! �Ựʱ����Ļ�뾶
							LONG		lPosX,				//! ���Χ����X����
							LONG		lPosY,				//! ���Χ����Y����
							const CGUID	&RegionGuid,		//! ���ڵ�ͼGUID
							DWORD		dwMaxPushNum		//! ��������Ʒ��������������MAX_ALLPURPOSE_CONTAINER_SIZE��
							);

	//!					��Ӧ��Ʒ����
	eOptResult			OnGoodsPut(const CGUID	&GoodsGuid, DWORD dwIdx);

	//!					��Ӧ��Ʒ�Ƴ�
	eOptResult			OnGoodsPop(DWORD dwIdx);

	//!					��Ӧ��ʼ��Ʒ����
	eOptResult			FinishOpt(void);

public:
	//!					�õ������е���ƷGUID
	const	CGUID&		GetOptGoodsGuid(DWORD dwIdx){if(m_dwMaxPushNum > dwIdx)return m_arrGoodsGuid[dwIdx];return NULL_GUID;}
	//!					ˢ��������ʾ����
	eOptResult			SetTxt(LPCSTR pTxt);
	//!					ˢ��������ʾ��Ǯ��
	eOptResult			SetGold(DWORD dwGoldNum);
	//!
	void				Release(BOOL bSendCloseMsg = TRUE);

private:
	//!					�ж���Ҳ����ĺϷ���
	BOOL				CanOpt(CPlayer *pPlayer);

private:
	//!					�������GUID
	CGUID				m_OwnerGuid;
	//!					����ID
	DWORD				m_dwTitleID;
	//!					��ťID
	DWORD				m_dwButtonID;
	//!					����˵������
	CHAR				m_szText[STR_TXT_SIZE];
	//!					��Ӧ��Ʒ����Ľű���
	CHAR				m_szOnPushScript[STR_INFO_SIZE];
	//!					��Ӧ��ť���µĽű���
	CHAR				m_szOnFinishScript[STR_INFO_SIZE];
	//!					�Ựʱ����Ļ�뾶
	LONG				m_lRadius;
	//!					���Χ����X����
	LONG				m_lPosX;
	//!					���Χ����Y����
	LONG				m_lPosY;
	//!					�Ի����ڵ�ͼGUID
	CGUID				m_RegionGuid;

	//!					������Ʒ��GUID
	CGUID				m_arrGoodsGuid[MAX_ALLPURPOSE_CONTAINER_SIZE];
	//!					��������Ʒ������
	DWORD				m_dwMaxPushNum;
};