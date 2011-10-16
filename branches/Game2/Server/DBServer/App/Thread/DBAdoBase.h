/**
*File:  DBAdoBase.h
*Brief: ������:�ö���ά��һ�����ݿ�Cn����
 ---------------------------------------��־���ݿ�����ķ�װ---------------------------------------
	ֻ��ִ���޷��ص�SQL���
 ������ʱ�Զ�����DB������ʱ�ͷ�����
 ������ʧЧ��ÿִ��SQL���ʱ�������³�������
 ����ִ�н�����Ĳ���(���������ָ�룬���಻���Ա�����ⲿ���ô������ö���ָ�뼴��)
---------------------------------------------------------------------------------------------------
*Author:������
*Date:  2008.11.03
*/
#ifndef DB_ADOBASE_H
#define DB_ADOBASE_H

#include "../../../../Public/ServerPublic/db/CnPool.h"
class DBAdoBase
{
public:
	DBAdoBase(string strConnectionString);
	virtual ~DBAdoBase(void);

public:
	/// ִ��SQL���
	bool ExecuteCn(LPCSTR sql);
	// ȡ�����Ӷ���
	_ConnectionPtr& GetConnPtr(void) { return m_cnPtr; }
	
	/// ������
	bool OpenCn(void);
	/// �ر�����
	bool CloseCn(void);
    /// ���������
    long CreateRs(_RecordsetPtr &rs);
    /// �򿪽����
    long OpenRs(LPCSTR sql, _RecordsetPtr &rs,
        CursorTypeEnum cursorType = adOpenStatic,
        LockTypeEnum lockType = adLockReadOnly,
        long lOptions = adCmdText);

    /// �رս����
    long CloseRs(_RecordsetPtr &rs);
    /// �ͷŽ����
    long ReleaseRs(_RecordsetPtr &rs);
    /// ִ�н����(������ִ�С��ͷ�)����Ҫ���ؽ����,��Ҫ���øýӿ�
    bool ExecuteRs(LPCSTR sql, _RecordsetPtr &rs);

	/// �����ݿ����Ӷ���
	_ConnectionPtr m_cnPtr;

	/// �����ַ�������
	std::string m_strConnectionString;

	/// ��������#
	void PrintErr(LPCSTR msg, _com_error &e);
	void PrintErr(LPCSTR msg);
};

#endif