/**
*File:  Entity.h
*brief: ������:�ö���ֻ�����Լ�����������,�ڽṹ���г�ΪҶ�ӽڵ�
        ��Ϸ������࣬���ж������Ϸ��ص����ɸ�������
*Author:������
*Date:  2008.11.03
*/
#ifndef ENTITY_H
#define ENTITY_H
#include "BaseEntity.h"

class CEntity : public CBaseEntity
{
public:
	CEntity(const std::string& flag, const CGUID& guid);
	virtual ~CEntity();

	// virtual interfaces
	void GetFromByteArray(BYTE* msgBuf, long& msgBufPos, long bufSize=-1);

	void AddToByteArray(std::vector<BYTE>& pBA, bool isLoadOperFlag);

	CEntity& operator=(CEntity& right);

	// ��ʼ����������
	virtual void InitProperty();		
};


#endif//DB_ENTITY_H
