// Fox@20080929----------------------------------------------
// File:    GateMsgType.h
// Brief:   ������ͻ��˼���Ϣ���͵�����, ������Ϣ����һ������
//          ����Ϣ�������Ϊ0x00010000, ��ǰ�����Ϣö��ԶԶ
//			С��0x010000000, Ϊ��֤��������Ϣ���ͼ���, ����Ϣ
//			������Ϊ0x10000000, ��ʽ����.
// Author:  Fox (yulefox@gmail.com)
// Date:    Sep.29, 2008
// Fox@20080929----------------------------------------------

#ifndef _NET_GATE_MSG_TYPE_H_
#define _NET_GATE_MSG_TYPE_H_

enum
{
	GMSG_SUB			= 0x00000100,
	GMSG_SEC			= 0x00010000,
};

enum MsgType
{
	GMSG_GATE_BASE			= 0x10000000,

	// Fox@20081010----------------------------------------------
	// C2G : �ͻ��˷��͵�Gate, Gate������߼�����.
	// Fox@20081010----------------------------------------------
	GMSG_C2G				= GMSG_GATE_BASE + GMSG_SEC * 1,
	GMSG_C2G_CONNECTED,					                    // �ͻ������ӳɹ�

	GMSG_C2G_GTOUP			= GMSG_C2G + GMSG_SUB * 1,		// �ͻ������ӳɹ�

	// Fox@20081010----------------------------------------------
	// C2C : Gateת��, �˴�ö�ٽ�Ϊע�����к�����.
	// Fox@20081010----------------------------------------------
	GMSG_C2C				= GMSG_GATE_BASE + GMSG_SEC * 2,
	GMSG_C2C_TRANSMIT		= GMSG_C2G + GMSG_SUB * 1,		// ��Ϣת��
	GMSG_C2C_TRANSMIT_GROUP,								// ��ϢȺ��
	GMSG_C2C_TRANSMIT_SINGLE,								// ��Ϣ����
};

#endif