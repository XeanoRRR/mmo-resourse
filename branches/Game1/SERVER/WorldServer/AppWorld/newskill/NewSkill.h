//////////////////////////////////////////////////////////////////////////////////////////////////
///  NewSkill.h
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\WorldServer\AppWorld\New Skill
///
///  CREATED: 08/06/2007 13:49:01 PM by ���Ƚ�
///
///  PURPOSE:   ������
///////////////////////////////////////////////////////////////////////////////////////////////////
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifndef _INC_NEWSKILL_INCLUDED
#define _INC_NEWSKILL_INCLUDED
#include "../setup/NewSkillList.h"

class CNewSkill : public CNewSkillProperties,public BaseMemObj
{
public:
	
	CNewSkill(void);
	CNewSkill(DWORD dwSkillID);
	virtual ~CNewSkill(void);
	
	void	AddNewSkill			( tagSkill *pSkill);													//��Ӽ���
	void	AddDoProcessToByteArray( vector<BYTE>& vRet,list<tagDoProcess*> &ltgDoProcess);				//���̱���
	bool	AddNewSkillToByteArray( vector<BYTE>& vRet);												//ѹ�뼼��
	void	AddConditionLength( vector<BYTE>& vRet,tagCondition *ptgCondition);							//��������
	void	Clear();																					//�ͷ�
	void	ClearCDo(tagCDo* ptgCDo);																	//�ͷ�CDo	
	void	ClearDo(tagBaseChange *ptgBaseChange);
	void	ClearCondition( void *pCondition,string strType);											//�ͷ�����
};


#endif//_INC_NEWSKILL_INCLUDED
