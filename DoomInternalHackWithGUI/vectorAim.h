#pragma once

#include "math.h"
#include "offsets.h"

#define M_PI 3.14159265358979323846

Vector3 GetAngle(Vector3 from, Vector3 to) {
	Vector3 out;
	out.y = -atan2f(to.x - from.x, to.y - from.y) / M_PI * 180.0f - 270;
	out.x = asinf((to.z - from.z) / Vector3::Distance(from, to)) * -180.0f / M_PI;
	return out;
}

void VectorAim() {
	int64_t* entityListAddress = (int64_t*)(mainModule + 0x2F4AB50);
	uint64_t* localEntAddress = reinterpret_cast<uint64_t*>(mainModule + 0x2F4AB70);
	uint64_t localEnt = *localEntAddress;
	idPhysics_Player* player = *(idPhysics_Player**)(localEnt + 0x808);
	idPhysics_AI targetEnt;
	float minDist = 1500;
	for (size_t i = 0; i < 16; i++)
	{
		int64_t* entityAddress = (int64_t*)(*entityListAddress + 0x18 + 0x10 * i);
		idAI2* enemyHealth = (idAI2*)*entityAddress;
		uint64_t* ent = (uint64_t*)(*entityAddress + 0x808);
		if (IsBadReadPtr((PVOID)ent, sizeof(PVOID)))
		{
			continue;
		}
		idPhysics_AI* enemy = (idPhysics_AI*)(*ent);
		if (enemy->origin == Vector3(0, 0, 0))
		{
			continue;
		}
		//if (CurrentAngles->rotation == Vector3(0, 0, 0))
		//{
			//continue;
		//}
		//std::cout << &(enemyHealth->health) << std::endl;
		if (enemyHealth->health <= 0)
		{
			continue;
		}
		float currentEntDist = Vector3::Distance(player->origin, enemy->origin);
		if (minDist > currentEntDist)
		{
			minDist = currentEntDist;
			targetEnt = *enemy;
		}
	}
	//if (GetAsyncKeyState('F') & 0x8001)
	//{
		//if (targetEnt.health > 0)
		//{
	//std::cout << targetEnt.origin << std::endl;
	if (GetAsyncKeyState('F') & 0x8001)
	{
		if (targetEnt.origin == Vector3(0, 0, 0))
		{
			return;
		}
		CurrentAngles->rotation = GetAngle(player->origin, targetEnt.origin - Vector3(0, 0, 20));
	}
	//std::cout << CurrentAngles->rotation << std::endl;
	//}
//}
}

void VectorAimOnline() {
	uint64_t* GamePointer = (uint64_t*)(mainModule + 0x5B0F6D0);
	uint64_t* Game = (uint64_t*)(*GamePointer);
	uint64_t* entityListAddress = (uint64_t*)((uintptr_t)Game + 0xA5548);
	uint64_t* localEntAddress = reinterpret_cast<uint64_t*>(mainModule + 0x2F4AB70);
	uint64_t localEnt = *localEntAddress;
	idPhysics_Player* player = *(idPhysics_Player**)(localEnt + 0x808);
	idPhysics_Player targetEnt;
	float minDist = 1500;
	for (size_t i = 0; i < 16; i++)
	{
		uint64_t* entityAddress = (uint64_t*)(*entityListAddress + 0x28 + 0x20 * i);
		idPlayer* enemyHealth = (idPlayer*)*entityAddress;
		uint64_t* ent = (uint64_t*)(*entityAddress + 0x808);
		if (IsBadReadPtr((PVOID)ent, sizeof(PVOID)))
		{
			continue;
		}
		idPhysics_Player* enemy = (idPhysics_Player*)(*ent);
		if (enemy->origin == Vector3(0, 0, 0))
		{
			continue;
		}
		//if (CurrentAngles->rotation == Vector3(0, 0, 0))
		//{
			//continue;
		//}
		//std::cout << &(enemyHealth->health) << std::endl;
		if (enemyHealth->health <= 0)
		{
			continue;
		}
		float currentEntDist = Vector3::Distance(player->origin, enemy->origin);
		if (minDist > currentEntDist)
		{
			minDist = currentEntDist;
			targetEnt = *enemy;
		}
	}
	//if (GetAsyncKeyState('F') & 0x8001)
	//{
		//if (targetEnt.health > 0)
		//{
	//std::cout << targetEnt.origin << std::endl;
	if (GetAsyncKeyState('F') & 0x8001)
	{
		if (targetEnt.origin == Vector3(0, 0, 0))
		{
			return;
		}
		CurrentAngles->rotation = GetAngle(player->origin, targetEnt.origin - Vector3(0, 0, 20));
	}
	//std::cout << CurrentAngles->rotation << std::endl;
	//}
//}
}