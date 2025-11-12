#pragma once
#include "math.h"

#define ENTITY_LIST 0x7FF76A958B50
#define LOCAL_PLAYER 0x7FF76A958B70


class idPhysics_Player
{
public:
	char pad_0000[6840]; //0x0000
	Vector3 origin; //0x1AB8
}; //Size: 0x2010

class idPhysics_AI
{
public:
	char pad_0000[504]; //0x0000
	Vector3 origin; //0x01F8
}; //Size: 0x105C

class idAI2 {
public:
	char pad_0002[176812];
	float health; //0x2B2AC
	float maxHealth; //0x2B2B0
};

class angles {
public:
	Vector3 rotation;
};

extern angles* CurrentAngles;