/** 
* @file   PetManager.h
* @author wing
* @brief  ��������࣬���������ع�����������Ϣ������.
*
* @date   2010-06-01       
*/

#pragma once

class PetManager
{
public:
	PetManager(void);
	~PetManager(void);

public:
	/// �����޸�����
	void OnChangePetName(const CGUID& guid, const char* name);
	/// ɾ������
	void OnDeletePet(const CGUID& guid);
	/// ��������
	void OnPetLevUpgrade(const CGUID& guid);
	/// �����������еȼ�
	void OnPetDLevUpgrade(const CGUID& guid);
	/// ����ı�״̬
	void OnPetChangeState(const CGUID& guid);

private:
	/// ���ݳ���GUIDȡ�ó���
	CPet* GetPet(const CGUID& guid);
};
