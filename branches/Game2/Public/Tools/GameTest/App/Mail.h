


#ifndef _mail_h_
#define _mail_h_
#pragma once


class CMail
{
public:
	CMail(void);
	~CMail(void);

	//	@ �Ƿ�õ�����
	bool	IsGetData(void)		{ return m_bHasGetData; }
	void	SetGetData(bool b)	{ m_bHasGetData = b; }

	//	@	ɾ���ʼ���Ʒ
	void ReleaseMailGoods(stMailData& stMail);

	//	@	�ʼ�����
	void DecodeFromByteArray(stMailData& stMail,BYTE* pBytes,long& lpos);

	//	@ �����ʼ�����
	void RefreshData();

private:

	//	@ �Ƿ��յ�����
	bool	m_bHasGetData;

	//	@	�ʼ��б�
	vector<stMailData>	m_vecMailData;
	//	@	��ɾ���ʼ��б�
	vector<stMailData>	m_vecShouldDelMailData;
	//	@	�ռ�������
	vector<string>			m_vecRecvMailName;
};
#endif//_mail_h_