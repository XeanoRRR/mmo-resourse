

#include "UpdateFolder.h"

#pragma once


class UpdatePack : public UpdateFolder
{
public:
	UpdatePack(VOID);
	~UpdatePack(VOID);

public:
	//!		���뵽���ݼ�
	BOOL	Code(DBWriteSet &setWriteDB);
	//!		�����ݼ�����
	BOOL	DeCode(DBReadSet &setReadDB);

	//!		���ø��°��ķ��������ͺͰ汾��
	VOID	SetBaseInfo(DWORD dwGappType, LPCSTR pVersion, LPCSTR pReadMe);
	//!		��÷���������
	DWORD	GetGappType(VOID);
	//!		��ð汾��
	LPCSTR	GetVersion(VOID);
	//!		�õ�˵���ı�
	LPCSTR	GetReadMe(VOID);

protected:
	//!		�ݹ�ı���ӿ�
	VOID	Code(DBWriteSet &setWriteDB, tagDirectory &Directory);
	//!		�ݹ�Ľ���ӿ�
	VOID	DeCode(DBReadSet &setReadDB, tagDirectory &Directory);

protected:
	//! ����������
	/////////////////////////////////////////////////////////////////////////////////////////
	//!				����Ŀ�����������
	DWORD			m_dwGappType;
	//!				���°��汾��
	char			m_szGappVersion[MAX_GAPP_NAME_SZIE];

	//!				���°�����
	char			m_szReadMe[PACK_README_TEXT_SIZE];

private:
	//! �����������ͷ����ݣ���˲�������󿽱�
	UpdatePack(const UpdatePack&);
	UpdatePack& operator=(const UpdatePack&);
};