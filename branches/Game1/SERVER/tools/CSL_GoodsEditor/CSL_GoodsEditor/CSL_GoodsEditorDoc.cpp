// CSL_GoodsEditorDoc.cpp :  CCSL_GoodsEditorDoc ���ʵ��
//

#include "stdafx.h"
#include "CSL_GoodsEditor.h"

#include "CSL_GoodsEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCSL_GoodsEditorDoc

IMPLEMENT_DYNCREATE(CCSL_GoodsEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CCSL_GoodsEditorDoc, CDocument)
END_MESSAGE_MAP()


// CCSL_GoodsEditorDoc ����/����

CCSL_GoodsEditorDoc::CCSL_GoodsEditorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CCSL_GoodsEditorDoc::~CCSL_GoodsEditorDoc()
{
}

BOOL CCSL_GoodsEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CCSL_GoodsEditorDoc ���л�

void CCSL_GoodsEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CCSL_GoodsEditorDoc ���

#ifdef _DEBUG
void CCSL_GoodsEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCSL_GoodsEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCSL_GoodsEditorDoc ����
