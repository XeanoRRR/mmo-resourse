#include "StdAfx.h"
#include "Goodslist.h"
#include "Goods.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hWnd;

map<ulong, CGoodsList::tagGoods2>		CGoodsList::m_mapGoodsList;			// ԭʼ��Ʒ�б�(wIndew, tagGoods)
map<string, CGoodsList::tagGoods2*>		CGoodsList::m_mapGoodsListByName;	// ������Ϊ������ԭʼ��Ʒ�б�(strName, *tagGoods)
map<ulong,string>									CGoodsList::m_mapSuitName;			// ����װID��Ӧ���������װ������
map<ulong/*��װID*/, CGoodsList::tagSuitAttribute>	CGoodsList::m_SuitList;				// ��װ�����Ա������������װ�ڸ��ּ��������¼������ܵ�����
//map<string,ulong>									CGoodsList::m_mapEffects;			// ӳ����Ʒ���ƺ͸�������
map<string,ulong>									CGoodsList::m_mapAllEffects;		// ӳ�丽���������ƺ͸�������ö��ֵ
map<ulong,string>									CGoodsList::m_mapEffectReverse;		// ӳ����Ʒ�������Ժ�����
map<ulong/*�׿���ID��*/,CGoodsList::tagSeriesCardProperty>	CGoodsList::m_mapSeriesCardList;	// �׿������б�
vector<CGoodsList::tagMoneyGraphics> CGoodsList::m_stMoneyGraphics;
vector<CGoodsList::tagMoneyGraphics> CGoodsList::m_stSilverMoneyGraphics;

CGoodsList::CGoodsList(void)
{
}

CGoodsList::~CGoodsList(void)
{
	m_mapGoodsList.clear();
	m_mapGoodsListByName.clear();
	m_mapAllEffects.clear();
	m_mapEffectReverse.clear();
	m_SuitList.clear();
	m_mapSeriesCardList.clear();
}

