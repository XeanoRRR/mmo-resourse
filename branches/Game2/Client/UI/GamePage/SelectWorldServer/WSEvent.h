#pragma once
#include "../../PageEvent.h"

class WSEvent:public PageEvent
{
public:
    WSEvent(){}

    WSEvent(EVENTTYPE type);

    ~WSEvent(){}

    virtual void OnPageLoad(GamePage *pPage);
    virtual void OnPageOpen(GamePage *pPage);
    virtual void OnPageClose(GamePage *pPage);
    virtual void OnPageUpdate(GamePage *pPage);
    virtual void OnPageRelease(GamePage *pPage);
    /*
    brief: ���worldserver��
    */
    void ClearWS(){m_WorldInfos.clear();}
    /*
    brief: ��worldserver�������ӷ�������Ϣ
    */
    void AddWorldInfo(long lID,char* strWorldName,long lState);
    /*
    brief: �ص���½���水ť�Ĵ���
    */
    bool OnBackToLoginBtn(const CEGUI::EventArgs &e);
    /*
    brief: ������Ϸ��ť�Ĵ���
    */
    bool OnEnterBtnDown(const CEGUI::EventArgs &e);
private:
    struct tagWorldInfo
    {
        string       strWSName;    //�����������
        long         lState;       //������״̬
    };
    static map<long,tagWorldInfo>		m_WorldInfos;				//��Ϸ��������Ϣ
    typedef map<long,tagWorldInfo>::iterator    wsIter;

    /*
    brief: �����������״̬��ͼ������
    */
    static map<long,string>    WS_stateToImageSetName;

    void LoadServerImageSet();
};