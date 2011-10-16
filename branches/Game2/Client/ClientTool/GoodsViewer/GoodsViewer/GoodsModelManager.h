/**
* @filename:GoodsModelManager.h
* @date: 2009/12/28
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: ��Ʒģ����ʾ������������ģʽ,��ģ�͹�������ʹ�÷�װ���˴���
*/

#pragma once

#include "../../../EngineExtend/DisplayModel.h"

class GoodsModelManager
{
	DECLARE_SINGLETON(GoodsModelManager);
public:
	GoodsModelManager();
	~GoodsModelManager();


	/// @brief
	/// ��ָ����ģ���ļ�
	/// @param szModefFileName
	/// ģ���ļ���Ĭ��Ϊ"model"
	/// @param IsFullPathName
	/// ��ʶ�Ƿ�������·����Ĭ��Ϊfalse
	/// @return
	/// ��ģ�ͳɹ�������true��ʧ�ܷ���false
	bool OpenModel(const char *szModelFileName="model",bool IsFullPathName=false);

	/// @brief
	/// ��ȡ��ǰģ���ļ���
	const char* GetOpenModelFileName() const
	{
		return m_ModelFileName;
	}

	/// @brief
	/// ���õ�ǰ�򿪵�ģ���ļ�
	/// @param szModeFileName
	/// �ļ���
	void SetOpenModelFileName(const char* szModelFileName)
	{
		if( szModelFileName !=NULL )
		{
			strcpy(m_ModelFileName,szModelFileName);
		}
	}

	/// @brief
	/// ����ģ���ļ�·��
	/// @param szPathName
	/// Ĭ��Ϊ��
	void SetPathName(const char* szPathName="")
	{
		if(szPathName != NULL)
		{
			strcpy(m_PathName,szPathName);
		}
	}


	/// @brief
	/// ��ȡģ���ļ�·��
	/// @return
	/// const char*
	const char* GetPathName() const
	{
		return m_PathName;
	}

	/// @brief
	/// ��ʼ�������
	/// @return
	///  �ɹ�����true��ʧ�ܷ���false
	bool InitiliseCamera();


	/// @brief
	/// ���������λ�ú͹۲�λ��
	/// @param XPos
	/// X �ľ�������
	/// @param YPos
	/// Y �ľ�������,Ĭ��Ϊ0.5f
	/// @param ZPos
	/// Z �ľ������꣬Ĭ��Ϊ-3.0f
	void ResetCamera(float XPos = 0.0f ,float YPos = 0.5f ,float ZPos = -2.5f);

	/// @brief
	/// �ƶ�����������λ��
	/// @param relX
	/// X ���������
	/// @param relY
	/// Y ���������
	/// @param relZ
	/// Z ���������
	void MoveCameraRel(float relX = 0.0f,float relY = 0.0f,float relZ = 0.0f);

	/// @brief
	/// ��ȡ�����λ��
	/// @param
	/// 
	/// @return const D3DVECTOR3
	/// 
	const D3DXVECTOR3 GetCameraPosition() const { return *(m_Camera->GetPosition());} 
	/// @brief
	/// ������Ʒģ�͹�������״̬
	void Update();

	/// @brief
	/// ��Ⱦģ��

	bool RenderModel();


	/// @brief
	/// �ͷ�ģ����Դ
	void ReleaseModel();


	/// @brief
	/// ����ģ�͹����� 
	void Destory();


	D3DMATRIX GetWorlMatrix() const { return m_matWorld; }


	D3DMATRIX GetViewMatrix() const { return m_matView; }


	D3DMATRIX GetProjectMatrix() const { return m_matProject; }


	/// @brief
	/// ����ģ��λ�úͽǶ�
	void ResetPosAndRot() { m_PosX = 0.0f; m_PosY = 0.0f ; m_PosH = 0.0f; m_Rot = 0.0f; }

