#pragma once


// CStickQuest �Ի���

class CStickQuest : public CDialog
{
	DECLARE_DYNAMIC(CStickQuest)

public:
	CStickQuest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStickQuest();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    long m_QuestID;
};
