/**
* @filename:AnimEffectIcon.h
* @date: 2010/3/4
* @author:  nikai
* @purpose: ����֡����
*/

#pragma  once

namespace CEGUI
{
    class String;
    class Imageset;
}

class AnimEffectIcon
{
public:
    AnimEffectIcon(CEGUI::Window * owner,CEGUI::Imageset & ImageSet, bool isLooped = false);
	
    ~AnimEffectIcon();
    
    //ÿһ֡����
    bool update();

    //���ÿ�ʼ����
    /*param:totalTime ����������ʱ��*/
    void BeginAnimateEffect(DWORD totalTime);
private:
    // ��������ָ��
    CEGUI::Window * OwnerWnd;

    // ������֡��
    long lFrameCount;

    // ������ʼ֡
    long lStartFrame;

    // ��������֡
    long EndFrame;

    // �����ĵ�ǰ֡
    long lCurFrame;
   
    // ����ÿ֡��ʱ��
    unsigned long lFrameTime;
    
    // ���һ�λ��Ƶ�ʱ��
    unsigned long lLastDrawTime;
   
    // ����ѭ����־
    bool bIsLoop;

    //����������ʱ��
    DWORD dwAnimTotalTime;

    //�������ſ�ʼʱ��
    DWORD dwAnimStartTime;

    //�Ƿ�����������
    bool  bIsAniming;
    
    //����ͼ��
    CEGUI::Imageset & AnimImageSet;
};