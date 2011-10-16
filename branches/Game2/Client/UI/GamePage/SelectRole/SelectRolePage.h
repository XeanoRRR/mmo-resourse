#pragma once
#include "../../GamePage.h"

class GameScene;
class CPlayer;
class SREvent;
class SelectRolePage:public GamePage
{
public:
    SelectRolePage();

    virtual ~SelectRolePage();

    virtual bool Open();

    virtual bool Close();

    virtual void ReleasePage();

    virtual void LoadPage();

    void Display();

    void AddPlayer(CPlayer* pPlayer);

    void ClearPlayer();

    ulong GetPlayerCount();

	CEGUI::Window* GetWindow(const char* WindwoName);

    CPlayer * GetSelPlayer();

	CPlayer * GetPlayer(ulong index);

    bool          m_bRoleLeftRotate;    //������ת
    bool          m_bRoleRightRotate;   //������ת
private:

    GameScene   * m_SelectSence;

    static float  s_RolePos[4];		// վ��λ��
    static int    m_CurrentAction;		// ��ǰ����
    static int    m_NextAction;           // ��һ������

	SREvent* m_event;
};