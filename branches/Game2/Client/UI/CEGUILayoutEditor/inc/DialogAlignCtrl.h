//���ܣ���ѡ������������У��м��ĶԻ���
//#include "../../../wxWidgets/include/wx/dialog.h"
//2010.9.13 nikai
#pragma once
#include "EditorDocument.h"
class DialogAlignCtrl:public wxDialog
{
public:
    DialogAlignCtrl(wxWindow *pParent);
    ~DialogAlignCtrl();

private:
    enum 
    {
        ID_RADIOBUTTON,
        ID_TXT_INPUT,
        ID_CONFIRM_BUTTON,
        ID_CANCEL_BUTTON
    };
    wxRadioBox*		m_RadioAlign;
    wxTextCtrl*     m_TxtInput;
    wxButton*		m_BtnConfirm;
    wxButton*		m_BtnCancel;
    DECLARE_EVENT_TABLE();
    void OnConfirm(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnTextChanged(wxCommandEvent& event);
    void OnSelectStyle(wxCommandEvent& event);
    //���뷽ʽ
    long            m_paddingStyle;
    //���ֵ
    long            m_paddingValue;
    EditorDocument *m_document;
public:
    void SetDocument (EditorDocument* document) {m_document = document;}
};