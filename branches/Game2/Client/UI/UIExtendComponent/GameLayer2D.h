/*
brief:��Ϸ2D�� λ�ڳ���3D���ϣ�UI���£��������TTF���֣�CEGUI::ImageSetManager�����ͼƬ
��2D��ʹ��Root ����ĺ�̨������
*/

#pragma  once
namespace CEGUI
{
    class GeometryBuffer;
}
class Layer2D
{
public:
    Layer2D();
    ~Layer2D();
private:
    CEGUI::GeometryBuffer * m_pRootBuff;
    CEGUI::GeometryBuffer * m_pCustomBuff;
    D3DXMATRIX              m_matrix;
    bool                    m_IsMatrixValid;
public:
    void RenderImage(int iX,int iY,int iWidth,int iHeight, const char *imageSetName, const char *image = "full_image");
    void RenderImage(CEGUI::GeometryBuffer& pDestBuff, int iX,int iY,int iWidth,int iHeight, const char *imageSetName, const char *image = "full_image");
    void RenderText(int iX,int iY,const char *pszString, D3DCOLOR color, int iStyleIndex=0,const float space_extra = 0.0f,
        const float x_scale = 1.0f, const float y_scale = 1.0f);
	void RenderRollText(float fStartX,float fStartY,
		const char* strRenderText,
		D3DCOLOR color,
		CEGUI::Rect* clip_rect = 0,
		int iPixSpeed = 1,
		int iRollStyle = 1,
		const float x_scale = 1.0f,
		const float y_scale = 1.0f,
		const float space_extra = 0.0f);
    void ClearBuffer();
    void SetupView();
};