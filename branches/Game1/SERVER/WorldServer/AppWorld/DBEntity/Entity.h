//Entity.h/////////////////////////////////////////////////////////////////////
//������:�ö���ֻ�����Լ�����������,�ڽṹ���г�ΪҶ�ӽڵ�
//Author:������
//Create Time:2008.11.03
#ifndef ENTITY_H
#define ENTITY_H
#include <list>
#include <vector>
#include <string>
#include <set>
#include "../public/msgtype.h"
#include "../public/GUID.h"
#include "../public/entityproperty.h"
#include "../public/dbdefine.h"
#include "BaseEntity.h"

// ��Ϸ������࣬���ж������Ϸ��ص����ɸ�������
class CEntity : public CBaseEntity
{
public:
	CEntity(const std::string& flag, const CGUID& guid);
	virtual ~CEntity();

	// virtual interfaces
	virtual void ProcessMsg(bool isLoadOperFlag, const CGUID& SessionID, long retFlag, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	virtual void GetFromByteArray(bool isLoadOperFlag, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);

	void AddToByteArray(std::vector<BYTE>& pBA);

	CEntity& operator=(CEntity& right);

	// ��ʼ����������
	virtual void InitProperty();	
};

#endif//DB_ENTITY_H
