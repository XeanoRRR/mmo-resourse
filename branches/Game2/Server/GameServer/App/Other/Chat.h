#pragma once

// ����
class CChat
{
public:

	enum eChatType
	{
		CHAT_NORMAL = 0,		// ͬ������
		CHAT_REGION,			// ͬ��������
		CHAT_FACTION,			// �������
		CHAT_UNION,				// ��������
		CHAT_TEAM,				// ��������
		CHAT_PRIVATE,			// ����˽��
		CHAT_COUNTRY,			// ��������
		CHAT_WOLRD,				// ��������

		CHAT_BOARDCAST,			// �㲥

		GM_CMD,					// GM����
		RUN_SCRIPT,				// ���нű�
		RUN_HELP,				// ����

		TYPE_NUM,
	};

public:
	CChat(void);
	virtual ~CChat(void);
};
