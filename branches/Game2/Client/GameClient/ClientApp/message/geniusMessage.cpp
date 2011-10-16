/********************************************************************
	created:	2010/10/18
	created:	18:10:2010   9:55
	filename: 	e:\SVN_P6\trunk\Client\GameClient\ClientApp\message\geniusMessage.cpp
	file path:	e:\SVN_P6\trunk\Client\GameClient\ClientApp\message
	file base:	geniusMessage
	file ext:	cpp
	author:		zhaohang(kaba) (kuailekaba@gmail.com)
	
	purpose:	�츳��Ϣģ��
*********************************************************************/
#include "stdafx.h"
#include "../../../Net/Message.h"
#include "../Skills/SkillListXml.h"
//��Ӧ�츳��Ϣ
void OnGeniusMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
		//���¼�������
	case MSG_S2C_GENIUS_SKILLUPDATE:
		//����ģ��ṹ
		// 1.SkillID ------����ID 
		// 2.SkillLevel ---���ܵȼ�
		// 3.size ------���¼����������� 
		//     |__ strKey ----������Ĺؼ���
		//     |__ value  ----�������ֵ
		long SkillID = pMsg->GetDWord();
		long SkillLevel = pMsg->GetLong();
		long size = pMsg->GetLong();
		std::vector<string>vecStrKey;
		std::vector<long>vecValue;
		for(long i = 0 ;i < size ; i++)
		{
			char temp[64];
			vecStrKey.push_back(pMsg->GetStr(temp,64));
			vecValue.push_back(pMsg->GetLong());
		}
		//���ø��½ӿ�,����ȫ�ּ���ģ������
		UpdateSkillXmlList(SkillID,SkillLevel,vecStrKey,vecValue);
		break;
	}
}