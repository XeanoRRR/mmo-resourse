#pragma once
#include "../../PageEvent.h"
class CPlayer;
class C3DIconEffect;
class SREvent:public PageEvent
{
public:
    SREvent();
    SREvent(EVENTTYPE type);
    ~SREvent(){}
    virtual void OnPageLoad(GamePage *pPage);
    virtual void OnPageOpen(GamePage *pPage);
    virtual void OnPageClose(GamePage *pPage);
    virtual void OnPageUpdate(GamePage *pPage);
    virtual void OnPageRelease(GamePage *pPage);
    //��������ɫ��ť
    bool OnCreateRoleBtn(const CEGUI::EventArgs &e);

    //ɾ����ɫ��ť����
    bool OnDeleteRoleBtn(const CEGUI::EventArgs &e);

    //������ϷGameserver
    bool OnEnterGameBtn(const CEGUI::EventArgs &e);

    //�ص���ҳ����
    bool GoBackBtn(const CEGUI::EventArgs &e);

    //ģ������ת
    bool TurnLeftBtn(const CEGUI::EventArgs &e);

    //ģ������ת
    bool TurnRightBtn(const CEGUI::EventArgs &e);

    //ֹͣ��ת
    bool StopRotation(const CEGUI::EventArgs &e);

	bool UpdateDelRemainDays(const CEGUI::EventArgs& e);

    void AddPlayer(CPlayer *pPlayer);

    void ClearPlayer();

    size_t GetPlayerCount()   {return m_vecPlayer.size();}

    CPlayer *GetPlayer(ulong index);

    CPlayer *GetSelPlayer();
    
    ulong GetSelectedIndex() {return m_SelectIndex;}
private:

    //��ǰ�˺�����һ�����Ϣ����
    static vector<CPlayer*>     m_vecPlayer;

    //��ǰѡ��Ľ�ɫ
    static ulong                m_SelectIndex;

public:
    // ��ʼ��Ʒ��װ���б�
    struct tagOrginEquip
    {
        BYTE	lOccupation;			// ְҵ
        WORD	wPlacePos;				// ����λ��
        string	strOrginName;			// ��Ʒԭʼ��
    };
   

    list<tagOrginEquip>     m_listOrginEquip;   // ��ʼװ���б�
};
