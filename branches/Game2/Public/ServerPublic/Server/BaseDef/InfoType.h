#pragma once
//��������Ϣ����
enum eInfoType
{
	CONNECTS=0,								//������
	AITICKS,								//ѭ������

	//LS
	LS_PLAYER=			10,					//��������
	LS_WRONGCDKEY,							//����CDKey
	//LS end---

	//DBS
	DBS_LASTSAVETIME	=	40,		        //���һ�α�����ʱ
	DBS_MEMORY,						        //�ڴ�ʹ��
	DBS_OPERMAIL,		                    //42	�ʼ�������
	DBS_OPERFACTION,				        //��������
	DBS_OPERPLAYER,		                    //44	��ɫ������
	DBS_LOADENTITY,					        //����ʵ����
	DBS_SAVEENTITY,		                    //46	����ʵ����
	DBS_LOADNUM,					        //������
	DBS_SAVENUM,		                    //48	������
	DBS_LOG,						        //��־
	DBS_REMSG,			                    //50	������Ϣ
	DBS_LOADTHREAD,					        //����ʹ�ý�����
	DBS_SAVETHREAD,		                    //52	����ʹ�ý�����
	DBS_SAVE_ALL,		                    //������������
	//DBS end---

	//WS
	WS_PLAYER=70,		//	70	��������
	WS_REGION,			//		������������
	WS_W2L,				//		WS��LS��Ϣ��
	WS_L2W,				//		LS��WS��Ϣ��
	WS_W2DB,			//		WS��DBS��Ϣ��
	WS_DB2W,			//	75	DBS��WS��Ϣ��
	WS_W2S,				//		WS��GS��Ϣ��
	WS_S2W,				//		GS��WS��Ϣ��
	WS_RELOADFILE,		//		�ض���Դ�ļ�
	WS_TOPNEWS,			//		�ö�����
	WS_KICK,			//	80	����
	WS_KICK_ALL,		//		��������
	WS_SAVE_ALL,		//      ������Ϸ����
	WS_SAVE_ALL_NOW,	//		�����浱ǰ����
	//WS end---

	//GS
	GS_PALYER=			100,//�����
	GS_TEAM,				//������
	GS_SCRIPTS,		        //102	�ű���
	GS_SERVERID,			//������ID
	GS_S2C,			        //104	GS��GC��Ϣ��
	GS_C2S,					//GC��GS��Ϣ��
	GS_W2S,			        //105	WS��GS��Ϣ��
	GS_S2W,					//GS��WS��Ϣ��
	GS_TEAMDUP,		        //107	���鸱����
	GS_ONEDUP,				//���˸�����
	//GS end---

	//LOGS
	LOGS_CREATETABLE=	140,//������
};