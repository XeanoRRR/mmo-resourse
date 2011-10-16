#ifndef DB_PLAYER_H
#define DB_PLAYER_H
#include "BaseDBEntity.h"
#include "DBEntityDef.h"
#include "../public/DepotDef.h"
#include "../public/DBDefine.h"
#include "SpriteSystem.h"
#include "../public/PlayerSetupOnServer.h"
using namespace std;

class CDBMail;
class CDBLoginPlayer;
class CDBGoodsGroup;
class CDBLinkmanGroup;
class CDBMailGroup;

class CDBPlayer : public CBaseDBEntity
{
public:
	enum EQUIPMENT_COLUMN 
	{	
		EC_HEAD,				//	1��ͷ��
		EC_NECKLACE,			//	2������
		EC_WING,				//	3�����
		EC_BODY,				//	4������
		EC_BACK,				//	5������
		EC_GLOVE,			//6������	
		EC_BOOT,			//7��Ь��
		EC_HEADGEAR,			//	8��ͷ��
		EC_FROCK,			//	9������		
		EC_LRING,			//	10�����ָ
		EC_RRING,			//	11���ҽ�ָ		
		EC_MEDAL1,			//	12��ѫ��1
		EC_MEDAL2,			//	13	ѫ��2
		EC_MEDAL3,			//	14	ѫ��3
		EC_FAIRY,			//	15��С����
		EC_WEAPON,			//	16������
		EC_WEAPON2,			//	17����������
		EC_TOTAL,

		EC_PACK1=10001,
		EC_PACK2=10002,
		EC_PACK3=10003,
		EC_PACK4=10004,
		EC_PACK5=10005
	};

	CDBPlayer();
	CDBPlayer(const CGUID& guid);
	~CDBPlayer();

	// virtual interfaces
	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);

	void Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	void Save(long msgid, BYTE* msgBuf, long& msgBufPos);

	void SetAccount(const char* szCdkey, long size) { if(szCdkey) m_strCdkey = szCdkey; }
	const char* GetAccount(void) { return m_strCdkey.c_str(); }

	// ���ݸ��±�������װ��Ϣ
	void AddEntityDataToMsg(DB_OPERATION_TYPE opType, vector<BYTE>& pBA);
	// ȫ��������װ��Ϣ
	void AddAllEntityDataToMsg(DB_OPERATION_TYPE opType, vector<BYTE>& pBA);

	typedef map<CGUID, CDBGoods*>::iterator MapDBGoodsItr;

	// ���װ����Ʒmap
	//void ClearContainerMap(PLAYER_EXTEND_ID containerID);
	// ����ӱ�������
	//void ClearSubpackObjMap(long subPos);
	
	// ��ʼ����������
	void				InitProperty();

	virtual CDBPlayer& operator=(CDBPlayer& right);

	CDBGoodsGroup* GetGoodsGroupPtr(void);
	CDBLinkmanGroup* GetLinkmanGroupPtr(void);
	CDBMailGroup* GetMailGroupPtr(void);

	// CDBMail
	// ���һ��CDBMail��Mapĩβ
	void AddOneDBMail(CDBMail* mail);
	// ɾ��һ��CDBMail
	void DelOneDBMail(CDBMail* mail);
	// �������
	void ClearDBMailMap(void);


private:
	CDBGoodsGroup* p_GoodsGroup;
	CDBLinkmanGroup* p_LinkmanGroup;
	CDBMailGroup* p_MailGroup;

private:
	string			m_strCdkey;
};

#endif//DB_PLAYER_H
