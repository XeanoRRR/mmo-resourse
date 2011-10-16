/**
* @filename:GoodsRender.h
* @date: 2010/3/17
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: ��Ʒ����Ⱦ������
*/

class CDisplayModel;
class AnimInfo;
class CGameModelManager;

//�궨��Ĭ����Ʒ�ķ���ֵ
#ifndef DEFAULT_SCALE
#define DEFAULT_SCALE 1500.0f
#endif

#ifndef DEFAULT_ZDISTANCE
#define DEFAULT_ZDISTANCE 1000
#endif

#ifndef DEFAULT_XTRANS
#define DEFAULT_XTRANS 370
#endif

#ifndef DEFAULT_YTRANS
#define DEFAULT_YTRANS 0
#endif

class GoodsRender 
{
public:
	GoodsRender();
	~GoodsRender();

	/// @brief
	/// ����ģ��·����ID����ģ��
	/// @param PathName
	/// ģ���ļ��Ĵ洢·��
	/// @param ID
	/// ģ�͵�Graphite ID
	/// @return
	/// 
	static bool LoadModel(uint ID,const char* PathName ="model/goods");

	/// @brief
	/// ģ�͵���Ⱦ	
	/// @param
	/// 
	/// @return
	/// 
	static void Render();

	/// @brief
	/// ����������ת�Ƕ�����
	/// @param xRot
	/// ��X����ת�Ƕ�����
	/// @param yRot
	/// ��Y����ת�Ƕ�����
	/// @param zRot
	/// ��Z����ת�Ƕ�����
	/// @return
	/// 
	static void ReSetRotAngle(float xRot = 5.0f ,float yRot = 5.0f ,float zRot = 5.0f) ;

	/// @brief
	/// ��������ģ�ͷ���ֵ
	/// @param scale
	/// ����ֵ��С
	/// @return
	/// 
	static void ResetScal(float scale = 2.0f);

	/// @brief
	/// ��������ģ��λ��
	/// @param xTran
	/// ģ����X���ϵ�ƽ��
	/// @param yTran
	/// ģ����Y���ϵ�ƽ��
	/// @param zTran
	/// ģ����Z���ϵ�ƽ��
	/// @return
	/// 
	static void ResetTranslation(float xTran = 1 ,float yTran = 1 ,float zTran = 1 );
	/// @brief
	/// ���ø����ϵ�ƽ������ֵ
	/// @param xTranAdd
	/// ģ����X���ϵ�ƽ������
	/// @param yTran
	/// ģ����Y���ϵ�ƽ������
	/// @param zTran
	/// ģ����Z���ϵ�ƽ������
	/// @return
	static void SetTranAdd(float xTranAdd = 1,float yTranAdd = 1, float zTranAdd = 1); 
protected:

private:
	static float m_XRot;
	static float m_YRot;
	static float m_ZRot;
	static float m_Scale;
	static float m_XTrans;
	static float m_YTrans;
	static float m_ZTrans;

	//����ģ����ʾ
	static CDisplayModel* m_DispModel;
	static CGameModelManager* m_GameModelManager;
	static AnimInfo* m_AniInfo;//ģ�͵Ķ���
};