//=============================================================================
/**
 *  file: CondiGroup.h
 *
 *  Brief:��װ�˶������������
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __CONDITION_GROUP_H__
#define __CONDITION_GROUP_H__

#pragma once

class CCondition;
class CMoveShape;
struct stModuParam;

class CCondiGroup : public BaseMemObj
{
public:
	CCondiGroup(void);
	virtual ~CCondiGroup(void);

	//���ģʽ
	enum eGrouMode
	{
		Mode_And=1,//"&&"ģʽ
		Mode_Or,//"||"ģʽ
	};
private:
	typedef vector<CCondition*>::iterator itCond;
	vector<CCondition*>	m_Conditions;

	eGrouMode	m_eMode;

private:
	//��&&������������жϽ��
	bool ResultByAnd(CMoveShape* pDest,stModuParam *pParam);
	//��||������������жϽ��
	bool ResultByOr(CMoveShape* pDest,stModuParam *pParam);
public:
	//
	void InitBaseData(vector<CCondition*> &vConditon,eGrouMode mode)
	{
		m_Conditions = vConditon;	
		m_eMode = mode;
	}
	//������������
	void Reset(CMoveShape* pUser);
	//�õ����
	bool Result(CMoveShape* pDest,stModuParam *pParam);
};

#endif
