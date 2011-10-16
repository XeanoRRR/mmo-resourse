/**
* @file    PetCtrl.h
* @author  wing 
* @date    2010-6-24
* @brief   �ͻ��ˡ�������ͨ�ó�������.
*/

#pragma once 

#include "BasePetDef.h"
#include "../DataStream/DataBlockSetWriteRead.h"

class PetSetup
{
public:
	PetSetup(void);
	~PetSetup(void);

	/// ���س���ǿ����������
	bool LoadPetMaterialConfig(void);

	/// ���س����Ƽ��ӵ�����
	bool LoadPetNominateModeConfig(void);

	/// ��ӵ�CByteArray
	bool CodeToDataBlock(DBWriteSet& setWriteDB);		

	/// ����
	bool DecodeFromDataBlock(DBReadSet& setReadDB);

	/// ȡ��ǿ�������б�
	MATERIAL_LIST* GetMaterialList(void) { return &m_materialList; }

	/// ����ǿ������ȡ��ǿ��˵������
	string GetExplainTextByID(long type);

private:
	MATERIAL_LIST	m_materialList;							///< �̴�ǿ������
};