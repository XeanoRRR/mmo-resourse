#pragma once
/*
�����߼��¼��� ������¼���ɼ̳д��࣬����д��������Ӧ���߼�����
*/
enum EVENTTYPE
{
    ON_LOAD   = 0,      //�������
    ON_OPEN   = 1,      //�����
    ON_CLOSE  = 2,      //����ر�
    ON_UPDATE = 3,      //�������
    ON_RELEASE = 4
};
class GamePage;
class PageEvent
{
public:
    PageEvent(){}
    PageEvent(EVENTTYPE type){m_emEventType = type;}
    virtual ~PageEvent(){}

    virtual void OnPageLoad(GamePage *pPage) = 0;
    virtual void OnPageOpen(GamePage *pPage) = 0;
    virtual void OnPageClose(GamePage *pPage) = 0;
    virtual void OnPageUpdate(GamePage *pPage) = 0;
    virtual void OnPageRelease(GamePage *pPage) = 0;
    EVENTTYPE GetEventType() {return m_emEventType;}
    void      SetEventType(EVENTTYPE type) {m_emEventType = type;}
private:
    EVENTTYPE  m_emEventType;
};