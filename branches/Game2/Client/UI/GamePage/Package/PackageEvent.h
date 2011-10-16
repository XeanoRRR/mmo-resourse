#pragma once

#include "../../PageEvent.h"
#include "../../../../Public/Module/GUID.h"
#include "../../../GameClient/ClientApp/Player.h"

#define GRID_H_ITEM 5							//����5�����߸���
#define GRID_V_ITEM 4							//����4�����߸���
#define GRIDITEM	GRID_H_ITEM*GRID_V_ITEM		//�������ĸ��ӵ�����
#define GRIDPACK	5							//�ӱ���������
#define GRID_V_SUBPACK	6						//�ӱ����������

#define GRID_H_INTEG 8							//���ϱ�������ĺ��Ÿ�����
#define GRID_V_INTEGMAXLINE	15					//���ϱ������������
#define GRIDINTEGMAXNUM  GRID_H_INTEG*GRID_V_INTEGMAXLINE	//���ϱ�������������
namespace CEGUI
{
    class DragContainer;
}

class PackageEvent:public PageEvent
{
public:
    PackageEvent(){}
    PackageEvent(EVENTTYPE type);
    ~PackageEvent();
    virtual void OnPageLoad(GamePage *pPage);
    virtual void OnPageOpen(GamePage *pPage);
    virtual void OnPageClose(GamePage *pPage);
    virtual void OnPageUpdate(GamePage *pPage);
    virtual void OnPageRelease(GamePage *pPage);

    void SetUpdatePos(int lPos){m_UpdateGoodsPos = lPos;}

    void SendChangeEquipMSG(int nIndex,int nSPos,int nExtendID,int nSExtendID);
    
    int  GetGuidMouseHover() {return m_UpdateGoodsPos;}
private:
    /*
    ��ǰ��Ҫ���µ���Ʒλ��
    */
    int m_UpdateGoodsPos;

private:
    void SendPutDownMes(long SorceType/*Դ����*/,const CGUID& SorceGUID/*Դ�����GUID*/,long lSorceObjectExtendID,long lSorceObjectPos,
        long DesType,const CGUID& DesGUID,long lDesObjectExtendID,long lDesObjectPos,long lObjectType,const CGUID& ObjectGUID,long lObjectNum);

    //�����Ҽ����
    bool ProcessMouseRBtnDown(const CEGUI::EventArgs &args);

    //�õ���ǰ�����ĵĸ�������
    long GetCurGridPos(CEGUI::DragContainer *pDrag);

	/// �Ҽ���Ӧ��Ʒ����
	bool OnRBClickGoodsForOtherUIPages(CPlayer::tagGoods* pstGoods);

    bool handle_GoodsDropped(const CEGUI::EventArgs& args); //������Ʒ

    bool handle_GoodsPickUp(const CEGUI::EventArgs& args);  //ʰȡ��Ʒ

    bool handle_PageClose(const CEGUI::EventArgs& args);    //�رս���

	bool handle_OpenPlayerShop(const CEGUI::EventArgs& args);	// �򿪸����̵�

    bool updateMoney(const CEGUI::EventArgs & args);        //ˢ�½�Ǯ

    bool OnMouseEnter(const CEGUI::EventArgs & args);       //����ƶ�����������UI��

    bool OnMouseLeave(const CEGUI::EventArgs & args);       //����ƶ�����������UI��
};

