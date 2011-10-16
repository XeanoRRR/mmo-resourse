//////////////////////////////////////////////////////////////////////////////////////////////////
///  NewSkill.cpp
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\Server/WorldServer\App/New Skill
///
///  CREATED: 08/06/2007 13:49:01 PM by ���Ƚ�
///
///  PURPOSE:   ������
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "NewSkill.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/*
NAME:CNewSkill()
DESC:Ĭ�Ϲ��캯��
PARA:��
*/

CNewSkill ::CNewSkill()
{
	m_dwID = 0;	
}

/*
NAME:CNewSkill(DWORD dwID)
DESC:���캯��
PARA:dwIDΪ���ܵ�ID
*/

CNewSkill::CNewSkill(DWORD dwID)
{
	m_dwID = dwID;
}

/*
NAME:~CNewSkill()
DESC:��������
PARA:��
*/
CNewSkill::~CNewSkill()
{
	Clear();
}

/*
NAME:AddNewSkill()
DESC:��Ӽ���
PARA:pSkill���ܵ���Ҫ�ṹ
*/
void CNewSkill::AddNewSkill( tagSkill *pSkill)
{
	m_lSkill.push_back(pSkill);
}

/*
NAME:AddConditionLength()
DESC:��������
PARA:vRet����
	 ptgCondition����ָ��
*/
void CNewSkill::AddConditionLength( vector<BYTE>& vRet,tagCondition *ptgCondition)
{
	DWORD length=0;
	string strType = ptgCondition->strType;

	length = ptgCondition->dwLength;
	_AddToByteArray(&vRet,length);

	if (strType=="BaseProperty")
	{	
		//��������
		tagConProperty *ptgConProperty = reinterpret_cast<tagConProperty*>(ptgCondition->pVoidCondition);
		for (list<tagPropertyCondition*>::iterator it = ptgConProperty->ltgProtertyConditon.begin();
			it!=ptgConProperty->ltgProtertyConditon.end();++it)
		{			
			_AddToByteArray(&vRet,(*it)->dwValue);
			_AddToByteArray(&vRet,(char*)(*it)->cmpareOp.c_str());
			_AddToByteArray(&vRet,(char*)(*it)->strName.c_str());
			_AddToByteArray(&vRet,(char*)(*it)->strObject.c_str());	
            _AddToByteArray(&vRet,(*it)->paramvalue.c_str() );
		}		
	}
	else if (strType=="Good")
	{
		//��Ʒ
		tagConGood *ptgConGood = reinterpret_cast<tagConGood*>(ptgCondition->pVoidCondition);
		for (list<tagGoodCondition*>::iterator it = ptgConGood->ltgGoodCondition.begin();
			it!=ptgConGood->ltgGoodCondition.end();++it)
		{			
			_AddToByteArray(&vRet,(*it)->dwPlace);
			_AddToByteArray(&vRet,(*it)->dwPos);
			_AddToByteArray(&vRet,(*it)->dwValue);
			_AddToByteArray(&vRet,(char*)(*it)->strCmpFun.c_str());
			_AddToByteArray(&vRet,(char*)(*it)->strName.c_str());
			_AddToByteArray(&vRet,(*it)->dwType);
			_AddToByteArray(&vRet,(char*)(*it)->strObject.c_str());
			_AddToByteArray(&vRet,(long)((*it)->ltgGoodAddProperty.size()));
			for (list<tagGoodAddProperty*>::iterator git=(*it)->ltgGoodAddProperty.begin();git!=(*it)->ltgGoodAddProperty.end();++git)
			{
				_AddToByteArray(&vRet,(*git),sizeof(tagGoodAddProperty));
			}
		}
	}
	else if (strType=="Buff")
	{
		//״̬
		tagConBuff *ptgConBuff= reinterpret_cast<tagConBuff*>(ptgCondition->pVoidCondition);
		for (list<tagBuffCondition*>::iterator it = ptgConBuff->ltgBuffCondition.begin();
			it!=ptgConBuff->ltgBuffCondition.end();++it)
		{
			_AddToByteArray(&vRet,(*it)->dwID);
			_AddToByteArray(&vRet,(*it)->dwLevel);
			_AddToByteArray(&vRet,(*it)->dwNum);
            _AddToByteArray(&vRet,(*it)->strOper.c_str());
			_AddToByteArray(&vRet,(char*)(*it)->strObject.c_str());
            _AddToByteArray(&vRet,(char*)(*it)->strBufObject.c_str());
            _AddToByteArray(&vRet,(char*)(*it)->strLevel.c_str());
		}
	}
    else if(strType=="Odds")
    {
        tagConOdds* ptgConOdds = reinterpret_cast<tagConOdds*>(ptgCondition->pVoidCondition);
        for (list<tagOddsCondition*>::iterator it = ptgConOdds->ltgOddsCondition.begin();
            it!=ptgConOdds->ltgOddsCondition.end();++it)
        {
            _AddToByteArray(&vRet,(*it)->value);
            _AddToByteArray(&vRet,(*it)->strCmpFun.c_str());
            _AddToByteArray(&vRet,(*it)->strValueName.c_str());
        }
    }
	else if (strType=="Distance")
	{
		//����
		tagConDistance *ptgConDistance = reinterpret_cast<tagConDistance*>(ptgCondition->pVoidCondition);
		for (list<tagDistanceCondition*>::iterator it = ptgConDistance->ltgDistanceCondition.begin();
			it!=ptgConDistance->ltgDistanceCondition.end();++it)
		{			
			_AddToByteArray(&vRet,(*it)->value);
			_AddToByteArray(&vRet,(char*)(*it)->strCmpFun.c_str());
			_AddToByteArray(&vRet,(char*)(*it)->strRefObj.c_str());
            _AddToByteArray(&vRet,(char*)(*it)->strValueName.c_str());
		}
	}
	else if (strType=="ValidObject")
	{
		//��Ч����
		tagConValidObject *ptgConValidObject = reinterpret_cast<tagConValidObject*>(ptgCondition->pVoidCondition);
		for (list<tagValidObjectCondition*>::iterator it = ptgConValidObject->ltgValidObjectCondition.begin();
			it!=ptgConValidObject->ltgValidObjectCondition.end();++it)
		{			
			_AddToByteArray(&vRet,(char*)(*it)->strType.c_str());			
			_AddToByteArray(&vRet,(long)(*it)->lObjType.size());
			for(list<string>::iterator Oit= (*it)->lObjType.begin();Oit!=(*it)->lObjType.end();++Oit)
			{
				_AddToByteArray(&vRet,(char*)(*Oit).c_str());
			}
		}
	}
	else if (strType=="Event")
	{
		//�¼�
		tagConEvent *ptgConEvent = reinterpret_cast<tagConEvent*>(ptgCondition->pVoidCondition);
		for (list<tagEventCondition*>::iterator it = ptgConEvent->ltgEventCondition.begin();
			it!=ptgConEvent->ltgEventCondition.end();++it)
		{
			_AddToByteArray(&vRet,(*it)->dwFlag);
			_AddToByteArray(&vRet,(char*)(*it)->strEventName.c_str());
			_AddToByteArray(&vRet,(*it)->dwVar1);
			_AddToByteArray(&vRet,(*it)->dwVar2);
			_AddToByteArray(&vRet,(*it)->dwVar3);
			_AddToByteArray(&vRet,(*it)->dwVar4);
            _AddToByteArray(&vRet,(*it)->dwSkillId.c_str());
			_AddToByteArray(&vRet,(char*)(*it)->strName.c_str());
		}

	}
	else if (strType=="Timer")
	{
		//ʱ��
		tagConTimer *ptgConTimer = reinterpret_cast<tagConTimer*>(ptgCondition->pVoidCondition);
		for (list<DWORD>::iterator it=ptgConTimer->ldwConTimer.begin();
			it!=ptgConTimer->ldwConTimer.end();++it)
		{
			_AddToByteArray(&vRet,*it);
		}

	}
    else if (strType=="Dispel")
    {
        tagConDispel *ptgConDispel = reinterpret_cast<tagConDispel*>(ptgCondition->pVoidCondition);
        for (list<tagDispelCondition*>::iterator it=ptgConDispel->ltgDispelCondition.begin();
            it!=ptgConDispel->ltgDispelCondition.end();++it)
        {
            _AddToByteArray(&vRet,(*it)->Num);
            _AddToByteArray(&vRet,(char*)(*it)->strDispelNum.c_str());
            _AddToByteArray(&vRet,(char*)(*it)->strDispelType.c_str());
        }
    }
	else if (strType=="Script")
	{
		//�ű�
		tagConScript *ptgConScript = reinterpret_cast<tagConScript*>(ptgCondition->pVoidCondition);
		_AddToByteArray(&vRet,(char*)ptgConScript->strScript.c_str());
	}
	else if (strType=="Block")
	{
		//�赲
		tagConBlock *ptgConBlcok = reinterpret_cast<tagConBlock*>(ptgCondition->pVoidCondition);
		for (list<tagBlockCondition*>::iterator it=ptgConBlcok->ltgBlockCondition.begin();
			 it!=ptgConBlcok->ltgBlockCondition.end();++it)
		{
			_AddToByteArray(&vRet,(char*)(*it)->strType.c_str());
			_AddToByteArray(&vRet,(char*)(*it)->strDir.c_str());
			_AddToByteArray(&vRet,(*it)->dwBlockType);
			_AddToByteArray(&vRet,(*it)->dwShapeType);
		}
	}
	else if (strType=="Face")
	{
		tagConFace *ptgConFace = reinterpret_cast<tagConFace*>(ptgCondition->pVoidCondition);
		for (list<tagFaceCondition*>::iterator it=ptgConFace->ltgFaceCondition.begin();
			it!=ptgConFace->ltgFaceCondition.end();++it)
		{
			_AddToByteArray(&vRet,(char*)(*it)->strFaceType.c_str());
		}
	}
	else if (strType=="Region")
	{
		tagConRegion *ptgConRegion = reinterpret_cast<tagConRegion*>(ptgCondition->pVoidCondition);
		for(list<tagRegionCondition*>::iterator it=ptgConRegion->ltgRegionCondition.begin();
			it!=ptgConRegion->ltgRegionCondition.end();++it)
		{
			_AddToByteArray(&vRet,(char*)(*it)->strName.c_str());
			_AddToByteArray(&vRet,(DWORD)(*it)->dwType);
		}
	}
	/*else if (strType=="Occupation")
	{
		tagConOccupation *ptgConOccupation = reinterpret_cast<tagConOccupation*>(ptgCondition->pVoidCondition);

		for (list<tagOccupationCondition*>::iterator it=ptgConOccupation->ltgOccupationCondition.begin();
			it!=ptgConOccupation->ltgOccupationCondition.end();++it)
		{
			_AddToByteArray(&vRet,(long)(*it)->lOccu.size());
			for (list<long>::iterator Oit=(*it)->lOccu.begin();Oit!=(*it)->lOccu.end();++Oit)
			{
				_AddToByteArray(&vRet,(*Oit));
			}
		}
	}	*/
}
/*
NAME:AddNewSkillToByteArray()
DESC:����
PARA:vRet����
*/
bool CNewSkill::AddNewSkillToByteArray(vector<BYTE>& vRet)
{
	_AddToByteArray(&vRet, m_dwID);
	_AddToByteArray(&vRet, (char*)m_strName.c_str());
	_AddToByteArray(&vRet, (char*)m_strInitSricpt.c_str());
	_AddToByteArray(&vRet, m_dwElement);
	_AddToByteArray(&vRet, (char*)m_strType.c_str());
	_AddToByteArray(&vRet, m_bUseType);
	_AddToByteArray(&vRet, m_bOnly);
	_AddToByteArray(&vRet, m_dwOnlyType);
	_AddToByteArray(&vRet, m_bNameOnly);
	_AddToByteArray(&vRet, m_bRoleOnly);
	_AddToByteArray(&vRet, m_bAttack);
	_AddToByteArray(&vRet, m_bTarget);
	_AddToByteArray(&vRet, m_bCooldown);
	_AddToByteArray(&vRet, m_bDeadUse);
	_AddToByteArray(&vRet, m_bDeadEnd);
	_AddToByteArray(&vRet, m_bBreak);
	_AddToByteArray(&vRet, m_bPetType);
	_AddToByteArray(&vRet, m_bTime);
	_AddToByteArray(&vRet, m_bSystem);
	_AddToByteArray(&vRet, m_dwTotalNum);
	//_AddToByteArray(&vRet,m_bSkillAdd);
    _AddToByteArray(&vRet, m_CDControl);
	_AddToByteArray(&vRet,(char*)m_strAddSub.c_str());
    _AddToByteArray(&vRet,(char*)m_strAttType.c_str());

	
	//��ʽ����
	_AddToByteArray(&vRet,(long)m_mapFormat.size());
	for (map<DWORD,tagExpression>::iterator it=m_mapFormat.begin();
		it!=m_mapFormat.end();++it)
	{
		//��ʽID
		_AddToByteArray(&vRet,it->first);
		//��ʽ
		_AddToByteArray(&vRet,(char*)((*it).second.strFileName.c_str() ));
		_AddToByteArray(&vRet,(char*)((*it).second.strContent.c_str() ));
	}

	//������
	//����
	_AddToByteArray(&vRet,(long)m_mapConditionGroup.size());
	for (map<DWORD,tagConditionGroup*>::iterator it=m_mapConditionGroup.begin();
		it!=m_mapConditionGroup.end();++it)
	{
		//ID
		_AddToByteArray(&vRet,it->first);
		//���ģʽ
		_AddToByteArray(&vRet,(char*)((it->second)->strMode.c_str()));
		//����������
		_AddToByteArray(&vRet,(long)(it->second)->lConditonGroup.size());		

		for (list<DWORD>::iterator cit=(it->second)->lConditonGroup.begin();
			cit!=(it->second)->lConditonGroup.end();++cit)
		{				
			_AddToByteArray(&vRet,*cit);
		}


	}

	//������
	_AddToByteArray(&vRet,(long)m_mapCondition.size());

	for (map<DWORD,tagCondition*>::iterator it=m_mapCondition.begin();
		it!=m_mapCondition.end();++it)
	{
		//ID
		_AddToByteArray(&vRet,it->first);			
		tagCondition *ptgCondition = it->second;


		_AddToByteArray(&vRet,(char*)ptgCondition->strType.c_str());
		AddConditionLength(vRet,ptgCondition);
	}

	//���ö���
	//����
	_AddToByteArray(&vRet,(long)m_mapOperateObject.size());

	for (map<DWORD,tagOperateObject*>::iterator Oit=m_mapOperateObject.begin();
		Oit!=m_mapOperateObject.end();++Oit)
	{			
		_AddToByteArray(&vRet,(Oit->second)->dwID);
		_AddToByteArray(&vRet,(Oit->second)->distance);
		_AddToByteArray(&vRet,(Oit->second)->radius);
		_AddToByteArray(&vRet,(Oit->second)->angle);
		_AddToByteArray(&vRet,(Oit->second)->width);
		_AddToByteArray(&vRet,(Oit->second)->dwSkillID);
		_AddToByteArray(&vRet,(Oit->second)->dwSkillLevel);
		_AddToByteArray(&vRet,(Oit->second)->dwConGroupID);		
		_AddToByteArray(&vRet,(Oit->second)->dwNumObject);

		_AddToByteArray(&vRet,(char*)(Oit->second)->strType.c_str());
		_AddToByteArray(&vRet,(char*)(Oit->second)->strPosition.c_str());
		_AddToByteArray(&vRet,(char*)(Oit->second)->strCenterObject.c_str());
		_AddToByteArray(&vRet,(char*)(Oit->second)->strObject.c_str());
        _AddToByteArray(&vRet,(char*)(Oit->second)->NumObjectName.c_str());
        _AddToByteArray(&vRet,(Oit->second)->IsWithIn);
	}

	//�����ȼ����ܸ���
	_AddToByteArray(&vRet,(long)m_lSkill.size());

	for (list<tagSkill*>::iterator it=m_lSkill.begin(); it!=m_lSkill.end(); ++it)
	{
		//temp id
		_AddToByteArray(&vRet,(*it)->dwTemplateId);
		//�ڲ���������
		_AddToByteArray(&vRet,(long)(*it)->mapCondition.size()); 

		for (map<DWORD,tagCondition*>::iterator Cit=(*it)->mapCondition.begin();
			Cit!=(*it)->mapCondition.end();++Cit)
		{
			//ID
			_AddToByteArray(&vRet,Cit->first);			
			tagCondition *ptgCondition = Cit->second;
			_AddToByteArray(&vRet,(char*)ptgCondition->strType.c_str());
			AddConditionLength(vRet,ptgCondition);
		}
		

		//����
		//����
		_AddToByteArray(&vRet,(long)(*it)->ltgExecuteStep.size());
		for (list<tagProcess*>::iterator Sit=(*it)->ltgExecuteStep.begin();
			Sit!=(*it)->ltgExecuteStep.end();++Sit)
		{
			//��������
			_AddToByteArray(&vRet,(char*)(*Sit)->strName.c_str());

			//ִ�в������
			_AddToByteArray(&vRet,(long)(*Sit)->ltgStep.size());
			for(list<tagStep*>::iterator Stepit=(*Sit)->ltgStep.begin();
				Stepit!=(*Sit)->ltgStep.end();++Stepit)
			{
				//����
				_AddToByteArray(&vRet,(char*)(*Stepit)->strName.c_str());

				//Do����
				_AddToByteArray(&vRet,(long)(*Stepit)->ltgDo.size());

				for (list<tagDo*>::iterator SDit=(*Stepit)->ltgDo.begin();
					SDit!=(*Stepit)->ltgDo.end();++SDit)
				{
					//����						
					//���ö���
					_AddToByteArray(&vRet,(*SDit)->dwOperobjid);
					//���ؽ��
					_AddToByteArray(&vRet,(char*)(*SDit)->strRet.c_str());

					//CDo��¦
					_AddToByteArray(&vRet,(long)(*SDit)->ltgCDo.size());

					for (list<tagCDo*>::iterator Dit=(*SDit)->ltgCDo.begin();
						Dit!=(*SDit)->ltgCDo.end();++Dit)
					{
						//����
						_AddToByteArray(&vRet,(char*)(*Dit)->tgCondition.strTrueRet.c_str());
						//�ٽ��
						_AddToByteArray(&vRet,(char*)(*Dit)->tgCondition.strFalseRet.c_str());
						//������ID
						_AddToByteArray(&vRet,(*Dit)->tgCondition.dwConditionGroup);
						//�洦�����
						_AddToByteArray(&vRet,(long)(*Dit)->ltgTrueDoProcess.size());
						AddDoProcessToByteArray(vRet,(*Dit)->ltgTrueDoProcess);
						//�ٴ���
						_AddToByteArray(&vRet,(long)(*Dit)->ltgFalseDoProcess.size());
						AddDoProcessToByteArray(vRet,(*Dit)->ltgFalseDoProcess);						
					}			


				}	

			}

		}
	}
	return true;
}
/*
NAME:AddDoProcessToByteArray()
DESC:DoProcess����
PARA:vRet����
	 ltgDoProcess�����б�
*/
void CNewSkill::AddDoProcessToByteArray(vector<BYTE>& vRet,list<tagDoProcess*> &ltgDoProcess)
{


	for (list<tagDoProcess*>::iterator DFPit=ltgDoProcess.begin();
		DFPit!=ltgDoProcess.end();++DFPit)
	{
		//���������
		_AddToByteArray(&vRet,(long)(*DFPit)->ltgCDo.size());

		for (list<tagCDo*>::iterator Dit=(*DFPit)->ltgCDo.begin();
			Dit!=(*DFPit)->ltgCDo.end();++Dit)
		{
			//����
			_AddToByteArray(&vRet,(char*)(*Dit)->tgCondition.strTrueRet.c_str());
			//�ٽ��
			_AddToByteArray(&vRet,(char*)(*Dit)->tgCondition.strFalseRet.c_str());
			//������ID
			_AddToByteArray(&vRet,(*Dit)->tgCondition.dwConditionGroup);
			//�洦�����
			_AddToByteArray(&vRet,(long)(*Dit)->ltgTrueDoProcess.size());
			AddDoProcessToByteArray(vRet,(*Dit)->ltgTrueDoProcess);
			//�ٴ���
			_AddToByteArray(&vRet,(long)(*Dit)->ltgFalseDoProcess.size());
			AddDoProcessToByteArray(vRet,(*Dit)->ltgFalseDoProcess);						
		}		


		//����
		_AddToByteArray(&vRet,(long)(*DFPit)->ltgBaseChange.size());
		for (list<tagBaseChange*>::iterator it=(*DFPit)->ltgBaseChange.begin();
			it!=(*DFPit)->ltgBaseChange.end();++it)
		{
			_AddToByteArray(&vRet,(char*)(*it)->strChangeType.c_str());
			_AddToByteArray(&vRet,(char*)(*it)->strObj.c_str());
			
			if ((*it)->strChangeType=="Script")
			{
				//�ű�
				tagScriptID *pTemp = dynamic_cast<tagScriptID*>(*it);				
				_AddToByteArray(&vRet,pTemp->dwScriptID);				
			} 
			else if((*it)->strChangeType=="ObjectPropertyChange")
			{
				//����
				tagChangeProperty *pTemp = dynamic_cast<tagChangeProperty*>(*it);				
				_AddToByteArray(&vRet,pTemp->dwValue);
				_AddToByteArray(&vRet,(char*)pTemp->strName.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strOperator.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strScriptVarName.c_str());
                _AddToByteArray(&vRet,(char*)pTemp->paramname.c_str());

			}
			else if ((*it)->strChangeType=="BuffChange")
			{
				//״̬
				tagChangeBuff *pTemp = dynamic_cast<tagChangeBuff*>(*it);			
				_AddToByteArray(&vRet,pTemp->dwID);
				_AddToByteArray(&vRet,pTemp->dwLevel);

				_AddToByteArray(&vRet,pTemp->dwCount);
				_AddToByteArray(&vRet,(char*)pTemp->strSIDName.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strSLvName.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strSCountName.c_str());        


				_AddToByteArray(&vRet,(char*)pTemp->strOperator.c_str());
                _AddToByteArray(&vRet,(char*)pTemp->strSkillType.c_str());

			}
			else if ((*it)->strChangeType=="PosChange")
			{
				//λ��
				tagChangePos *pTemp = dynamic_cast<tagChangePos*>(*it);				
				_AddToByteArray(&vRet,(char*)pTemp->strDir.c_str());				
				_AddToByteArray(&vRet,(char*)pTemp->strRefObject.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strStyle.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strSpeed.c_str());
                _AddToByteArray(&vRet, pTemp->dis);

			} 
			else if ((*it)->strChangeType=="AttakObjectChange")
			{
				tagAttackChangeObject *pTemp = dynamic_cast<tagAttackChangeObject*>(*it);
				_AddToByteArray(&vRet,(char*)pTemp->strAttackChangeObject.c_str());

			}
			else if ((*it)->strChangeType=="CooldownChange")
			{
				//ʱ��
				tagCoolDownChange *pTemp = dynamic_cast<tagCoolDownChange*>(*it);
				_AddToByteArray(&vRet,pTemp->dwValue);
				_AddToByteArray(&vRet,(char*)pTemp->strType.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strSVarianame.c_str());
                _AddToByteArray(&vRet,(char*)pTemp->paramname.c_str());

			} 
			else if ((*it)->strChangeType=="DestObjsChange")
			{
				//Ŀ�����
				tagDestObjChange *pTemp = dynamic_cast<tagDestObjChange*>(*it);
				_AddToByteArray(&vRet,(char*)pTemp->strDestObjChange.c_str());
			}
			else if ((*it)->strChangeType=="RegisterTimer")
			{
				//ע�ᶨʱ��
				tagRegisterTimer *pTemp = dynamic_cast<tagRegisterTimer*>(*it);
				_AddToByteArray(&vRet,pTemp->dwCount);
				_AddToByteArray(&vRet,pTemp->dwFlag);
				_AddToByteArray(&vRet,pTemp->dwFuture);
				_AddToByteArray(&vRet,pTemp->dwIntelTime);
				_AddToByteArray(&vRet,(char*)pTemp->strSCountName.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strSFutvarname.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strSIntelName.c_str());
                _AddToByteArray(&vRet,(char*)pTemp->strVFuturename.c_str());
                _AddToByteArray(&vRet,(char*)pTemp->strVIntername.c_str());
                _AddToByteArray(&vRet,(char*)pTemp->strVCountname.c_str());
			} 
			else if ((*it)->strChangeType=="RegisterEvent")
			{
				//ע���¼�
				tagRegisterEvent *pTemp = dynamic_cast<tagRegisterEvent*>(*it);
				_AddToByteArray(&vRet,pTemp->dwFlag);
				_AddToByteArray(&vRet,(char*)pTemp->strName.c_str());
			}
			else if ((*it)->strChangeType=="UnRegisterTimer")
			{
				//ע��ʱ��
				tagUnRegisterTimer *pTemp = dynamic_cast<tagUnRegisterTimer*>(*it);
				_AddToByteArray(&vRet,pTemp->dwUnRegisterTimer);
			} 
			else if ((*it)->strChangeType=="UnRegisterEvent")
			{
				//ע���¼�
				tagUnRegisterEvent *pTemp = dynamic_cast<tagUnRegisterEvent*>(*it);
				_AddToByteArray(&vRet,pTemp->dwUnRegisterEvent);
			}
			else if ((*it)->strChangeType=="SendMsg")
			{
				//������Ϣ
				tagSendMsg *pTemp = dynamic_cast<tagSendMsg*>(*it);
				_AddToByteArray(&vRet,pTemp->dwValue);
				_AddToByteArray(&vRet,(char*)pTemp->strType.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strRange.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strSVName.c_str());
				_AddToByteArray(&vRet,(BYTE)pTemp->nAdd);
			} 
			else if ((*it)->strChangeType=="DamageChange")
			{
				//�˺�����
				tagDamageChange *pTemp = dynamic_cast<tagDamageChange*>(*it);
				_AddToByteArray(&vRet,pTemp->dwValue);			
				_AddToByteArray(&vRet,(char*)pTemp->strOperator.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strType.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strSVarianame.c_str());
                _AddToByteArray(&vRet,(char*)pTemp->strDamagename.c_str());
			}
			else if ((*it)->strChangeType=="DoDamage")
			{
				//ִ���˺�
				tagDoDamage *pTemp = dynamic_cast<tagDoDamage*>(*it);
				_AddToByteArray(&vRet,pTemp->dwBlastAttack);
				_AddToByteArray(&vRet,pTemp->dwResult);
				_AddToByteArray(&vRet,(char*)pTemp->strSBlastVariName.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strSResVariName.c_str());
                _AddToByteArray(&vRet,(char*)pTemp->strDamagename.c_str());
                _AddToByteArray(&vRet,(char*)pTemp->strBlastname.c_str());
			} 
			else if ((*it)->strChangeType=="GoodChange")
			{
				//��Ʒ
				tagGoodChange *pTemp = dynamic_cast<tagGoodChange*>(*it);
				_AddToByteArray(&vRet,pTemp->dwPlace);
				_AddToByteArray(&vRet,pTemp->dwPos);
				_AddToByteArray(&vRet,pTemp->dwValue);
				_AddToByteArray(&vRet,(char*)pTemp->strName.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strOperator.c_str());
				_AddToByteArray(&vRet,pTemp->dwGoodsType);
				_AddToByteArray(&vRet,pTemp->dwWeaponType);

			}
			else if ((*it)->strChangeType=="DirecChange")
			{
				//����ı�
				tagDirection *pTemp = dynamic_cast<tagDirection*>(*it);
				_AddToByteArray(&vRet,(char*)pTemp->strDir.c_str());
			}
			else if ((*it)->strChangeType=="SkillChange")
			{
				//���ܲ���
				tagSkillChange *pTemp = dynamic_cast<tagSkillChange*>(*it);
				_AddToByteArray(&vRet,(char*)pTemp->strObj.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strType.c_str());
				_AddToByteArray(&vRet,(char*)pTemp->strOpera.c_str());
				_AddToByteArray(&vRet,pTemp->lID);
				_AddToByteArray(&vRet,pTemp->lLv);
			}
            else if((*it)->strChangeType=="GeniusChange")
            {
                tagGeniusChange* pTemp = dynamic_cast<tagGeniusChange*>(*it);
                _AddToByteArray(&vRet,(char*)pTemp->strGenius.c_str());
            }
		}

	}

}

