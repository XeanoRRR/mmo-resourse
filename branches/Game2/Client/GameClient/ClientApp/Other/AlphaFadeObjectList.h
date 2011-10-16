/* AlphaFadeObjectList.h
 * ����: ͸�����������ʾ�б���
 * ժҪ: ������������һ��������������Ҫ͸������������ʾ��Ϣ
 *		 Ŀǰ��Ҫ������͸�������Ķ���,���ñ���Ķ�����ڽ������ʱ,�Զ�ɾ����
 * ����: lpf
 * ��������: 2008-10-13
 */

#pragma once

class CShape;
class AnimInfo;
class CDisplayModel;

// ͸�����������ʾ�б���
class CAlphaFadeObjectList
{
	vector<CShape *> m_vecObjectList;	//������������

public:
	// ����͸������ģʽ
	enum eALPHA_FADE_MODE
	{
		ALPHA_FADE_NONE = 0,			//û�н������
		ALPHA_FADE_IN,					//����
		ALPHA_FADE_OUT,					//����
	};

	CAlphaFadeObjectList(void);			//���캯��
	~CAlphaFadeObjectList(void);		//��������

	// �����ӿ�
	void ReleaseAllObject();						//�ͷ����ж���
	void ReleaseObjectById(DWORD dwId);				//�ͷ�ָ����������
	void AddFadeInObject(CShape * pShape);			//����һ��������������
	void AddFadeOutObject(CShape * pShape);			//����һ��������������

	void Update();				//�����б��еĶ���
	void Render();				//��Ⱦ�б��еĶ���

private:
	void RenderShadow(CShape * pShape);				//��Ⱦ��Ӱ
};