#ifndef BASE_ITEM_DEF_H
#define BASE_ITEM_DEF_H


	//�����������
	enum eClass
	{
		C_Human=0,		//����
		C_Element,		//Ԫ��
		C_Alien,		//����
		C_Animal,		//����
		C_Undead,		//����
		C_Evil,			//��ħ
		C_Hexapods,		//����

		C_Max,
	};

	//Ԫ������
	typedef enum eElementalType
	{
		ET_Fire=0,		//��ϵ
		ET_Water,		//ˮϵ
		ET_Earth,		//��ϵ
		ET_Wind,		//��ϵ
		ET_Dark,		//��ϵ
		ET_Holy,		//ʥϵ

		ET_Max,
	}eEleType;

	enum eNpcType
	{
		NT_Normal=0,		//��ͨNPC
		NT_Monster,		//����
		NT_Guard,		//����
	};

	enum eOccupation
	{
		OCC_Novice=0,			//������
		OCC_Fighter,			//սʿ
		OCC_Mystic,				//��ʦ(���෨ʦ)
		OCC_Rogue,				//����
		OCC_Knight,				//��ʿ
		OCC_Warrior,			//��ʿ
		OCC_Scout,				//���
		OCC_Craftsman,			//����
		OCC_Warlock,			//��ʿ
		OCC_Cleric,				//��ʦ
		OCC_Paladin,			//ʥ��ʿ
		OCC_DarkKnight,			//����ʿ
		OCC_Gladiator,			//�Ƕ�ʿ
		OCC_Berserker,			//��սʿ
		OCC_Ranger,				//Ѳ��
		OCC_Assassin,			//�̿�
		OCC_BladeDancer,		//������
		OCC_Bard,				//����ʫ��
		OCC_Sorcerer,			//��ʦ
		OCC_Necromancer,		//������ʦ
		OCC_Bishop,				//����
		OCC_Prophet,			//����(��֪)

		OCC_Max,				//ְҵ�����
	};

	//Debuf������
	enum eDefbuffType
	{
		DFT_Bled,		//��ѪDebuf
		DET_Dizzy,		//����
		DET_Lull,		//���
		DET_Hypnosis,	//����
		DET_Poison,		//�ж�
		DET_Fix,		//����
		DET_Max,
	};

	//�����������
	const int MONSTER_SKILL_SUM=6;
	const int MONSTER_AI_REFERRENCE_NUM=5;


#endif//BASE_ITEM_DEF_H