// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// RrMMOLauncherServerDoc.h : CRrMMOLauncherServerDoc ��Ľӿ�
//


#pragma once


class CRrMMOLauncherServerDoc : public CDocument
{
protected: // �������л�����
    CRrMMOLauncherServerDoc();
    DECLARE_DYNCREATE(CRrMMOLauncherServerDoc)

// ����
public:

// ����
public:

// ��д
public:
    virtual BOOL OnNewDocument();
    virtual VOID Serialize(CArchive& ar);

// ʵ��
public:
    virtual ~CRrMMOLauncherServerDoc();
#ifdef _DEBUG
    virtual VOID AssertValid() CONST;
    virtual VOID Dump(CDumpContext& dc) CONST;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
    DECLARE_MESSAGE_MAP()

    // ���ɵ� OLE ����ӳ�亯��

    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()
};


