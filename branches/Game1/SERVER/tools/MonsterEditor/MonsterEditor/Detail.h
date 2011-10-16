#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDetail �Ի���

class CDetail : public CDialog
{
	DECLARE_DYNAMIC(CDetail)

public:
	//���ÿؼ�ֵ
	void SetData(vector<tagMonster>*vecMonster,vector<tagMonster>::iterator iter);
	//��listcontrol��似�ܼ�¼
	void FillData();
	//�����ļ�
	bool Save();
	//�½���¼ʱ���Ĭ��ֵ
	bool SetNewData(vector<tagMonster>*vecMonster,size_t n);
	//�ж��Ƿ����ظ���¼
	bool IsSimple();
	//��ȡ�ű��ļ�·��
	CString GetScript();
	CDetail(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDetail();

// �Ի�������
	enum { IDD = IDD_DETAIL };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo7();
	afx_msg void OnClose();
	int nIDCLevel;	//�ȼ��༭��
	CListCtrl m_MonSkillList;//�����б�
	DWORD dwIDCExp;	//����༭��
	DWORD dwIDCWorkPoint;//ְҵ����
	DWORD dwIDCMaxHp;//���HP�༭��
	DWORD dwIDCMinAttack;//��С������
	DWORD dwIDCMaxAttack;//��󹥻���
	DWORD dwIDCMinMagicAttack;//��Сħ������
	DWORD dwIDCMaxMagicAttack;//���ħ������
	DWORD dwIDCHitRating;//������
	DWORD dwIDCStormHit;//������
	DWORD dwIDCMagicStorm;//ħ������
	DWORD dwIDCDefense;//������
	DWORD dwIDCMagicDefense;//ħ��������
	DWORD dwIDCBlock;//��
	DWORD dwIDCParry;//�м�
	DWORD dwIDCDodge;//����
	float fIDCMoveSpd;//�ƶ��ٶ�
	float fIDCBatMoveSpd;//ս���ƶ��ٶ�
	CString strIDCMonRace;//��������
	CString strIDCElem;//Ԫ������
	DWORD dwIDCVitality;//����
	DWORD dwIDCHpRecoverSpd;//HP�ָ��ٶ�
	DWORD dwIDCMpRecoverSpd;//MP�ָ��ٶ�


	CString strIDCAIModel;//AIģʽ
	CString strIDCBlock;//�Ƿ��赲
	CString strIDCSuper;//�Ƿ��޵�
	CString strIDCTame;//�ɷ�ѱ��
	CString bIDCTalk;//�ɷ�Ի�
	CString strIDCReturn;//�Ƿ�ɻ�
	CString strIDCAllRecover;//�Ƿ���ȫ�ָ�
	DWORD dwIDCReDistance;//�ɻؾ���
	DWORD dwIDCReTime;//����ʱ��
	double fIDCInjurRate;//���˱���
	DWORD dwIDCAlertTime;//����ʱ��
	double fIDCEscape;//���ܵ���
	DWORD dwIDCSkillRate;//���ܸ���
	DWORD dwIDCSkillInter;//���ܼ��
	DWORD dwIDCTameTimes;//���ѱ������
	DWORD dwIDCMinStayTimes;//��Сͣ��ʱ��
	DWORD dwIDCMaxStayTimes;//���ͣ��ʱ��
	DWORD dwIDCMinWalkTimes;//��С���ߴ���
	DWORD dwIDCMaxWalkTimes;//������ߴ���
	DWORD dwIDCAlertRange;//���䷶Χ
	DWORD dwIDCPurRange;//׷����Χ
	DWORD dwIDCPeaMoveRange;//��ƽ�ƶ���Χ
	DWORD dwIDCStayTime;//ͣ��ʱ��
	CString strIDCAIWay;//AT����ʽ
	CString strIDCNO;//�Զ���
	DWORD dwIDCIndex;//����
	CString strIDCOrigName;//ԭʼ��
	CString strIDCName;//������
	CString strIDCCollection;//�ɼ�����
	CString strIDCTitle;//��ν
	DWORD dwIDCTileColor;//��ν��ɫ
	DWORD dwIDCTitlePicture;//��νͼƬ
	CString strIDCCountryID;//����id
	CString strIDCNpcSort;//NPC���
	DWORD dwIDCPicID;//ͼƬid
	DWORD dwIDCPicLevel;//ͼƬ�ȼ�
	DWORD dwIDCNameColorID;//��ν��ɫid
	DWORD dwIDCLevelColorID;//�ȼ���ɫ

	DWORD dwIDCHpColor;//Ѫ����ɫ
	DWORD dwIDCSoundID;//����id
	DWORD dwIDCHurtHuman;//�����������˺�
	DWORD dwIDCHurtElem;//��Ԫ�������˺�
	DWORD dwIDCHurtDiff;//�����������˺�
	DWORD dwIDCHurtAnimal;//�Զ����˺�
	DWORD dwIDCHurtDead;//�������˺�
	DWORD dwIDCHurtDevil;//��ħ���˺�
	DWORD dwIDCHurtInsect;//�������˺�
	DWORD dwIDCStormHurt;//�������˺�
	DWORD dwIDCMagicStormHurt;//ħ�������˺�
	DWORD dwIDCHurtFire;//�Ի�ϵ�����˺�
	DWORD dwIDCHurtWater;//��ˮϵ�����˺�
	DWORD dwIDCHurtGround;//����ϵ�����˺�
	DWORD dwIDCHurtWind;//�Է�ϵ�����˺�
	DWORD dwIDCHurtSacre;//����ʥϵ�����˺�
	DWORD dwIDCHurtDark;//�Ժڰ�ϵ�����˺�
	DWORD dwIDCHurtNotElem;//����Ԫ��ϵ�����˺�
	DWORD dwIDCGroundAnti;//��Ԫ�ؿ���
	DWORD dwIDCWaterAnti;//ˮԪ�ؿ���
	DWORD dwIDCFireAnti;//��Ԫ�ؿ���
	DWORD dwIDCWindAnti;//ˮԪ�ؿ���
	DWORD dwIDCSacreAnti;//��ʥ����
	DWORD dwIDCDarkAnti;//�ڰ�����
	DWORD dwIDCNotElemAnti;//��Ԫ�ؿ���
	DWORD dwIDCStormAnti;//����������
	DWORD dwIDCMagicStormAnti;//ħ����������
	DWORD dwIDCWillAnti;//��־����
	DWORD dwIDCBodyAnti;//���ʿ���
	DWORD dwIDCBloodAnti;//��Ѫ����
	DWORD dwIDCDuskAnti;//���ο���
	DWORD dwIDCNumbAnti;//��Կ���
	DWORD dwIDCSleepAnti;//˯�߿���
	DWORD dwIDCDrugAnti;//�ж�����
	DWORD dwIDCNotMoveAnti;//������
	CString strIDCRefe1;//ai�ο�1
	CString strIDCRefe2;//ai�ο�2
	CString strIDCRefe3;
	CString strIDCRefe4;
	CString strIDCRefe5;
	CString strIDCBirthSct;//�����ű�
	CString strIDCBattleSct;//ս���ű�
	CString strIDCInjurSct;//���˽ű�
	CString strIDCDeadSct;//�����ű�
	CString strIDCCycleSct;//���ڽű�
	CString strIDCChoseSkillSct;//ѡ���ܽű�
	CString strIDCFindTgtSct;//Ѱ��Ŀ��ű�
	CString strIDCKillTgtSct;//ɱ��Ŀ��ű�
	CString strIDCShape;//����
	//��Ӽ���
	afx_msg void OnAppendSkill();

	afx_msg void OnEnChangeEdit26();
	// ��������
	CString strIDCSkillType;
	// ����ID
	DWORD dwIDCSkillID;
	// ���ܵȼ�
	DWORD dwIDCSkillLevel;
	// ���ܼ�����������ܡ���ͨ����û�У���Ϊ0
	DWORD dwIDCInterTime;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_combox;
	afx_msg void OnUpdateSkill();
	afx_msg void OnDeleteSkill();
	//��ӽű��İ�ť
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
};