void CGoodsList::InitMapEffects()
{
	if (m_mapEffectReverse.empty())
	{
		//��ת������ӳ��Map
        m_mapEffectReverse[GAP_MAXHP_CORRECT] = AppFrame::GetText("Player_23");//��������
        m_mapEffectReverse[GAP_MAXMP_CORRECT]= AppFrame::GetText("Player_24");//�������� 
        m_mapEffectReverse[GAP_HP_RESUME_CORRECT] = AppFrame::GetText("Player_25");//�����ָ��ٶ�
        m_mapEffectReverse[GAP_MP_RESUME_CORRECT] = AppFrame::GetText("Player_26");//�����ָ��ٶ�
        m_mapEffectReverse[GAP_ADD_STR] = AppFrame::GetText("Player_27");//����
        m_mapEffectReverse[GAP_ADD_DEX] = AppFrame::GetText("Player_28");//����
        m_mapEffectReverse[GAP_ADD_CON] = AppFrame::GetText("Player_29");//����    
        m_mapEffectReverse[GAP_ADD_INT] = AppFrame::GetText("Player_30");//����
        m_mapEffectReverse[GAP_ADD_WIS] = AppFrame::GetText("Player_31");//�ǻ� 
        m_mapEffectReverse[GAP_ADD_SPI] = AppFrame::GetText("Player_32");//����
        m_mapEffectReverse[GAP_PARRY_CORRECT] = AppFrame::GetText("Player_33");//�� 
        m_mapEffectReverse[GAP_BLOCK_CORRECT] = AppFrame::GetText("Player_34");//�м� 
        m_mapEffectReverse[GAP_FLEE_CORRECT] = AppFrame::GetText("Player_35");//����  
        m_mapEffectReverse[GAP_HIT_RATE_CORRECT] = AppFrame::GetText("Player_36");//����
        m_mapEffectReverse[GAP_DEDICANT_CORRECT] = AppFrame::GetText("Player_37");//רע
        m_mapEffectReverse[GAP_NOR_DEF] = AppFrame::GetText("Player_38");//�������
        m_mapEffectReverse[GAP_MAGIC_DEF] = AppFrame::GetText("Player_39");//ħ������
        m_mapEffectReverse[GAP_NORATTACK_KILLRATE] = AppFrame::GetText("Player_40");//������
        m_mapEffectReverse[GAP_MAGIC_SPEED] = AppFrame::GetText("Player_41");//ʩ���ٶ�
        m_mapEffectReverse[GAP_MIN_ATTACK] = AppFrame::GetText("Player_42");//��С������
        m_mapEffectReverse[GAP_MAX_ATTACK] = AppFrame::GetText("Player_43");//��󹥻���
        m_mapEffectReverse[GAP_FUMO_PROPERTY] = AppFrame::GetText("Player_44");//ħ��������
        m_mapEffectReverse[GAP_MAGIC_KILLRATE] = AppFrame::GetText("Player_45");//ħ��������
        m_mapEffectReverse[GAP_LEECH_HP] = AppFrame::GetText("Player_46");//����������
        m_mapEffectReverse[GAP_LEECH_MP] = AppFrame::GetText("Player_47");//����������
        m_mapEffectReverse[GAP_WILL_DEF] = AppFrame::GetText("Player_48");//��־����
        m_mapEffectReverse[GAP_CON_DEF] = AppFrame::GetText("Player_49");//���ʿ���
        m_mapEffectReverse[GAP_BLED_DEF] = AppFrame::GetText("Player_50");//��Ѫ����
        m_mapEffectReverse[GAP_DIZZY_DEF] = AppFrame::GetText("Player_51");//���迹��
        m_mapEffectReverse[GAP_LULL_DEF] = AppFrame::GetText("Player_52");//��Կ���
        m_mapEffectReverse[GAP_HYPNOSIS_DEF] = AppFrame::GetText("Player_53");//���߿���
        m_mapEffectReverse[GAP_POISON_DEF] = AppFrame::GetText("Player_54");//�ж�����
        m_mapEffectReverse[GAP_FIX_DEF] = AppFrame::GetText("Player_55");//������
        m_mapEffectReverse[GAP_FIREDEF_CORRECT] = AppFrame::GetText("Player_56");//��
        m_mapEffectReverse[GAP_WATERDEF_CORRECT] = AppFrame::GetText("Player_57");//ˮ��
        m_mapEffectReverse[GAP_EARTHDEF_CORRECT] = AppFrame::GetText("Player_58");//�ؿ�
        m_mapEffectReverse[GAP_WINDDEF_CORRECT] = AppFrame::GetText("Player_59");//�翹
        m_mapEffectReverse[GAP_DARKDEF_CORRECT] = AppFrame::GetText("Player_60");//�ڰ���
        m_mapEffectReverse[GAP_HOLYDEF_CORRECT] = AppFrame::GetText("Player_61");//��ʥ��
        m_mapEffectReverse[GAP_HURT_SUCK_PER] = AppFrame::GetText("Player_62");//�˺����ձ���
        m_mapEffectReverse[GAP_WEAPON_DAMAGE_LEVEL] = AppFrame::GetText("Player_101");//����ǿ��
        m_mapEffectReverse[GAP_LEECH_MP] = AppFrame::GetText("Player_102");//����Mp����
        m_mapEffectReverse[GAP_HURT2MP] = AppFrame::GetText("Player_103");//����תMp����
        m_mapEffectReverse[GAP_CRI_DAMAGE] = AppFrame::GetText("Player_104");//�������˺�
        m_mapEffectReverse[GAP_CRI_DEF] = AppFrame::GetText("Player_105");//����������
        m_mapEffectReverse[GAP_MCRI_DAMAGE] = AppFrame::GetText("Player_106");//ħ�������˺�
        m_mapEffectReverse[GAP_MCRI_DEF] = AppFrame::GetText("Player_107");//ħ����������
        m_mapEffectReverse[GAP_MAX_ENERGY_COR] = AppFrame::GetText("Player_108");//������������
        m_mapEffectReverse[GAP_ENERGY_RESUME_COR] = AppFrame::GetText("Player_109");//�����ָ��ٶ�����
        m_mapEffectReverse[GAP_ENERGY_SUCK] = AppFrame::GetText("Player_110");//��������
        m_mapEffectReverse[GAP_PEOPLE_HURT_RATIO] = AppFrame::GetText("Player_111");//������������˺��ӳ�
        m_mapEffectReverse[GAP_ELEMENT_HURT_RATIO] = AppFrame::GetText("Player_112");//��Ԫ��������˺��ӳ�
        m_mapEffectReverse[GAP_DIFFRACE_HURT_RATIO] = AppFrame::GetText("Player_113");//������������˺��ӳ�
        m_mapEffectReverse[GAP_ANIM_HURT_RATIO] = AppFrame::GetText("Player_114");//�Զ���������˺��ӳ�
        m_mapEffectReverse[GAP_SOUL_HURT_RATIO] = AppFrame::GetText("Player_115");//������������˺��ӳ�
        m_mapEffectReverse[GAP_DEMON_HURT_RATIO] = AppFrame::GetText("Player_116");//�Զ�ħ������˺��ӳ�
        m_mapEffectReverse[GAP_INSECT_HURT_RATIO] = AppFrame::GetText("Player_117");//������������˺��ӳ�
        m_mapEffectReverse[GAP_FIRE_HURT_RATIO] = AppFrame::GetText("Player_118");//�Ի�ϵ�����˺��ӳ�
        m_mapEffectReverse[GAP_WATER_HURT_RATIO] = AppFrame::GetText("Player_119");//��ˮϵ�����˺��ӳ�
        m_mapEffectReverse[GAP_EARTH_HURT_RATIO] = AppFrame::GetText("Player_120");//����ϵ�����˺��ӳ�
        m_mapEffectReverse[GAP_WIND_HURT_RATIO] = AppFrame::GetText("Player_121");//�Է�ϵ�����˺��ӳ�
        m_mapEffectReverse[GAP_DARK_HURT_RATIO] = AppFrame::GetText("Player_122");//�԰�ϵ�����˺��ӳ�
        m_mapEffectReverse[GAP_HOLINESS_HURT_RATIO] = AppFrame::GetText("Player_123");//��ʥϵ�����˺��ӳ�
        m_mapEffectReverse[GAP_HURT_RID] = AppFrame::GetText("Player_124");//����������
        m_mapEffectReverse[GAP_MAGIC_HURT_RID] = AppFrame::GetText("Player_125");//����ħ������
        m_mapEffectReverse[GAP_SHIELD_DEF] = AppFrame::GetText("Player_126");//�����������
        m_mapEffectReverse[GAP_TIME_LIMIT] = AppFrame::GetText("Player_127");//��ʱ
        m_mapEffectReverse[GAP_ADDON_CHARM] = AppFrame::GetText("Player_128");//����
        m_mapEffectReverse[GAP_HP_RESUME_RATE] = AppFrame::GetText("Player_129");//����ֵ�ָ�����
        m_mapEffectReverse[GAP_BUFF_ATK_STRENGTH] = AppFrame::GetText("Player_130");//״̬����ǿ��
        m_mapEffectReverse[GAP_PENETRATE] = AppFrame::GetText("Player_214");//������͸
        m_mapEffectReverse[GAP_EXCUSE_HURT] = AppFrame::GetText("Player_215");//�˺�����
        m_mapEffectReverse[GAP_LUCKY] = AppFrame::GetText("Player_223");//����
        m_mapEffectReverse[GAP_ADDON_PENETRATE] = AppFrame::GetText("Player_214");//���Ӵ�͸����
        m_mapEffectReverse[GAP_ADDON_EXCUSE_HURT] = AppFrame::GetText("Player_215");//�����˺�����
        m_mapEffectReverse[GAP_ADDON_LUCKY] = AppFrame::GetText("Player_223");//��������
        m_mapEffectReverse[GAP_CUR_ATK_ED] = AppFrame::GetText("Player_221");//ǿ������
        m_mapEffectReverse[GAP_CUR_DEF_ED] = AppFrame::GetText("Player_222");//ǿ������
        m_mapEffectReverse[GAP_ADDON_ATK_ED] = AppFrame::GetText("Player_221");//����ǿ������
        m_mapEffectReverse[GAP_ADDON_DEF_ED] = AppFrame::GetText("Player_222");//����ǿ������
        m_mapEffectReverse[GAP_MIN_ATK_ADDON] = AppFrame::GetText("Player_224");//С��׷��
        m_mapEffectReverse[GAP_MAX_ATK_ADDON] = AppFrame::GetText("Player_225");//��׷��
        m_mapEffectReverse[GAP_MAGIC_ATK_ADDON] = AppFrame::GetText("Player_226");//ħ��׷��
        m_mapEffectReverse[GAP_DEF_ADDON] = AppFrame::GetText("Player_227");		//���׷��
        m_mapEffectReverse[GAP_MAGIC_DEF_ADDON] = AppFrame::GetText("Player_228");//ħ��׷��
	}
	if (m_mapAllEffects.empty())
	{
		m_mapAllEffects["GAP_ADD_STATE"]=GAP_ADD_STATE;//������״̬
		m_mapAllEffects["GAP_ADD_SKILL"]=GAP_ADD_SKILL;//���Ӽ���
		m_mapAllEffects["GAP_BUFF_ATK_STRENGTH"]=GAP_BUFF_ATK_STRENGTH;//״̬����ǿ��
		m_mapAllEffects["GAP_HP_RESUME_RATE"]=GAP_HP_RESUME_RATE;//����ֵ�ָ�����
		m_mapAllEffects["GAP_ADDON_CHARM"]=GAP_ADDON_CHARM;//����
		m_mapAllEffects["GAP_TIME_LIMIT"]=GAP_TIME_LIMIT;//��ʱ
		m_mapAllEffects["GAP_SHIELD_DEF"]=GAP_SHIELD_DEF;//�����������
		m_mapAllEffects["GAP_MAGIC_HURT_RID"]=GAP_MAGIC_HURT_RID;//����ħ������
		m_mapAllEffects["GAP_HURT_RID"]=GAP_HURT_RID;//����������
		m_mapAllEffects["GAP_HOLINESS_HURT_RATIO"]=GAP_HOLINESS_HURT_RATIO;//��ʥϵ�����˺��ӳ�
		m_mapAllEffects["GAP_DARK_HURT_RATIO"]=GAP_DARK_HURT_RATIO;//�԰�ϵ�����˺��ӳ�
		m_mapAllEffects["GAP_WIND_HURT_RATIO"]=GAP_WIND_HURT_RATIO;//�Է�ϵ�����˺��ӳ�
		m_mapAllEffects["GAP_EARTH_HURT_RATIO"]=GAP_EARTH_HURT_RATIO;//����ϵ�����˺��ӳ�
		m_mapAllEffects["GAP_WATER_HURT_RATIO"]=GAP_WATER_HURT_RATIO;//��ˮϵ�����˺��ӳ�
		m_mapAllEffects["GAP_FIRE_HURT_RATIO"]=GAP_FIRE_HURT_RATIO;//�Ի�ϵ�����˺��ӳ�
		m_mapAllEffects["GAP_INSECT_HURT_RATIO"]=GAP_INSECT_HURT_RATIO;//������������˺��ӳ�
		m_mapAllEffects["GAP_DEMON_HURT_RATIO"]=GAP_DEMON_HURT_RATIO;//�Զ�ħ������˺��ӳ�
		m_mapAllEffects["GAP_SOUL_HURT_RATIO"]=GAP_SOUL_HURT_RATIO;//������������˺��ӳ�
		m_mapAllEffects["GAP_ANIM_HURT_RATIO"]=GAP_ANIM_HURT_RATIO;//�Զ���������˺��ӳ�
		m_mapAllEffects["GAP_DIFFRACE_HURT_RATIO"]=GAP_DIFFRACE_HURT_RATIO;//������������˺��ӳ�
		m_mapAllEffects["GAP_ELEMENT_HURT_RATIO"]=GAP_ELEMENT_HURT_RATIO;//��Ԫ��������˺��ӳ�
		m_mapAllEffects["GAP_PEOPLE_HURT_RATIO"]=GAP_PEOPLE_HURT_RATIO;//������������˺��ӳ�
		m_mapAllEffects["GAP_ENERGY_SUCK"]=GAP_ENERGY_SUCK;//��������
		m_mapAllEffects["GAP_ENERGY_RESUME_COR"]=GAP_ENERGY_RESUME_COR;//�����ָ��ٶ�����
		m_mapAllEffects["GAP_MAX_ENERGY_COR"]=GAP_MAX_ENERGY_COR;//������������
		m_mapAllEffects["GAP_MCRI_DEF"]=GAP_MCRI_DEF;//ħ����������
		m_mapAllEffects["GAP_MCRI_DAMAGE"]=GAP_MCRI_DAMAGE;//ħ�������˺�
		m_mapAllEffects["GAP_CRI_DEF"]=GAP_CRI_DEF;//����������
		m_mapAllEffects["GAP_CRI_DAMAGE"]=GAP_CRI_DAMAGE;//�������˺� 
		m_mapAllEffects["GAP_HURT2MP"]=GAP_HURT2MP;//����תMp���� 
		m_mapAllEffects["GAP_LEECH_MP"]=GAP_LEECH_MP;//����Mp���� 
		m_mapAllEffects["GAP_HURT_SUCK_PER"]=GAP_HURT_SUCK_PER;//�˺����ձ��� 
		m_mapAllEffects["GAP_WEAPON_DAMAGE_LEVEL"]=GAP_WEAPON_DAMAGE_LEVEL;//����ǿ��   
		m_mapAllEffects["GAP_MAXHP_CORRECT"]=GAP_MAXHP_CORRECT;//��������             
		m_mapAllEffects["GAP_MAXMP_CORRECT"]=GAP_MAXMP_CORRECT;//��������                     
		m_mapAllEffects["GAP_HP_RESUME_CORRECT"]=GAP_HP_RESUME_CORRECT;//�����ָ��ٶ�     
		m_mapAllEffects["GAP_MP_RESUME_CORRECT"]=GAP_MP_RESUME_CORRECT;//�����ָ��ٶ�                     
		m_mapAllEffects["GAP_ADD_STR"]=GAP_ADD_STR;//����         
		m_mapAllEffects["GAP_ADD_DEX"]=GAP_ADD_DEX;//����          
		m_mapAllEffects["GAP_ADD_CON"]=GAP_ADD_CON;//����       
		m_mapAllEffects["GAP_ADD_INT"]=GAP_ADD_INT;//����
		m_mapAllEffects["GAP_ADD_WIS"]=GAP_ADD_WIS;//�ǻ�
		m_mapAllEffects["GAP_ADD_SPI"]=GAP_ADD_SPI;//����         
		m_mapAllEffects["GAP_PARRY_CORRECT"]=GAP_PARRY_CORRECT;//��            
		m_mapAllEffects["GAP_BLOCK_CORRECT"]=GAP_BLOCK_CORRECT;//�м�            
		m_mapAllEffects["GAP_FLEE_CORRECT"]=GAP_FLEE_CORRECT;//����           
		m_mapAllEffects["GAP_HIT_RATE_CORRECT"]=GAP_HIT_RATE_CORRECT;//����
		m_mapAllEffects["GAP_DEDICANT_CORRECT"]=GAP_DEDICANT_CORRECT;//רע            
		m_mapAllEffects["GAP_NOR_DEF"]=GAP_NOR_DEF;//�������  
		m_mapAllEffects["GAP_MAGIC_DEF"]=GAP_MAGIC_DEF;//ħ������            
		m_mapAllEffects["GAP_NORATTACK_KILLRATE"]=GAP_NORATTACK_KILLRATE;//������         
		m_mapAllEffects["GAP_MAGIC_SPEED"]=GAP_MAGIC_SPEED;//ʩ���ٶ�          
		m_mapAllEffects["GAP_MIN_ATTACK"]=GAP_MIN_ATTACK;//��С������
		m_mapAllEffects["GAP_MAX_ATTACK"]=GAP_MAX_ATTACK;//��󹥻���
		m_mapAllEffects["GAP_FUMO_PROPERTY"]=GAP_FUMO_PROPERTY;//ħ��������            
		m_mapAllEffects["GAP_MAGIC_KILLRATE"]=GAP_MAGIC_KILLRATE;//ħ��������          
		m_mapAllEffects["GAP_LEECH_HP"]=GAP_LEECH_HP;//����������            
		m_mapAllEffects["GAP_LEECH_MP"]=GAP_LEECH_MP;//����������      
		m_mapAllEffects["GAP_WILL_DEF"]=GAP_WILL_DEF;//��־����     
		m_mapAllEffects["GAP_CON_DEF"]=GAP_CON_DEF;//���ʿ���
		m_mapAllEffects["GAP_BLED_DEF"]=GAP_BLED_DEF;//��Ѫ����
		m_mapAllEffects["GAP_DIZZY_DEF"]=GAP_DIZZY_DEF;//���迹��
		m_mapAllEffects["GAP_LULL_DEF"]=GAP_LULL_DEF;//��Կ���
		m_mapAllEffects["GAP_HYPNOSIS_DEF"]=GAP_HYPNOSIS_DEF;//���߿���
		m_mapAllEffects["GAP_POISON_DEF"]=GAP_POISON_DEF;//�ж�����
		m_mapAllEffects["GAP_FIX_DEF"]=GAP_FIX_DEF;//������
		m_mapAllEffects["GAP_FIREDEF_CORRECT"]=GAP_FIREDEF_CORRECT;//��        
		m_mapAllEffects["GAP_WATERDEF_CORRECT"]=GAP_WATERDEF_CORRECT;//ˮ��
		m_mapAllEffects["GAP_EARTHDEF_CORRECT"]=GAP_EARTHDEF_CORRECT;//�ؿ� 
		m_mapAllEffects["GAP_WINDDEF_CORRECT"]=GAP_WINDDEF_CORRECT;//�翹
		m_mapAllEffects["GAP_DARKDEF_CORRECT"]=GAP_DARKDEF_CORRECT;//�ڰ���
		m_mapAllEffects["GAP_HOLYDEF_CORRECT"]=GAP_HOLYDEF_CORRECT;//��ʥ��
		m_mapAllEffects["GAP_HURT_SUCK_PER"] = GAP_HURT_SUCK_PER;//�˺����ձ���
		m_mapAllEffects["GAP_WEAPON_DAMAGE_LEVEL"] = GAP_WEAPON_DAMAGE_LEVEL;//����ǿ��
		m_mapAllEffects["GAP_LEECH_MP"] = GAP_LEECH_MP;//����Mp����
		m_mapAllEffects["GAP_HURT2MP"] = GAP_HURT2MP;//����תMp����
		m_mapAllEffects["GAP_CRI_DAMAGE"] = GAP_CRI_DAMAGE;//�������˺�
		m_mapAllEffects["GAP_CRI_DEF"] = GAP_CRI_DEF;//����������
		m_mapAllEffects["GAP_MCRI_DAMAGE"] = GAP_MCRI_DAMAGE;//ħ�������˺�
		m_mapAllEffects["GAP_MCRI_DEF"] = GAP_MCRI_DEF;//ħ����������
		m_mapAllEffects["GAP_MAX_ENERGY_COR"] = GAP_MAX_ENERGY_COR;//������������
		m_mapAllEffects["GAP_ENERGY_RESUME_COR"] = GAP_ENERGY_RESUME_COR;//�����ָ��ٶ�����
		m_mapAllEffects["GAP_ENERGY_SUCK"] = GAP_ENERGY_SUCK;//��������
		m_mapAllEffects["GAP_PEOPLE_HURT_RATIO"] = GAP_PEOPLE_HURT_RATIO;//������������˺��ӳ�
		m_mapAllEffects["GAP_ELEMENT_HURT_RATIO"] = GAP_ELEMENT_HURT_RATIO;//��Ԫ��������˺��ӳ�
		m_mapAllEffects["GAP_DIFFRACE_HURT_RATIO"] = GAP_DIFFRACE_HURT_RATIO;//������������˺��ӳ�
		m_mapAllEffects["GAP_ANIM_HURT_RATIO"] = GAP_ANIM_HURT_RATIO;//�Զ���������˺��ӳ�
		m_mapAllEffects["GAP_SOUL_HURT_RATIO"] = GAP_SOUL_HURT_RATIO;//������������˺��ӳ�
		m_mapAllEffects["GAP_DEMON_HURT_RATIO"] = GAP_DEMON_HURT_RATIO;//�Զ�ħ������˺��ӳ�
		m_mapAllEffects["GAP_INSECT_HURT_RATIO"] = GAP_INSECT_HURT_RATIO;//������������˺��ӳ�
		m_mapAllEffects["GAP_FIRE_HURT_RATIO"] = GAP_FIRE_HURT_RATIO;//�Ի�ϵ�����˺��ӳ�
		m_mapAllEffects["GAP_WATER_HURT_RATIO"] = GAP_WATER_HURT_RATIO;//��ˮϵ�����˺��ӳ�
		m_mapAllEffects["GAP_EARTH_HURT_RATIO"] = GAP_EARTH_HURT_RATIO;//����ϵ�����˺��ӳ�
		m_mapAllEffects["GAP_WIND_HURT_RATIO"] = GAP_WIND_HURT_RATIO;//�Է�ϵ�����˺��ӳ�
		m_mapAllEffects["GAP_DARK_HURT_RATIO"] = GAP_DARK_HURT_RATIO;//�԰�ϵ�����˺��ӳ�
		m_mapAllEffects["GAP_HOLINESS_HURT_RATIO"] = GAP_HOLINESS_HURT_RATIO;//��ʥϵ�����˺��ӳ�
		m_mapAllEffects["GAP_HURT_RID"] = GAP_HURT_RID;//����������
		m_mapAllEffects["GAP_MAGIC_HURT_RID"] = GAP_MAGIC_HURT_RID;//����ħ������
		m_mapAllEffects["GAP_SHIELD_DEF"] = GAP_SHIELD_DEF;//�����������
		m_mapAllEffects["GAP_TIME_LIMIT"] = GAP_TIME_LIMIT;//��ʱ
		m_mapAllEffects["GAP_ADDON_CHARM"] = GAP_ADDON_CHARM;//����
		m_mapAllEffects["GAP_HP_RESUME_RATE"] = GAP_HP_RESUME_RATE;//����ֵ�ָ�����
		m_mapAllEffects["GAP_BUFF_ATK_STRENGTH"] = GAP_BUFF_ATK_STRENGTH;//״̬����ǿ��
		m_mapAllEffects["GAP_PENETRATE"] = GAP_PENETRATE;//������͸
		m_mapAllEffects["GAP_EXCUSE_HURT"] = GAP_EXCUSE_HURT;//�˺�����
		m_mapAllEffects["GAP_LUCKY"] = GAP_LUCKY;//����
		m_mapAllEffects["GAP_ADDON_PENETRATE"] = GAP_ADDON_PENETRATE;//���Ӵ�͸����
		m_mapAllEffects["GAP_ADDON_EXCUSE_HURT"] = GAP_ADDON_EXCUSE_HURT;//�����˺�����
		m_mapAllEffects["GAP_ADDON_LUCKY"] = GAP_ADDON_LUCKY;//��������
		m_mapAllEffects["GAP_CUR_ATK_ED"] = GAP_CUR_ATK_ED;//ǿ������
		m_mapAllEffects["GAP_CUR_DEF_ED"] = GAP_CUR_DEF_ED;//ǿ������
		m_mapAllEffects["GAP_ADDON_ATK_ED"] = GAP_ADDON_ATK_ED;//����ǿ������
		m_mapAllEffects["GAP_ADDON_DEF_ED"] = GAP_ADDON_DEF_ED;//����ǿ������
		m_mapAllEffects["GAP_MIN_ATK_ADDON"] = GAP_MIN_ATK_ADDON;//С��׷��
		m_mapAllEffects["GAP_MAX_ATK_ADDON"] = GAP_MAX_ATK_ADDON;//��׷��
		m_mapAllEffects["GAP_MAGIC_ATK_ADDON"] = GAP_MAGIC_ATK_ADDON;//ħ��׷��
		m_mapAllEffects["GAP_DEF_ADDON"] = GAP_DEF_ADDON;		//���׷��
		m_mapAllEffects["GAP_MAGIC_DEF_ADDON"] = GAP_MAGIC_DEF_ADDON;//ħ��׷��
	}
}

