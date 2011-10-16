/*
*	file:		GoodsExManage.h
*	Brief:		��Ʒ��չ���ܣ��ֽ�/�ϳɡ��������ܡ�����/���ᣩ������
*	detail:		����������ݲ��࣬��Ҫ�Ƕ�����ģ���ʹ�ý��еķ�װ��
*				���󰴹��ܷ�Ϊ���¼������֣�
*				����������Ա��������Ϣ��
*				������Ʒ�ֽ�
*				������Ʒ�ϳ�
*				������������
*	Authtor:	�ſ���
*	Datae:		2007-12-11
*/

#include "..\..\..\public\GoodsExSetup.h"

class CGoodsContainer;
class CPlayer;
class CGoods;


class GoodsExManage
{
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ������Ա�������ļ���GoodsExManage_base.cpp��
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	GoodsExManage(void);
	~GoodsExManage(void);

public:
	//!								�õ���̬ʵ��
	static		GoodsExManage&		GetInstance(void);

private:
	//!								��̬ʵ��
	static		GoodsExManage		c_GoodsExManage;


public:
	//!								ͨ����Ϣ��ȡ������Ϣ����Ϣ���ͣ�MSG_W2S_SERVER_SENDSETUP�����ݱ�ţ�SI_GOODSEX
	bool							InitSetup(BYTE *pData, long &lPos);

	//!
	GoodsExSetup&					GetGoodsExSetup(void){return m_GoodsExSetup;}

	//!	�����Ʒ��չ���ܻỰ
	struct tagGoodsExWndSession
	{
		ULONG	uWndType;	//! �Ự��������
		ULONG	uRadius;	//! �Ựʱ�������һ�뾶
		long	lPosX;		//! ���Χ����X����
		long	lPosY;		//! ���Χ����Y����
		CGUID	NpcGuid;	//! ��֮�Ի���NPC��guid
	};

	//��							�򿪿ͻ�����Ʒ��չ�������ڣ�ͬʱֻ�ܴ�һ�����ʹ��ڣ�
	bool							OpenGoodsExWindow(CPlayer *pPlayer, tagGoodsExWndSession& GoodsExWndSession);

	//!								�رտͻ�����Ʒ��չ�������ڣ������ػỰ��
	void							CloseGoodsExWindow(CPlayer *pPlayer);

	//��							��Ӧ�ͻ��˹ر���Ʒ��չ����
	void							OnClientCloseGoodsExWindow(CMessage *pMsg);

	//!								Ϊ��Ҽ������߽�����Ʒ
	BOOL							LockGoods(CPlayer *pPlayer, const CGUID& GoodsGuid, BOOL bLock);

private:
	//!								��֤��Ϣ�Ự����
	bool							TestSession(const CGUID& guid, eGoodsExWindowType eGoodsWinType);
	//!								����������ҵ�һ����Ʒ����������Ϊ��������5���ӱ�����
	CGoodsContainer*				GetGoodsContainer(CPlayer *pPlayer, long lContianerId);


private:
	
	//!										��Ҵ򿪵Ĵ���map<���ID��pair<�������ͣ��Ự����>>
	map<CGUID, tagGoodsExWndSession>		m_mapOpendGEWnd;

	

	
	//!-----------------------------------------------------------------------------------------
	//! ��Ʒ������������
	struct tagGoodsOperate
	{
		long lContianerPos;	//! Ŀ������ö��
		long lGoodsPos;		//! ����λ��
		long lOperateWay;	//!	����������CS2CContainerObjectMove::OPERATION_TYPE��
		long lOperateNum;	//! ��������
		CGoods *pGoods;
		long lReserved;		//! �����ֶΣ��ڲ�ʹ��
		tagGoodsOperate(void){ memset(this, 0, sizeof(tagGoodsOperate)); }
	};

	//!		���������ָ����λ���ҵ�һ����Ʒ��ֻ��ָ����������5���ӱ�����
	CGoods* FindGoodsFromPlayer(CPlayer *pPlayer, long lContianerID, long lPos);

	//!		����һ����Ʒ�Ƿ���ָ����λ����
	CGoods*	TestGoodsHere(CPlayer *pPlayer, const CGUID &GoodsGuid, long lContianerID, long lPos, BOOL bTestFrost = FALSE);

	//!		������Ʒ����ռ��Ƿ��㹻,����ʣ��ռ���������Ϊ������˵���ռ䲻��
	long	TestPutWay(CPlayer *pPlayer, vector<tagGoodsOperate> &vecDelGoods, vector<CGoods*> &vPutGoods); 

	//!				����������Ʒ
	bool	AddGoods(CPlayer *pPlayer, vector<CGoods*>& vAddGoods, vector<long>& vOptType, vector<long>& vOptNum);

	//!				ɾ�������Ʒ
	bool	DelGoods(CPlayer *pPlayer, vector<tagGoodsOperate> &vDelGoods, DWORD dwLogType, const CGUID &optGuid);
	//!-----------------------------------------------------------------------------------------


private:
	//!								���ù������
	GoodsExSetup					m_GoodsExSetup;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ��Ʒ�ֽ⣨�����ļ���GoodsExManage_Disassemble.cpp��
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//!				��Ӧ��Ʒ�ֽ�����
	void			OnGoodsDisassemble(CMessage *pMsg);

private:

