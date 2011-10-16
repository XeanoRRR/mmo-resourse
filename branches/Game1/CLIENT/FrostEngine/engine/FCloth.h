/**
	��ģ��ʵ�ֲ��İڶ�Ч��
	�ڶ����ɷ紵������������ƶ������
	�����˶������ʵ�-����ģ����ģ��,����:
	a----b----c
	|\../|\../|
	|.\/.|.\/.|
	|./\.|./\.|
	|/..\|/..\|
	e----O----f
	|\../|\../|
	|.\/.|.\/.|
	|./\.|./\.|
	|/..\|/..\|
	g----h----i	
	��ô��Ϊ�����������ֵ���:
	1.��Ľṹ����,ָ������������ڵ�ĵ���,��oe,of,ob,oh.�����ʵ���ʼ״̬ʱ�ľ���
	2.��ļ��е���,ָ���б�����ڵ�ĵ���,��oc,oa,og,oi.��ֹ����ƽ�����
	3.�����������,ָ��������������һ��ĵ�������ac,ag.��ֹ���κ�����
	���ֲ�ͬ����,���ǵĵ���ϵ������һ��
	�ڽ�������ģ��ʱ,�䶥������Ӧ����������Ҫ��:
	1.�ɾ�����������,w*h������,h��,w��
	2.���ж��������ͬһ��ƽ��
	3.������õȾ�����
	4.���ϵ�С����,�϶˹̶�
*/

#pragma once

#ifndef _FCLOTHMESH_H_
#define _FCLOTHMESH_H_

#include <d3dx9math.h>
#include "fmodel.h"

using namespace std;

typedef D3DXVECTOR3 Vector3;

//���еĽڵ�
struct CMVertex{
	Vector3 pos;//����
	Vector3 force;//����
	Vector3 velocity;//�ٶ�
	float oneOverMass;//�����ĵ���,���Ϊ0,��ʾ�õ����������,�����ƶ�
};

//���ɵĵ���
struct CMSpring{
	int v1,v2;//���ɵ�������������
	float restLen;//���ɾ�ֹʱ��ĳ���
	float ks;//���ɵĵ�������
	float kd;//���ɵ�����ϵ��
};
//����ײ�淢���Ӵ��ĽӴ���
struct CMCollisionPoint{
	int v;
	Vector3 normal;
};

class ClothyMesh{
public:
	ClothyMesh();
	virtual ~ClothyMesh();
public:
	//��һ��ģ�������첼����,���ģ�͸��𲼵���ʾ
	void Create(const model::Mesh *mesh,unsigned int w,unsigned int h);
	//�ö������鹹��һ�����Ķ���,��������ǳɾ������ж�������Ϊw*h,�������Щ�����ʼʱλ��ͬһ��ƽ��
	void Create(const Vector3 *pPosBuf,unsigned int w,unsigned int h);
	
	//������ײ�����Ϣ,ÿ����ײ����һ�������ι涨,pIndexBuff�Ǹ������νṹ��ָ��(ͬmodel::Mesh::tagTriangle)
	void SetCollisionMesh(Vector3 *pVertexBuff,unsigned int *pIndexBuff,unsigned int indexCount);
	//void SetCollisionMesh(model::Mesh *mesh);

	void SetClothy(float arg);//���ò����ʵ�,ָ�����ı�������,Խ�����Խ����
	void SetDampingFactor(float damp);//���ÿ����Բ����赲ϵ��(0-1),���������ٶȵı���
	void SetGravity(const Vector3& g);//�����������ٶȵķ���ʹ�С,������СӰ�첼���ھ�ֹ���ٶ�
	void SetFixedPoint(const Vector3 &pos);//ָ����Ҫ�̶������ĵ�

	void Move(float time);//ʹ����һ���˶�����,�仯ʱ�䷶Χ��time
	void Move(Vector3 &pos1,Vector3 &pos2,float time);//��������timeʱ���ڴ�pos1�ƶ���pos2
	void Move(Vector3 &offset,float time);//��������timeʱ���ڷ���λ��offset
	void Reset();//ʹ���е�ص��ʼ��״̬
	void MoveAndStand(const Vector3 &offset);//ƽ�Ʋ�,���ı䲼�ľֲ��˶�����
private:
	void AddSpring(int v1,int v2,float ks,float kd);
	void Simulate(float deltaTime);
	int  CheckCollisionState();
	void ResolveCollisions();
	void CalcVelocity();
	void CalcPosition(float deltaTime);
	void CopyPositions(Vector3 *pOut,int count);
private:
	bool IsPointInLineSegment(const Vector3 &p,const Vector3 &p1,const Vector3 &p2);
	bool IsPointInTriangle(const Vector3 &p,const Vector3 &p1,const Vector3 &p2,const Vector3 &p3);
	float GetAngleBetweenVectors(const Vector3 &v1,const Vector3 &v2);
private:
	model::Mesh *clothMesh;//����ģ��
	float damping;//�����Ĵ�С
	Vector3 gravity;//����
	CMVertex *vertexArray[3];//�������������,��һ������������ǰλ�ý���仯�ļ���,����������洢��ʼ״̬��λ����Ϣ
	unsigned int vertexCount;//��������
	unsigned int width,height;
	CMSpring *springArray;//��������
	unsigned int springCount;//��������
	Vector3 *collisionVertexArray;
	unsigned int *collisionIndexArray;
	unsigned int collisionIndexCount;
	CMCollisionPoint *collisionPointArray;
	unsigned int collisionCount;
};
#endif