#pragma once
#include <math.h>

const int namebuff = 10;

class Renderer;


class Objects
{
protected:
	float3 m_Pos;		//��ġ
	float4 m_Color;	//����
	float m_Size;		//ũ��
	float m_Weight;	//����

	char* m_Name;	//�̸�
	
	float3 m_moveDir;		//�̵����� (x,y,z)
	float m_moveSpeed;	//�̵��ӵ�

	int now_crash_count;
	OOBB* m_oobb = nullptr;;

	bool m_Live;		//���ӻ� ǥ���Ǵ� ���� (true - ǥ���ض�, false -  ������ ����)
	int m_LifeTime;
	int m_Life;

	int m_type;

public:
	Objects();
	Objects(float x, float y, float z, float r, float g, float b, float a, float size, float weight, 
		char* name, float mx, float my, float mz, float speed, int life);
	Objects(float3 pos, float4 color, float size, float weight, char* name, float3 dir, float speed, int life);
	~Objects();

	//������ ����
	void setPos(float, float, float);
	void setPos(float3);
	void setColor(float, float, float, float);
	void setColor(float4);
	void setSize(float size) { m_Size = size; }
	void setWeight(float weight) { m_Weight = weight; }
	void setType(int type) { m_type = type; }

	void setLive(bool live) { m_Live = live; }
	void setName(char* name) { m_Name = name; }

	void setSpeed(float speed) { m_moveSpeed = speed; }
	void setMoveDir(float, float, float);
	void setMoveDir(float3);

	void setminusLife(int tmp) { m_Life -= tmp; }
	void setCollisionCheck(int tmp) { now_crash_count = tmp; }

	//���� �� �ҷ�����
	float3 getPos() const { return m_Pos; }
	float4 getColor() const { return m_Color; }
	float getSize() const { return m_Size; }
	float getWeight() const { return m_Weight; }
	int getType() const { return m_type; }

	bool getLive() const { return m_Live; }
	int getLife() const { return m_Life; }
	char* getName() const { return m_Name; }

	float getSpeed() const { return m_moveSpeed; }
	float3 getMoveDir() const { return m_moveDir; }

	OOBB* getOOBB() const { return m_oobb; }
	int getCollisonCheck() const { return now_crash_count; }
	//bool getNowCollision()const { return now_crash; }

	//������Ʈ ����
	void Move(float ElapsedTime);								//�̵��������� �̵��ӵ���ŭ �̵�
	void Move(float ElapsedTime, float3 moveValue);		//�ӵ����͸�ŭ �̵�


	virtual void Animate();					//�ִϸ���Ʈ
	virtual void CrashCheck();				//�浹üũ
	

	virtual void OnPrepareRender();		//������ ���� �����ؾ� �� �͵�
	virtual void Render(Renderer& renderer);					//������

	virtual void Update(float ElapsedTime);					//������Ʈ
};