	struct tagReValue
	{
		long lGoodsIndex;
		long lGoodsNum;
	};
	//!				�õ�һ����Ʒ�ķֽ�������ID�����������ز�Ʒ���������
	long			GetGoodsDisassembleResult(long lGoodsID,  tagReValue arrReValue[MAX_DISASSEMBLE_NUM]);

	





	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ��Ʒ�ϳɣ������ļ���GoodsExManage_Synthesize.cpp��
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//!				��Ӧ��Ʒ�ϳ�����
	void			OnGoodsSynthesize(CMessage *pMsg);


private:





	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ��������
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//!				����������Ʒ���õ����
	//!				��Ʒ�Ϸ��ԡ�ʹ�ú����Ʒɾ���������ⲿִ��
	bool			ReinforcePower(CGoods *pArm, CGoods *pGoods, CPlayer *pPlayer);

	//!				��ʼ��ҵ������ӳ�Ч����һ�����ܶ���ֻ�ܵ���һ�νӿڣ�Ч���ڷ���ֵ�У�
	bool			ArmReinforceBegin(CPlayer *pPlayer, CGoods *pArm, bool isMagic, bool isAttack, pair<float, float>&);

	//!				�ж��Ƿ񻹿��Լ�����
	long			CanExpendPower(CGoods *pArm, BOOL isMagic);

	//!				��������������õ�һ�����õļӳ�ֵ
	float			GetReinforceByEnum(long lAimLevel, long lPowerIntensity, long eValuePos);

	//!				�õ�һ�����ĵ�����������
	long			GetExpendPower(CGoods *pArm, BOOL isMagic);

private:
	//!
	GoodsExSetup::tagAmendCoefficient	m_Immobile;



	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! �󶨡�����
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:

	//! ��
	//!------------------------------------------------------------------

	//!				���������Ʒ�İ����ԣ����ֻ����eBT_Untie��eBT_Bind֮��ת��
	//!				�ؼ�����ֵ��
	//!					0���ɹ�
	//!					30������ֵ�ڵ�ǰֵ��ͬ
	long			SetGoodsBindProperty_Player(CGoods *pGoods, long lBindType, const CGUID& playerGuid);

	//!				Ϊ�ű��ṩ�İ󶨺���
	//��			����1����Ʒӵ����
	//!				����2��3����Ʒ���ڰ���λ�ã�ֻ������������5���ӱ�����
	//!				����4��0=��󶨡�1=�󶨡�����ֵ�Ƿ�
	//!				����ֵ��0=ʧ�ܣ�1=�ɹ���
	long			SetGoodsBindProperty_Script(CPlayer *pPlayer, long lContianerID, long lPos, long lBindType);

	//!				ϵͳ������Ʒ�İ�����
	long			SetGoodsBindProperty_System(CGoods *pGoods, long lBindType, const CGUID& playerGuid);

	//!				�����Ʒ�Ƿ��
	bool			IsBind(CGoods *pGoods);
	

	//! ��
	//!------------------------------------------------------------------
	//!				���������Ʒ�Ķ�������
	//!				�ؼ�����ֵ��
	//!					0���ɹ�
	//!					30������ֵ�ڵ�ǰֵ��ͬ
	//!					40���ⶳ�У�7��ʱ��δ��
	long			SetGoodsFrostProperty_Player(CGoods *pGoods, long lFrostType, const CGUID& playerGuid);

	//!				Ϊ�ű��ṩ�Ķ��ắ��
	//��			����1����Ʒӵ����
	//!				����2��3����Ʒ���ڰ���λ�ã�ֻ������������5���ӱ�����
	//!				����4��0=�ⶳ��1=���ᡣ����ֵ�Ƿ�
	//!				����ֵ��0=ʧ�ܣ�1=�ɹ���
	long			SetGoodsFrostProperty_Script(CPlayer *pPlayer, long lContianerID, long lPos, long lBindType);

	//!				�����Ʒ�Ƿ񶳽�
	//!				�����Ʒ�ⶳʱ���ѵ��������ⶳ���Ե�����
	bool			IsFrost(CGoods *pGoods);


	//! ��Ӧ��Ϣ
	//!------------------------------------------------------------------
	//!				��Ӧ��Ʒ��
	void			OnGoodsBind(CMessage *pMsg);
	//!				��Ӧ��Ʒ����
	void			OnGoodsFrost(CMessage *pMsg);

private:

	//!				��������Ϣ��Ӧ�ڴ�ִ�к���
	void			BindAndFrost(bool isBind, CMessage *pMsg);		


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ��Ʒ��������ȣ�
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//!				��Ӧװ������Ԥ��
	void			OnEquipmentUpgradeView(CMessage *pMsg);
	//!				��Ӧװ������
	void			OnEquipmentUpgrade(CMessage *pMsg);
	//!				����һ����ҵ�������Ʒ��Ч
	void			PlayUpgradeEffect(CPlayer *pPlayer, LONG lEffectID);


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ��Ʒħ��
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//!									��Ӧװ��ħ��
	void								OnEquipmentPlugEvil(CMessage *pMsg);
	//!									��Ӧ����ħ������
	void								OnEquipmentResetElementType(CMessage *pMsg);
	//!									��Ӧ����ħ������
	void								OnEquipmentResetXiangXing(CMessage *pMsg);


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! ��ʱ��Ʒ
	///////////////////////////////////////////////////////////////////////////////////////////////////
public:


};
 