	/// @brief
	/// �ƶ�ģ�͵�����λ��
	/// @param X
	/// @parma Y
	/// @parma H
	void MoveTo(float X,float Y,float H) { m_PosX = X ; m_PosY = Y ; m_PosH = H ;}


	/// @brief
	/// �ƶ�ģ�͵����λ��
	/// @param relX
	/// @param relY
	/// @param relH

	void MoveRel(float relX,float relY,float relH) { m_PosX += relX; m_PosY += relY; m_PosH += relH; }


	/// @brief
	/// ģ����Y����ת
	/// @param angle
	/// ��ת�Ƕ�
	void RotationY(float angle) { m_Rot += angle; }

	/// @brief
	/// ģ�ͷ���
	/// @param matScal
	/// ��������

	void Scale(D3DXMATRIX &matScal){ m_cAnimInfo.SetScalMatrix(&matScal);}

	/// @brief
	/// ���ۻ���ʽ����ģ��
	/// @param relXScal
	/// X���ϵķ���ֵ
	/// @param relYScal
	/// Y���ϵķ���ֵ
	/// @param relZScal
	/// Z���ϵķ���ֵ
	void ScaleCumulate(float relXScal,float relYScal,float resZScal)
	{
		m_ScaleX += relXScal;
		m_ScaleY += relYScal;
		m_ScaleZ += resZScal;
		D3DXMATRIX matScal;
		D3DXMatrixScaling(&matScal,m_ScaleX,m_ScaleY,m_ScaleZ);
		Scale(matScal);
	};


	/// @brief
	/// �����ۻ��ķ���ֵ
	/// @param XScal
	/// Ĭ��Ϊ1.0
	/// @param YScal
	/// Ĭ��Ϊ1.0
	/// @param ZScal
	/// Ĭ��Ϊ1.0
	/// @return
	/// 
	void ResetScaleCumulate(float XScal = 1.0f,float YScal = 1.0f,float ZScal = 1.0f)
	{
		m_ScaleX = XScal;
		m_ScaleY = YScal;
		m_ScaleZ = ZScal;
	}
protected:
private:

	/// @brief
	/// ��������任
	void SetupTransform(void);
	

	/// @brief
	/// ��ԭ����任
	void ResetTransform();

	/// @brief
	/// ��������
	/// @param in rect
	/// �����ľ���
	void SetViewPort(const RECT& rect);

	/// @brief
	/// ����������С
	/// @param xStart
	/// ����X��ʼ����
	/// @param yStart
	/// ����Y��ʼ����
	/// @param Width
	/// �����Ŀ�
	/// @param Height
	/// @�����ĸ�
	void SetViewPort(int xStart,int yStart,int Width,int Height);

	//ģ���ļ�����
	char m_ModelFileName[MAX_PATH];
	//ģ���ļ�·��
	char m_PathName[MAX_PATH];

	//����ģ����ʾ
	CDisplayModel* m_DisplayModel;
	CGameModelManager* m_GameModelManager;	
	render::Camera* m_Camera;
	render::Frustum* m_Frustum;
	AnimInfo		m_cAnimInfo;///<ģ�Ͷ�����Ϣ

	D3DMATRIX		m_matWorld;///<���ڱ����޸�֮ǰ�ľ���
	D3DMATRIX		m_matView;
	D3DMATRIX		m_matProject;

	float	m_PosX;	///<ģ�͵�X����
	float   m_PosY;	///<ģ�͵�Y����
	float   m_PosH;    ///<ģ�͵�H���꣨�߶�),������Ļ��Y����
	float   m_Rot;		///<ģ����ת�Ƕ�

	
	float   m_ScaleX;  ///<�ۻ���ģ�ͷ���ֵX
	float   m_ScaleY;  ///<�ۻ���ģ�ͷ���ֵY
	float   m_ScaleZ;  ///<�ۻ���ģ�ͷ���ֵZ
	
	D3DVIEWPORT9 m_OldViewPort;	///<�洢����ģ��ǰ������;
};