/*
NAME:Clear()
DESC:�ͷ��ڴ�
PARA:��
*/
void CNewSkill::Clear()								
{

	//����
	for (map<DWORD,tagCondition*>::iterator it=m_mapCondition.begin();
		it!=m_mapCondition.end();++it)
	{
		//��������
		ClearCondition((it->second)->pVoidCondition,(it->second)->strType);		
		SAFE_DELETE(it->second);
	}
	m_mapCondition.clear();

	//������
	for (map<DWORD,tagConditionGroup*>::iterator it=m_mapConditionGroup.begin();
		it!=m_mapConditionGroup.end();++it)
	{
		//������
		SAFE_DELETE(it->second);
	}
	m_mapConditionGroup.clear();

	//���ö���
	for(map<DWORD,tagOperateObject*>::iterator it=m_mapOperateObject.begin();
		it!=m_mapOperateObject.end();++it)
	{
		SAFE_DELETE(it->second);
	}
	m_mapOperateObject.clear();

	for (list<tagSkill*>::iterator it=m_lSkill.begin();
		it!=m_lSkill.end(); ++it)
	{
        /*
		if((*it)->pStaticParam)
			SAFE_DELETE((*it)->pStaticParam);
        */
	
		for (map<DWORD,tagCondition*>::iterator cit=(*it)->mapCondition.begin();
			cit!=(*it)->mapCondition.end();++cit)
		{	
			ClearCondition((cit->second)->pVoidCondition,(cit->second)->strType);		
			SAFE_DELETE(cit->second);
		}
		(*it)->mapCondition.clear();

		for(list<tagProcess*>::iterator Pit=(*it)->ltgExecuteStep.begin();
			Pit!=(*it)->ltgExecuteStep.end();++Pit)
		{
			for (list<tagStep*>::iterator Sit=(*Pit)->ltgStep.begin();
				Sit!=(*Pit)->ltgStep.end();++Sit)
			{
				for (list<tagDo*>::iterator SDit=(*Sit)->ltgDo.begin();
					SDit!=(*Sit)->ltgDo.end();++SDit)
				{

					for (list<tagCDo*>::iterator Dit=(*SDit)->ltgCDo.begin();
						Dit!=(*SDit)->ltgCDo.end();++Dit)
					{
						ClearCDo((*Dit));
					}					
					(*SDit)->ltgCDo.clear();
					SAFE_DELETE(*SDit);
				}
				(*Sit)->ltgDo.clear();
				SAFE_DELETE(*Sit);

			}
			(*Pit)->ltgStep.clear();
			SAFE_DELETE(*Pit);

		}
		(*it)->ltgExecuteStep.clear();
		SAFE_DELETE(*it);
	}

	m_lSkill.clear();
}
