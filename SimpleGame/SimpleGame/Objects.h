#pragma once
#include <math.h>

const int namebuff = 10;

class Renderer;

enum TEAM {TEAM_1=0, TEAM_2, NONE};

class RENDER_LEVEL
{
public:
	const float RENDER_GOD					= 0.0f;
	const float RENDER_BUILDING				= 0.1f;
	const float RENDER_CHARACTER			= 0.2f;
	const float RENDER_PROJECTILE			= 0.3f;
};

class Objects
{
protected:
	float3 m_Pos;		//��ġ
	float4 m_Color;	//����
	float4 m_teamColor; //��Į��
	float m_Size;		//ũ��
	float m_Weight;	//����
	
	UINT m_texIndex;		//�ؽ���
	bool	m_IsAnimate;		//�ִϸ��̼��� �ִ°�?
	int		m_CurrXSeq;		//����x������
	int		m_CurrYSeq;		//����y������
	int		m_MaxXSeq;		//�ִ�x������
	int		m_MaxYSeq;		//�ִ�y������
	float	m_aniTime;

	bool	m_IsPaticle;			//��ƼŬ�� �ִ°�
	UINT	m_PaticleTexIndex;	//��ƼŬ �ؽ���
	float	m_paticleTime;

	float m_RenderLevel; //������ ���� (�������� ���� �׷���)
	bool m_isProjecttile;

	char* m_Name;	//�̸�
	int m_id;
	
	float3 m_moveDir;		//�̵����� (x,y,z)
	float m_moveSpeed;	//�̵��ӵ�

	int now_crash_count;	//��� �浹�߳�?
	OOBB* m_oobb = nullptr;;	//OOBB�ڽ�

	bool m_Live;		//���ӻ� ǥ���Ǵ� ���� (true - ǥ���ض�, false -  ������ ����)
	int m_LifeTime;   //�ִ� ���� �ð�

	bool m_isLifeGuage; //������������ ǥ�� ����
	float m_LifeGuage;	//������������
	int m_FullLife;		//�ִ� HP
	int m_Life;			//���� HP
	char m_buf[10];	//������������ �ؽ�Ʈ

	int m_type;			//������Ʈ Ÿ��
	TEAM m_Team;		//��

	DWORD m_PrevTime; 
	float		m_LimitTime;

public:
	Objects();
	Objects(float x, float y, float z, float r, float g, float b, float a, float size, float weight, 
		char* name, float mx, float my, float mz, float speed, int life);
	//��ǥ, RGB , ������, ����, �̸�, �̵�����, �ӵ�, �����
	Objects(float3 pos, float4 color, float size, float weight, char* name, float3 dir, float speed, int life);
	//��ǥ, RGB , ������, ����, �̸�, �̵�����, �ӵ�, �����
	~Objects();

	//������ ����
	void setPos(float, float, float);
	void setPos(float3);
	void setColor(float, float, float, float);
	void setColor(float4);
	void setSize(float size) { 
		m_Size = size; 
		if (m_oobb)
			delete m_oobb;
		m_oobb = new OOBB(m_Pos, m_Size);
	}
	void setWeight(float weight) { m_Weight = weight; }
	void setType(int type) { m_type = type; }
	void setIsProjecttile(bool tmp) { m_isProjecttile = tmp; }

	void setLive(bool live) { m_Live = live; }
	void setName(char* name) { m_Name = name; }

	void setSpeed(float speed) { m_moveSpeed = speed; }
	void setMoveDir(float, float, float);
	void setMoveDir(float3);

	void setminusLife(int tmp);
	void setCollisionCheck(int tmp) { now_crash_count = tmp; }
	void setID(int tmp) { m_id = tmp; }
	void setTexIndex(int tmp) { 	m_texIndex = tmp; }
	void setTexSeq(int x, int y) {
		m_IsAnimate = true;
		m_CurrXSeq = 0;
		m_CurrYSeq = 0;
		m_MaxXSeq = x;
		m_MaxYSeq = y;
	}
	void setPaticle(UINT tex) {
		m_IsPaticle = true;
		m_PaticleTexIndex = tex;
		m_paticleTime = 0.0f;
	}

	void setTeam(TEAM tmp) { 
		m_Team = tmp; 
		if (tmp == TEAM::TEAM_1)
			m_teamColor = float4(1, 0, 0, 1);
		else if (tmp == TEAM::TEAM_2)
			m_teamColor = float4(0, 0, 1, 1);
	}
	void setRenderLevel(float tmp) { m_RenderLevel = tmp; }
	void setIsLifeGuage(bool tmp) { m_isLifeGuage = tmp; }

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
	int getID() const { return m_id; }
	int getTexIndex() const { return m_texIndex; }
	TEAM getTeam() const { return m_Team; }

	//bool getNowCollision()const { return now_crash; }

	//������Ʈ ����
	void Move(float ElapsedTime);								//�̵��������� �̵��ӵ���ŭ �̵�
	void Move(float ElapsedTime, float3 moveValue);		//�ӵ����͸�ŭ �̵�


	virtual void Animate();					//�ִϸ���Ʈ
	virtual void CrashCheck();				//�浹üũ
	

	virtual void OnPrepareRender();		//������ ���� �����ؾ� �� �͵�
	virtual void Render(Renderer& renderer);					//������

	virtual bool Update(float ElapsedTime);					//������Ʈ
};
