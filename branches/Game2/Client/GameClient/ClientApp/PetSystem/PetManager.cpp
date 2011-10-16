#include "stdafx.h"
#include "Pet.h"
#include "PetManager.h"
#include "../Player.h"
#include "../../Game.h"


//����������ı����״̬
bool AskforSetPetAwait(const CPet* pet)
{
    //�ȼ������ĺϷ���
    if (pet->GetPetState() != PET_STATE_FIGHT &&
        pet->GetPetState() != PET_STATE_ACTIVE)
        return false;

    //����������ı����ĵ�ǰս��״̬
    CMessage msg(MSG_C2S_PET_CHANGE_STATE_AWAIT);
    msg.Add(pet->GetExID());
    msg.Send();
    return true;
}

bool AskforSetPetFight(const CPet* pet)
{
    //�ȼ������ĺϷ���
    if (pet->GetPetState() != PET_STATE_AWAIT &&
        pet->GetPetState() != PET_STATE_ACTIVE)
        return false;

    //����������ı����ĵ�ǰս��״̬
    CMessage msg(MSG_C2S_PET_CHANGE_STATE_FIGHT);
    msg.Add(pet->GetExID());
    msg.Send();
    return true;
}

bool AskforSetPetActive(const CPet* pet)
{
    //�ȼ������ĺϷ���
    if (pet->GetPetState() != PET_STATE_FIGHT)
        return false;

    //����������ı����ĵ�ǰս��״̬
    CMessage msg(MSG_C2S_PET_CHANGE_STATE_ACTIVE);
    msg.Add(pet->GetExID());
    msg.Send();
    return true;
}

PetManager::PetManager(void)
{

}

PetManager::~PetManager(void)
{

}

/// �����޸�����
void PetManager::OnChangePetName(const CGUID& guid, const char* name)
{
	CPet* pet = GetPet(guid);
	if (!pet)
		return;
	
	pet->SetPetName(name);

	CMessage msg(MSG_C2S_PET_CHANG_NAME);
	msg.Add(pet->GetExID());
	msg.Add(name);
	msg.Send();
}

/// ɾ������
void PetManager::OnDeletePet(const CGUID& guid)
{
	CPet* pet = GetPet(guid);
	if (!pet)
		return;

	CMessage msg(MSG_C2S_PET_CANCEL);
	msg.Add(guid);
	msg.Send();
}

/// ��������
void PetManager::OnPetLevUpgrade(const CGUID& guid)
{
	CPet* pet = GetPet(guid);
	if (!pet)
		return;

	CMessage msg(MSG_C2S_PET_UPGRADE);
	msg.Add(guid);
	msg.Send();
}

/// �����������еȼ�
void PetManager::OnPetDLevUpgrade(const CGUID& guid)
{
	CPet* pet = GetPet(guid);
	if (!pet)
		return;

	CMessage msg(MSG_C2S_PET_UPGRADE_DLEV);
	msg.Add(guid);
	msg.Send();
}

/// ����ı�״̬
void PetManager::OnPetChangeState(const CGUID& guid)
{
	CPet* pet = GetPet(guid);
	if (!pet)
		return;

	long lStateType = 0;
	CMessage msg(MSG_C2S_PET_CHANGE_STATE);
	msg.Add(guid);
	msg.Add(lStateType);
	msg.Send();
}

/// ���ݳ���GUIDȡ�ó���
CPet* PetManager::GetPet(const CGUID& guid)
{
	CPet* pet = NULL;
	CPlayer* player = GetGame()->GetMainPlayer();
	if (player)
	{
		pet = player->GetPet(guid);
		return pet;
	}
	return pet;
}