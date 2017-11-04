
#include "stdafx.h"
#include "SceneMgr.h"
#include "Renderer.h"
#include <random>

std::default_random_engine dre;
std::uniform_int_distribution<int> ui(-5, 5);
std::uniform_int_distribution<int> pos(-240, 240);


SceneMgr::SceneMgr() {
	m_Renderer = new Renderer(WindowWidth, WindowHeight);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
}

SceneMgr::~SceneMgr() {

}


void SceneMgr::BuildObjects() {
	AddObject(0, 0, ObjectType::OBJECT_BUILDING);
}

void SceneMgr::Collision() {
	for (auto& building : m_BuildingObjects) {
		building->setColor(float4(1, 1, 0, 1));
	}

	int damage = 0;
	//������Ʈ ������ �浹üũ
	for (auto& character : m_CharacterObjects) {
		//ĳ���� - ����
		for (auto& building : m_BuildingObjects) {
			if (character->getOOBB()->collision(*building->getOOBB())) {
				damage = character->getLife();
				character->setminusLife(damage);
				building->setminusLife(damage);
				building->setColor(float4(1, 0, 0, 1));
			}
		}

		//ĳ���� - �Ѿ�
		for (auto& bullet : m_BulletObjects) {
			if (character->getOOBB()->collision(*bullet->getOOBB())) {
				damage = bullet->getLife();
				character->setminusLife(damage);
				bullet->setminusLife(damage);
			}
		}
	}
}

void SceneMgr::Update(float ElapsedTime) {
	DWORD currTime = timeGetTime() *0.001f;

	if (currTime - prevTime > timeCount) {
		AddObject(0, 0, ObjectType::OBJECT_BULLET);
		prevTime = currTime;
	}
	
	std::list<Objects*>::iterator iter;

	//ĳ���� ������Ʈ
	for (iter = m_CharacterObjects.begin(); iter != m_CharacterObjects.end(); )
	{
		(*iter)->Update(ElapsedTime);
		if (!(*iter)->getLive()) {//Live�� False�� ��ü ����
			m_CharacterObjects.erase(iter++);
		}
		else {
			iter++;
		}
	}
	//���� ������Ʈ
	for (iter = m_BuildingObjects.begin(); iter != m_BuildingObjects.end(); )
	{
		(*iter)->Update(ElapsedTime);
		if (!(*iter)->getLive()) {//Live�� False�� ��ü ����
			m_BuildingObjects.erase(iter++);
		}
		else {
			iter++;
		}
	}
	//�Ѿ� ������Ʈ
	for (iter = m_BulletObjects.begin(); iter != m_BulletObjects.end(); )
	{
		(*iter)->Update(ElapsedTime);
		if (!(*iter)->getLive()) {//Live�� False�� ��ü ����
			m_BulletObjects.erase(iter++);
		}
		else {
			iter++;
		}
	}
	//ȭ�� ������Ʈ
	for (iter = m_ArrowObjects.begin(); iter != m_ArrowObjects.end(); )
	{
		(*iter)->Update(ElapsedTime);
		if (!(*iter)->getLive()) {//Live�� False�� ��ü ����
			m_ArrowObjects.erase(iter++);
		}
		else {
			iter++;
		}
	}

	//������Ʈ �浹üũ
	Collision();
}

void SceneMgr::Render() {
	for (auto& character : m_CharacterObjects) {
		character->Render(*m_Renderer);
	}
	for (auto& building : m_BuildingObjects) {
		building->Render(*m_Renderer);
	}
	for (auto& bullet : m_BulletObjects) {
		bullet->Render(*m_Renderer);
	}
	for (auto& arrow : m_ArrowObjects) {
		arrow->Render(*m_Renderer);
	}
}

void SceneMgr::MouseInput(int x, int y) {
	AddObject(x, y, ObjectType::OBJECT_CHARACTER);
}


void SceneMgr::AddObject(int x, int y, int type) {

	Objects* newObject = nullptr;

	switch (type) {
	case ObjectType::OBJECT_BUILDING:
		newObject = new Objects(float3(x, y, 0), float4(1, 1, 0, 1), 50, 0, "����",
			float3(ui(dre), ui(dre), 0), 0, 500);

		newObject->setType(ObjectType::OBJECT_BUILDING);

		m_BuildingObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_CHARACTER:
		newObject = new Objects(float3(x, y, 0), float4(1, 1, 1, 1), 10, 0, "ĳ����",
			float3(ui(dre), ui(dre), 0), 100, 10);

		newObject->setType(ObjectType::OBJECT_CHARACTER);

		m_CharacterObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_ARROW:
		newObject = new Objects(float3(x, y, 0), float4(0, 1, 0, 1), 2, 0, "ȭ��",
			float3(ui(dre), ui(dre), 0), 100, 10);

		newObject->setType(ObjectType::OBJECT_ARROW);

		m_ArrowObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_BULLET:
		newObject = new Objects(float3(x, y, 0), float4(1, 0, 0, 1), 2, 0, "�Ѿ�",
			float3(ui(dre), ui(dre), 0), 300, 20);
		
		newObject->setType(ObjectType::OBJECT_BULLET);

		m_BulletObjects.push_back(newObject);
		break;

	default:
		return;
	}
}