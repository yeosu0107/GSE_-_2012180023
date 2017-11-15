
#include "stdafx.h"
#include "SceneMgr.h"
#include "Renderer.h"
#include <random>

std::default_random_engine dre;

std::uniform_int_distribution<int> uix(-5, 5);
std::uniform_int_distribution<int> uiy(0, 5);

std::uniform_int_distribution<int> posx(-WindowWidth/2 + 30, WindowWidth/2 -30);
std::uniform_int_distribution<int> posy(30, 100);


SceneMgr::SceneMgr() {
	m_Renderer = new Renderer(WindowWidth, WindowHeight);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	m_texImage[0] = m_Renderer->CreatePngTexture("Resource/pic1.png");
	m_texImage[1] = m_Renderer->CreatePngTexture("Resource/pic2.png");
}

SceneMgr::~SceneMgr() {

}


void SceneMgr::BuildObjects() {
	//��
	AddObject(float3(0, WindowHeight / 2 - 150, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_1, m_texImage[0]);
	AddObject(float3(-WindowWidth/2 + 50, WindowHeight / 2 - 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_1, m_texImage[0]);
	AddObject(float3(+WindowWidth/2 - 50, WindowHeight / 2 - 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_1, m_texImage[0]);
	//��
	AddObject(float3(0, -WindowHeight / 2 + 150, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_2, m_texImage[1]);
	AddObject(float3(-WindowWidth/2 + 50, -WindowHeight / 2 + 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_2, m_texImage[1]);
	AddObject(float3(+WindowWidth/2 - 50, -WindowHeight / 2 + 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_2, m_texImage[1]);
}

void SceneMgr::Collision() {
	int damage = 0;
	//������Ʈ ������ �浹üũ

	for (auto& character : m_CharacterObjects) {
		//ĳ���� - ����
		for (auto& building : m_BuildingObjects) {
			if (character->getTeam() == building->getTeam())
				continue;

			if (character->getOOBB()->collision(*building->getOOBB())) {
				damage = character->getLife();
				character->setminusLife(damage);
				building->setminusLife(damage);
			}
		}

		//ĳ���� - �Ѿ�
		for (auto& bullet : m_BulletObjects) {
			if (character->getTeam() == bullet->getTeam())
				continue;

			if (character->getOOBB()->collision(*bullet->getOOBB())) {
				damage = bullet->getLife();
				character->setminusLife(damage);
				bullet->setminusLife(damage);
			}
		}

		//ĳ���� - ȭ��
		for (auto& arrow : m_ArrowObjects) {
			/*if (character->getID() == arrow->getID())
				continue;*/
			if (character->getTeam() == arrow->getTeam())
				continue;

			if (character->getOOBB()->collision(*arrow->getOOBB())) {
				damage = arrow->getLife();
				character->setminusLife(damage);
				arrow->setminusLife(damage);
			}
		}
	}

	for (auto& arrow : m_ArrowObjects) {
		//���� - ȭ��
		for (auto& building : m_BuildingObjects) {
			if (arrow->getTeam() == building->getTeam())
				continue;

			if (arrow->getOOBB()->collision(*building->getOOBB())) {
				damage = arrow->getLife();
				arrow->setminusLife(damage);
				building->setminusLife(damage);
			}
		}
	}
}

void SceneMgr::Update(float ElapsedTime) {
	DWORD currTime = timeGetTime() *0.001f;

	//1�� �������� �����ϴ� �Լ�
	if (currTime - prevTime[0] > timeLimit[0]) {
		CreateCharacterObjects(TEAM::TEAM_1);
		ShootBulletFromBuildings();
		ShootArrowFromCharacters();
		prevTime[0] = currTime;
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
	if (y > 0)
		return;
	DWORD currTime = timeGetTime() *0.001f;
	//2�� �������� �����ϴ� �Լ�
	if (currTime - prevTime[1] > timeLimit[1]) {
		AddObject(float3(x, y, 0), float3(uix(dre), uiy(dre), uix(dre)), ObjectType::OBJECT_CHARACTER, TEAM::TEAM_2, -1);
		prevTime[1] = currTime;
	}
	//float3 dir(float3(ui(dre), ui(dre), 0));
	//float3 pos(x, y, 0);
	//AddObject(pos, dir, ObjectType::OBJECT_CHARACTER, TEAM::TEAM_1, -1);
}


void SceneMgr::AddObject(float3 pos, float3 dir, int type, TEAM team, int texIndex) {

	Objects* newObject = nullptr;
	float4 color(0, 0, 0, 1);
	switch (type) {
	case ObjectType::OBJECT_BUILDING:
		newObject = new Objects(pos, float4(1, 1, 0, 1), 100, 0, "����",
			float3(1, 1, 1), 0, 500);

		newObject->setType(ObjectType::OBJECT_BUILDING);
		newObject->setTexIndex(texIndex);
		newObject->setTeam(team);
		m_BuildingObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_CHARACTER:
		switch (team) {
		case TEAM::TEAM_1:
			color.x = 1;
			break;
		case TEAM::TEAM_2:
			color.z = 1;
			break;
		default:
			break;
		}

		newObject = new Objects(pos, color, 10, 0, "ĳ����",
			dir, 300, 10);

		newObject->setType(ObjectType::OBJECT_CHARACTER);
		newObject->setID(characterID);
		characterID += 1;
		
		newObject->setTeam(team);
		m_CharacterObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_BULLET:
		switch (team) {
		case TEAM::TEAM_1:
			color.x = 1;
			break;
		case TEAM::TEAM_2:
			color.z = 1;
			break;
		default:
			break;
		}

		newObject = new Objects(pos, color, 2, 0, "�Ѿ�",
			dir, 600, 20);
		
		newObject->setType(ObjectType::OBJECT_BULLET);
		newObject->setTeam(team);
		m_BulletObjects.push_back(newObject);
		break;

	/*case ObjectType::OBJECT_ARROW:
		switch (team) {
		case TEAM::TEAM_1:
			color.x = 0.5;
			color.y = 0.2;
			color.z = 0.7;
			break;
		case TEAM::TEAM_2:
			color.x = 1;
			color.y = 1;
			break;
		default:
			break;
		}

		newObject = new Objects(pos, color, 2, 0, "ȭ��",
			float3(ui(dre), ui(dre), 0), 100, 10);

		newObject->setType(ObjectType::OBJECT_ARROW);
		newObject->setTeam(team);
		m_ArrowObjects.push_back(newObject);
		break;*/

	default:
		return;
	}
}

void SceneMgr::AddArrow(float3 pos, float3 dir, int id, TEAM team)
{
	Objects* newObject = nullptr;
	float4 color(0, 0, 0, 1);

	switch (team) {
	case TEAM::TEAM_1:
		color.x = 0.5;
		color.y = 0.2;
		color.z = 0.7;
		break;
	case TEAM::TEAM_2:
		color.x = 1;
		color.y = 1;
		break;
	default:
		break;
	}
	newObject = new Objects(pos, color, 2, 0, "ȭ��",
		dir, 100, 10);

	newObject->setType(ObjectType::OBJECT_ARROW);
	newObject->setID(id);
	newObject->setTeam(team);
	m_ArrowObjects.push_back(newObject);
}

void SceneMgr::ShootBulletFromBuildings()
{
	float3 dir(float3(uix(dre), uiy(dre), 0));
	float3 pos(0, 0, 0);

	//������ �Ѿ��� ��
	for (const auto building : m_BuildingObjects) {
		pos = building->getPos();
		if (building->getTeam() == TEAM::TEAM_1)
			dir.y *= -1;

		AddObject(pos, dir, ObjectType::OBJECT_BULLET, building->getTeam(), -1);
	}
}

void SceneMgr::ShootArrowFromCharacters()
{
	//ĳ���Ͱ� ȭ���� ��
	for (auto& character : m_CharacterObjects) {
		float3 pos = character->getPos();
		float3 dir = character->getMoveDir();

		AddArrow(pos, dir, character->getID(), character->getTeam());
	}
}

void SceneMgr::CreateCharacterObjects(TEAM team)
{
	float3 pos(posx(dre), +WindowHeight / 2 - posy(dre), 0);
	float3 dir(uix(dre), uiy(dre), uix(dre));
	dir.y *= -1;

	AddObject(pos, dir, ObjectType::OBJECT_CHARACTER, team, -1);
}