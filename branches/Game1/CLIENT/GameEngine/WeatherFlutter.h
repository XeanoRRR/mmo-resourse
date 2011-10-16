/* WeatherFlutter.h
 * ����: Ϊ�˷ḻ����Ч����ʹ��Ϸ�����Եø��ӻ���ͬʱ����ӳ�г����廷���ĸо������ǽ��޸�
 *		 ����Ч���ı��ֻ����ӻ���Ч����ģ��ħ���������ڣ�ѩ�����裬�ѹ�Ӣ��������Ч����
 * ժҪ: �����趨�����ӡ�����ͼ�����Ƕ�������
 *		 �����趨�á����ӡ��Ĵ�С ������ԭʼģ�ʹ�С�ı���ֵ
 *		 �����趨��ʼ�߶ȣ���ˮƽ��Ϊ��0�����������븺ֵ��������ˮƽ������
 *		 �����趨�ڸ�ʸ�������ϵ�����ƶ�����ı仯���ֵ����Χֵ��
 *		 �����趨����ʱ�䣨��λ��MS���룩
 *		 �����趨�����ӡ���ʼ�ƶ�ʸ�������ٶ�ֵ
 *		 ���������������Ҹ�����ֵ�Ǻ㶨����ģ������Ƿ��������߳�����
 * ����: lpf
 * ��������: 2008-06-26
 */

#pragma once
#include "Weather.h"

class WeatherFlutter : public Weather
{
public:
	struct tagGranule
	{
		D3DXVECTOR3 vPos;			//�����ĵ�ǰ����
		D3DXVECTOR3 vSpeed;			//�������ٶ�ʸ��

		bool  bLiving;				//��ǰ�����Ƿ���
		DWORD dwLiveTimeMax;		//�����������ʱ��
		DWORD dwStartTime;			//���������Ŀ�ʼ����ʱ��
	};

private:
	tagGranule * m_tGranules;

	float m_fSize;      // ��׼�ߴ�

	D3DXVECTOR3 m_vSpeedMin;			//�����ٶ�ʸ������
	D3DXVECTOR3 m_vSpeedMax;			//�����ٶ�ʸ������
	DWORD m_dwLiveTimeMax;				//�����������ʱ��
	DWORD m_dwFrameLineNum;				//����֡�ĵ�������
	float m_fFrameWH;					//����֡��������
	DWORD m_dwFrameMax;					//�������������֡��
	DWORD m_dwFrameInterval;			//����������֡���

public:
	WeatherFlutter();					//���캯��
	virtual ~WeatherFlutter();			//��������

	void		 EnableLiving(bool bEnable);					//�趨���еĿ����Ƿ񱻼���
	virtual void ModifyParameters(const char * pszConfig);		//�޸Ĳ���

	virtual bool Create();				//������������
	virtual void Destroy();				//���ٷ�������

	virtual void Render();																//��Ⱦ����
	virtual bool Update(BOOL bEnable, D3DXVECTOR3 & vWindForce = D3DXVECTOR3(0, 0, 0));	//��������

private:
	virtual void ReLiveGranules(tagGranule * pFlutterGranules);									//����Ϊ���������������
	virtual void ProcessAnimTexture(VERTEX * pVertex, DWORD dwStartTime, DWORD dwCurrentTime);	//����������
};
