/**
* @filename:3DIcon.h
* @date: 2010/3/4
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: ���CEGUI+��â����ʵ��3DIcon�ؼ�
*/


#pragma  once

//class CBitmapX;
//class GameGraphics2D;
class CShape;
class AnimInfo;
namespace CEGUI
{
    class String;
    class WindowEventArgs;
    class RenderingSurface;
    struct Vertex;
}
class C3DIconEffect:public CEGUI::RenderEffect
{
public:
    C3DIconEffect(CEGUI::Window * owner);
    
	~C3DIconEffect();

    int getPassCount() const;
    void performPreRenderFunctions(const int pass);
    void performPostRenderFunctions();
	
	///  ������Ч����ӵ�����������
	bool realiseGeometry(CEGUI::RenderingWindow& window, CEGUI::GeometryBuffer& geometry);
    
    //ÿһ֡����
    bool update(const float elapsed, CEGUI::RenderingWindow& window);

	/// @brief
	/// ����������ת�Ƕ�
	/// @param angle
	/// ��ת�ĽǶ�(��λ��) 
	/// @return void
	/// 
	void ReSetRotAngle(float angle){ m_RotAngle = angle ; }
    float GetRotAngle()             {return m_RotAngle;}

	//������Ҫ��ʾ��Shape
    void SetIconShape(CShape *pShape);

    //����ģ����ת
    void SetLeftRot(bool b) { m_bLeftRotation = b; }
    void SetRightRot(bool b) { m_bRightRotation = b; }

    //������Ⱦλ��
    void SetIconPos(float x,float y,float z) {m_DrawX = x; m_DrawY = y; m_DrawZ = z;}

    //��������ֵ
    void SetIconScale(float xScale,float yScale,float zScale)
        {m_Scal[0] = xScale;
         m_Scal[1] = yScale;
         m_Scal[2] = zScale;}
    void SetRttType(long type)   {m_lRttType = type;}
protected:
	
	CShape   *m_Shape;//ģ������
	AnimInfo *m_AniInfo;//��������
	unsigned int m_ActionNum;//��������
	unsigned int m_RotDir;//��ת����0.��װ 1.��ת -1.��ת)
	
	float m_DrawX;//ģ�͵�X����
	float m_DrawY;//ģ�͵�Y����
    float m_DrawZ;//ģ�͵�Z����
	float m_RotAngle;//ģ�͵���ת�Ƕ�

	float m_Scal[3];//����
    int   m_3DIconTextureID;  //��Ⱦ��ģ������ID
    long  m_lRttType;
private:
    CEGUI::Vertex  vb[6];
    CEGUI::Window  *ownerWnd;
    bool    m_bLeftRotation;
    bool    m_bRightRotation;
 
};