// ���ļ���ȡԭʼ��Ʒ�б�
bool CGoodsList::LoadGoodsList(const char* filename)
{
	m_mapGoodsList.clear();
	m_mapGoodsListByName.clear();

	CRFile* prfile = rfOpen(filename);	
	if( prfile == NULL )
	{
		char str[256];
		sprintf(str, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
		return false;
	}

	// �ļ�ͷ�Ͱ汾
	char szHeader[6];
	memset(szHeader, 0, 6);
	prfile->ReadData(szHeader, 5);
	if( strcmp(szHeader, "GOODS") != 0 )
	{
		char str[256];
		sprintf(str, "file '%s' is not goodslist file!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
		rfClose(prfile);
		return false;
	}

	long lVersion = 0;
	prfile->ReadData(&lVersion, sizeof(long));

	// ��������

	long lNum = 0;
	prfile->ReadData(&lNum, sizeof(long));
	for(int i=0; i<lNum; i++)
	{
		long lStrSize=0;
		tagGoods2 stGoods;

		prfile->ReadData(&stGoods.BaseProperty.dwIndex, sizeof(stGoods.BaseProperty.dwIndex));

		prfile->ReadData(&lStrSize, sizeof(long));
		char *pStrOrginName=new char[lStrSize+1];	
		prfile->ReadData(pStrOrginName, lStrSize);
		pStrOrginName[lStrSize]='\0';
		stGoods.BaseProperty.strOrginName=pStrOrginName;
		SAFE_DELETE(pStrOrginName);
		prfile->ReadData(&lStrSize, sizeof(long));
		char *pStrName=new char[lStrSize+1];

		prfile->ReadData(pStrName, lStrSize);
		pStrName[lStrSize]='\0';
		stGoods.BaseProperty.strName=pStrName;
		SAFE_DELETE(pStrName);

		prfile->ReadData(&stGoods.BaseProperty.dwIconId, sizeof(stGoods.BaseProperty.dwIconId));		//	����ͼ��ID
		prfile->ReadData(&stGoods.BaseProperty.dwGroundId, sizeof(stGoods.BaseProperty.dwGroundId));		//	��غ�ͼ��ID
		prfile->ReadData(&stGoods.BaseProperty.dwEquipID, sizeof(stGoods.BaseProperty.dwEquipID));		//	��װͼ��ID
		prfile->ReadData(&stGoods.BaseProperty.dwSound, sizeof(stGoods.BaseProperty.dwSound));		//	ʰȡ����
		prfile->ReadData(&stGoods.BaseProperty.dwSoundID1, sizeof(stGoods.BaseProperty.dwSoundID1));		//	�Ӷ�/���˺�����
		prfile->ReadData(&stGoods.BaseProperty.dwSoundID2, sizeof(stGoods.BaseProperty.dwSoundID2));		//	���⹥��/��������
		prfile->ReadData(&stGoods.BaseProperty.bSoundSwitch, sizeof(stGoods.BaseProperty.bSoundSwitch));	//	����ʱ�Ƿ����
		prfile->ReadData(&stGoods.BaseProperty.dwWeaponActType, sizeof(stGoods.BaseProperty.dwWeaponActType));//	������Ӧ�Ķ�������
		prfile->ReadData(&stGoods.BaseProperty.dwType, sizeof(stGoods.BaseProperty.dwType));				// ����
		prfile->ReadData(&stGoods.BaseProperty.dwPrice, sizeof(stGoods.BaseProperty.dwPrice));				// ��Ҽ۸�
		prfile->ReadData(&stGoods.BaseProperty.dwSilverPrice, sizeof(stGoods.BaseProperty.dwSilverPrice));	// ���Ҽ۸�
		//prfile->ReadData(&stGoods.BaseProperty.dwWeight, sizeof(stGoods.BaseProperty.dwWeight));

		//������������
		prfile->ReadData(&lStrSize, sizeof(long));
		char *pStrContent=new char[lStrSize+1];	
		prfile->ReadData(pStrContent, lStrSize);
		pStrContent[lStrSize]='\0';
		stGoods.BaseProperty.strContent=pStrContent;
		SAFE_DELETE(pStrContent);

		long lEffectNum = 0;
		prfile->ReadData(&lEffectNum, sizeof(long));
		ushort wType=0;
		for(int j=0;j<lEffectNum;j++)
		{
			tagEffect stEffect;

			prfile->ReadData(&wType/*&stEffect.wType*/,sizeof(ushort));
			stEffect.wType=wType+1;
			prfile->ReadData(&stEffect.bEnable,sizeof(bool));
			prfile->ReadData(&stEffect.bHide,sizeof(bool));
			prfile->ReadData(&stEffect.dwValue1,sizeof(ulong));
			prfile->ReadData(&stEffect.dwValue2,sizeof(ulong));
			//prfile->ReadData(&stEffect.wOdds,sizeof(ushort));
			stGoods.vectorEffect.push_back(stEffect);
		}		

		//////////////////����װ����/////////////////////////
		prfile->ReadData(&lEffectNum, sizeof(long));
		for(int i=0;i<lEffectNum;i++)
		{
			tagGoodsBeControlledAttri stSuitSingleGoodsAttri;

			prfile->ReadData(&wType,sizeof(ushort));
			stSuitSingleGoodsAttri.wPropertyName = wType+1;
			prfile->ReadData(&(stSuitSingleGoodsAttri.dwValue1),sizeof(ulong));
			prfile->ReadData(&(stSuitSingleGoodsAttri.dwValue2),sizeof(ulong));
			prfile->ReadData(&(stSuitSingleGoodsAttri.wActiveNum),sizeof(ushort));

			stGoods.m_SuitProperty.push_back(stSuitSingleGoodsAttri);
		}

		////////////////���Լ���������////////////////////
		prfile->ReadData(&lEffectNum, sizeof(long));
		for(int i=0;i<lEffectNum;i++)
		{
			tagGoodsBeControlledAttri stGoodsAktiviertAttri;

			prfile->ReadData(&wType,sizeof(ushort));
			stGoodsAktiviertAttri.wPropertyName = wType+1;
			prfile->ReadData(&(stGoodsAktiviertAttri.dwValue1),sizeof(ulong));
			prfile->ReadData(&(stGoodsAktiviertAttri.dwValue2),sizeof(ulong));
			prfile->ReadData(&(stGoodsAktiviertAttri.wActiveNum),sizeof(ushort));

			stGoods.m_AkiviertProperty.push_back(stGoodsAktiviertAttri);
		}

		//////////////////////////////////////////////////////
		m_mapGoodsList[stGoods.BaseProperty.dwIndex] = stGoods;
		/////////////������Ϣ////////////
		//if (stGoods.BaseProperty.dwIndex==32)
		//{
		//	char buf[256];
		//	vector<CGoodsList::tagEffect>::iterator Iter = stGoods.vectorEffect.begin();
		//	for(;Iter!=stGoods.vectorEffect.end();Iter++)
		//	{
		//		PutDebugString(ltoa(Iter->wType,buf,10));
		//		PutDebugString(ltoa(Iter->dwValue1,buf,10));
		//	}
		//}
		////////////////////////////////
		m_mapGoodsListByName[stGoods.BaseProperty.strOrginName] = &m_mapGoodsList[stGoods.BaseProperty.dwIndex];
	}

	rfClose(prfile);
	return true;
}

//���ļ���ȡ��װ�����б�
void CGoodsList::LoadSuitList(const char* filename)
{
	InitMapEffects();
	m_SuitList.clear();
	CRFile* prfile = rfOpen(filename);

	if (prfile == NULL)
	{
		char str[256];
		sprintf(str, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
		return ;
	}
	stringstream  stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	ulong suitID = 0;						//�����װ��ID��
	string strSuitName;						//�����װ������ 
	while (ReadTo(stream,"SUIT_ID"))
	{
		stream>>suitID;
		stream>>strSuitName;
		m_mapSuitName.insert(pair<ulong,string>(suitID,strSuitName));
		if(ReadTo(stream,"<SUIT_GOODS>"))
		{		
			tagSuitAttribute tempSuit;
			//��ȡtagSuitAttribute��vector<string> Suit_OriginName��uchar	wSuit_CountҪ��ŵ�����
			string tempString;
			while (true)				
			{					
				stream>>tempString;
				if (tempString == "</SUIT_GOODS>")
				{
					break;
				}
				tempSuit.wSuit_Count = (uchar)atoi(tempString.c_str());
				string tempSuitOriginName;
				stream>>tempSuitOriginName;
				tempSuit.Suit_OriginName.push_back(tempSuitOriginName);
			}
			//��ȡmap<ulong,map<ulong,tagEffect>>��Ҫ��ŵ�����
			ulong SuitNum;				//��װ���ܵļ���
			string SuitAttriName;		//��װ�ڶ�Ӧ�����¼������������
			//ulong SuitAttriValue1,SuitAttriValue2;		//��Ӧ�ĸ������Ե�����ֵ
			while (ReadTo(stream,"NUM"))
			{
				stream>>SuitNum;			
				if (ReadTo(stream,"<ATTR>"))
				{	
					map<ulong,tagEffect> tempEffects;
					tagEffect tEffect;
					while (true)		
					{
						stream>>tempString;
						if (tempString == "</ATTR>"||tempString == "</ATTR_END>")
						{
							break;
						}
						SuitAttriName = tempString;
						stream>>tEffect.dwValue1>>tEffect.dwValue2;										//��ȡ������������ֵ
						ulong SuitAttriEnum;
						SuitAttriEnum = m_mapAllEffects[SuitAttriName];					//ͨ���������ҵ���Ӧ������ö��ֵ
						tempEffects.insert(pair<ulong,tagEffect>(SuitAttriEnum,tEffect));	
					}
					tempSuit.Suit_ActiveProperty[SuitNum] = tempEffects;
					if (tempString == "</ATTR_END>")
					{
						break;
					}
				}
			}
			m_SuitList.insert(pair<ulong,tagSuitAttribute>(suitID,tempSuit));
		}
	}
}

// ���ļ���ȡ�׿��б���Ϣ
void CGoodsList::LoadCardList(const char* filename)
{
	InitMapEffects();
	m_mapSeriesCardList.clear();
	CRFile* prfile = rfOpen(filename);

	if (prfile == NULL)
	{
		char str[256];
		sprintf(str, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
		return ;
	}
	stringstream  stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	ulong cardID = 0;							//����׿���ID��
	string CardAttriName = "";					//�׿��������������
	ulong CardAttriValue = 0;					//��Ӧ�����Ե�ֵ
	ulong CardAttriEnum = 0;					//���Զ�Ӧ��ö��ֵ

	while (ReadTo(stream,"<KIT_CARD>")&&!stream.eof())
	{
		char buf[256];
		int num = 0;
		tagSeriesCardProperty	tempCardProperty;
		if (ReadTo(stream,"KIT_CARD_ID"))
		{
			stream>>cardID;
			Log4c::Trace(ROOT_MODULE,ltoa(cardID,buf,10));
		}
		if (ReadTo(stream,"HOLE_NUM_REQUIRE"))
		{
			stream>>num;
			tempCardProperty.HOLE_NUM_REQUIRE = (uchar)num;
			Log4c::Trace(ROOT_MODULE,ltoa((long)tempCardProperty.HOLE_NUM_REQUIRE,buf,10));
		}
		if (ReadTo(stream,"CARD_NUM_REQUIRE"))
		{
			stream>>num;
			tempCardProperty.CARD_NUM_REQUIRE = (uchar)num;	
			Log4c::Trace(ROOT_MODULE,ltoa((long)tempCardProperty.CARD_NUM_REQUIRE,buf,10));
		}
		if(ReadTo(stream,"CARD_ID"))
		{		
			ulong tempCardID = 0;
			for (uchar i= 0;i<tempCardProperty.CARD_NUM_REQUIRE;i++)
			{
				stream>>tempCardID;
				Log4c::Trace(ROOT_MODULE,ltoa(tempCardID,buf,10));
				tempCardProperty.CardsID.push_back(tempCardID);
			}
		}
		if (ReadTo(stream,"<ATTR>"))
		{
			string tempString = "";
			while(true)
			{
				stream>>tempString;
				Log4c::Trace(ROOT_MODULE,tempString.c_str());
				if (tempString=="</ATTR>") break;
				if (tempString[0]=='/') continue;
				CardAttriName = tempString;
				stream>>CardAttriValue;
				Log4c::Trace(ROOT_MODULE,ltoa((long)CardAttriValue,buf,10));
				CardAttriEnum = m_mapAllEffects[CardAttriName];					//ͨ���������ҵ���Ӧ������ö��ֵ
				tempCardProperty.CardsEffects.insert(pair<ulong,int>(CardAttriEnum,CardAttriValue));
			}
		}
		if (ReadTo(stream,"</KIT_CARD>")) 
		{
			m_mapSeriesCardList[cardID] = tempCardProperty;
		}
	}
}

bool CGoodsList::FillGoodsInfo(ulong dwIndex,CGoods *pGood)
{	
	if(!dwIndex)return false;
	tagBaseProperty p=GetProperty(dwIndex)->BaseProperty;
	pGood->SetName(p.strName.c_str());
	pGood->m_dwIndex=dwIndex;
	//pGood->m_lNum=1;
	pGood->m_stBaseProperty.strName=p.strName;
	pGood->m_vectorEffect=GetProperty(dwIndex)->vectorEffect;
	pGood->m_stBaseProperty.dwValue = p.dwPrice;
	pGood->m_stBaseProperty.dwSilverPrice = p.dwSilverPrice;
	pGood->m_stBaseProperty.dwType = p.dwType;
	pGood->SetGoodsBaseType((eGoodsBaseType)p.dwType);
	return true;
}

long CGoodsList::GetEffectVal(ulong dwIndex,ushort wType,int nWhich)
{
	CGoodsList::tagGoods2* pstGoods2=CGoodsList::GetProperty(dwIndex);
	if (pstGoods2)
	{
		vector<CGoodsList::tagEffect>::iterator it=pstGoods2->vectorEffect.begin();
		for(;it!=pstGoods2->vectorEffect.end();it++)
		{
			if(it->wType==wType)
			{
				if(nWhich==1)
					return it->dwValue1;
				else if(nWhich==2)
					return it->dwValue2;
			}
		}
	}
	return 0;
}

CGoodsList::tagEffect* CGoodsList::GetEffectTag(ulong dwIndex,ushort wType)
{
	CGoodsList::tagGoods2* pstGoods=CGoodsList::GetProperty(dwIndex);
	if(pstGoods)
	{
		vector<CGoodsList::tagEffect>::iterator it=pstGoods->vectorEffect.begin();
		for(;it!=pstGoods->vectorEffect.end();it++)
		{
			if(it->wType == wType)
			{
				return &(*it);
			}
		}
	}
	return NULL;
}

bool CGoodsList::InitMoneyGraphics(const char* filename)
{
	m_stMoneyGraphics.clear();
	m_stSilverMoneyGraphics.clear();

	tagMoneyGraphics stMoney;

	stringstream stream;	
	CRFile* prfile = rfOpen(filename);
	if( !prfile )
	{
		char str[256];
		sprintf(str, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
		return false;
	}
	prfile->ReadToStream(stream);
	rfClose(prfile);
	int iLoopNum = 0;
	if (ReadTo(stream,"GoldMoney"))
	{
		stream>>iLoopNum;
		for (int i = 0;i<iLoopNum;i++)
		{
			if (ReadTo(stream,"#"))
			{
				stream >> stMoney.lMinNum >> stMoney.lMaxNum >> stMoney.lGraphicsID;
				m_stMoneyGraphics.push_back(stMoney);
			}
		}
	}
	if (ReadTo(stream,"SilverMoney"))
	{
		stream>>iLoopNum;
		for (int i = 0;i<iLoopNum;i++)
		{
			if (ReadTo(stream,"#"))
			{
				stream >> stMoney.lMinNum >> stMoney.lMaxNum >> stMoney.lGraphicsID;
				m_stSilverMoneyGraphics.push_back(stMoney);
			}
		}
	}

	return true;
}

long CGoodsList::GetMoneyGraphics(long lMoneyNum,int iMoneyType)
{
	vector<tagMoneyGraphics> t_vecMoney;
	// ���
	if (iMoneyType==GBT_MONEY)
	{
		t_vecMoney = m_stMoneyGraphics;
	}
	// ����
	else if (iMoneyType==GBT_SILVERMONEY)
	{
		t_vecMoney = m_stSilverMoneyGraphics;
	}
	vector<tagMoneyGraphics>::iterator iter = t_vecMoney.begin();
	for (;iter!=t_vecMoney.end();iter++)
	{
		if (lMoneyNum>=iter->lMinNum&&lMoneyNum<=iter->lMaxNum)
		{
			return iter->lGraphicsID;
		}
	}
	return 0;
}

// ����װ����װ��Ż�ȡ��Ʒ����
CGoodsList::tagGoods2*	CGoodsList::GetPropertyByEquipID(ulong dwEquipID)
{
	for (map<ulong, tagGoods2>::iterator it=m_mapGoodsList.begin(); it!=m_mapGoodsList.end(); it++)
	{
		if (it->second.BaseProperty.dwEquipID == dwEquipID)
		{
			return &it->second;
		}
	}
	return NULL;
}

//��ȡ��ID����װ��Ʒ����������(nWhich = 1 ȡ��װ������Ʒ��ĳ������ֵ1�� = 2ȡ����ֵ2��= 3ȡ����ֵ3���������Ǽ�����������ﵽ����װ��Ʒ������)
long CGoodsList::GetSuitAttrOfGoods(ulong dwIndex,ulong wType,int nWhich)
{
	CGoodsList::tagGoods2* pstGoods2=CGoodsList::GetProperty(dwIndex);
	if (pstGoods2)
	{
		vector<CGoodsList::tagGoodsBeControlledAttri>::iterator it=pstGoods2->m_SuitProperty.begin();
		for(;it!=pstGoods2->m_SuitProperty.end();it++)
		{
			if(it->wPropertyName==wType)
			{
				if(nWhich==1)
					return it->dwValue1;
				else if(nWhich==2)
					return it->dwValue2;
				else if(nWhich == 3)
					return it->wActiveNum;
			}
		}
	}
	return 0;
}

//��ȡͨ����װID���Ҷ�Ӧ����װ����
string& CGoodsList::GetSuitName(ulong dwSuitID)
{
	if (dwSuitID!=0&&m_mapSuitName.find(dwSuitID)!=m_mapSuitName.end())
	{
		return m_mapSuitName[dwSuitID];
	}
	return m_mapSuitName[0];
}

//��ȡ��ID����װ���ܼ���
WORD CGoodsList::GetSuitCount(ulong SuitID)	
{
	if (SuitID!=0&&m_SuitList.find(SuitID)!=m_SuitList.end())
	{
		return m_SuitList[SuitID].wSuit_Count;
	}
	return 0;
}

//����ĳ����Ƭ��ID���Ҵ����׿���ID
ulong CGoodsList::GetSeriesCardIndexByCardID(ulong cardIndex)
{
	map<ulong/*�׿���ID��*/,tagSeriesCardProperty>::iterator it = m_mapSeriesCardList.begin();
	for (;it!=m_mapSeriesCardList.end();it++)
	{
		vector<ulong> temp = m_mapSeriesCardList[it->first].CardsID;
		for (size_t i = 0;i<temp.size();i++)
		{
			if (cardIndex==temp[i])
			{
				return it->first;
			}
		}
	}
	return 0;
}

/// �Ƿ��д˸�������
bool CGoodsList::HaveEffect(ulong dwGoodsIndex,ushort wEffectType)
{
	tagGoods2* ptGoods = GetProperty(dwGoodsIndex);
	if (ptGoods!=NULL)
	{
		vector< CGoodsList::tagEffect >::iterator	iter;
		for( iter = ptGoods->vectorEffect.begin(); iter != ptGoods->vectorEffect.end(); iter++ )
		{
			if( iter->wType == wEffectType )
			{
				return true;
			}
		}
	}

	return false;
}

// �ж��Ƿ���װ��
bool CGoodsList::IsCanEquip(CGoods *pGoods)
{
	if(pGoods)
	{
		ulong dwType = pGoods->GetProperty()->dwType;
        if( dwType == GBT_EQUIPMENT)
			return true;
	}
	return false;
}

/// ��ȡ��Ʒ��ʱ��
const string CGoodsList::GetGoodsLimitTime(ulong dwGoodsIndex)
{
	//��ʧʱ��
	long val1 = GetEffectVal(dwGoodsIndex,GAP_TIME_LIMIT,1);
	long val2 = GetEffectVal(dwGoodsIndex,GAP_TIME_LIMIT,2);		
	long val3 = 0;
	long val4 = 0;
	long val5 = 0;
	char scResult[64] = "";
	char strTime[64]="";char strHour[32] = "";char strSecond[32] = "";
	/// ֵ1Ϊ1ʱ��ʾ��Ʒ�ܵ���Чʱ��
	if(val1==1)
	{		
		val3 = val2/1440;  val4 = val2%1440;	/// ������
		if (val3!=0)
		{
            sprintf(strTime,AppFrame::GetText("Goods_447"),val3);
		}
		val3 = val4/60; val5 = val4%60;			/// ��Сʱ����
		if (val3!=0)
		{
            sprintf(strHour,AppFrame::GetText("Goods_448"),val3);
			strcat(strTime,strHour);
		}
		if (val5!=0)
		{
            sprintf(strSecond,AppFrame::GetText("Goods_449"),val5);
			strcat(strTime,strSecond);
		}
        sprintf(scResult,AppFrame::GetText("Goods_445"),strTime);
		//strResult = scResult;
	}
	return string(scResult);
}

// ���ݴ������Ʒ��ö�Ӧ��Ʒ��ͼƬ�Լ���ɫ
void CGoodsList::GetGoodsQuality(ulong dwIndex,string & str, long & color)
{
	str = "";
	color = 0xffffffff;
	switch( GetEffectVal( dwIndex, GAP_QUALITY, 1 ) )
	{
	case 2:		// ��ɫƷ��
		{
			str = "datafiles/imagesets/public/icon_00A2FF.tga";
			color = 0xff00A2FF;
		}
		break;
	case 3:		// ��ɫƷ��
		{
			str = "datafiles/imagesets/public/icon_00FF19.tga";
			color = 0xff00FF19;
		}
		break;
	case 4:		// ��ɫƷ��
		{
			str = "datafiles/imagesets/public/icon_8400FF.tga";
			color = 0xff8400FF;
		}
		break;
	case 5:
		{
			str = "datafiles/imagesets/public/icon_FF00F0.tga";
			color = 0xffff00f0;
		}
		break;
	case 6:
		{
			str = "datafiles/imagesets/public/icon_dab26c.tga";
			color = 0xffdab26c;
		}
		break;
	case 7:
		{
			str = "datafiles/imagesets/public/icon_FF7200.tga";
			color = 0xffff7200;
		}
		break;
	case 8:
		{
			str = "datafiles/imagesets/public/icon_FF0000.tga";
			color = 0xffff0000;
		}
		break;
	case 9:		//�������̵���Ʒר�ñ߿�
		{
			str = "datafiles/imagesets/public/icon_Tradables.tga";
			color = 0xffff0000;
		}
		break;
	}
}