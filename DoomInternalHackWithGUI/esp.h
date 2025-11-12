#pragma once

#include "math.h"
#include "offsets.h"
#include <iostream>
#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_internal.h"
#include <Windows.h>

ImColor GetBarColor(float percent) {
	ImVec4 red(1, 0, 0, 1);
	ImVec4 yellow(1, 1, 0, 1);
	ImVec4 green(0, 1, 0, 1);

	ImColor color;
	if (percent <= 0.5)
	{
		color = ImLerp(red, yellow, percent * 2);
	}
	else
	{
		color = ImLerp(yellow, green, (percent - 0.5f) * 2);
	}
	return color;
}

void RenderESP() {
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	view_matrix_t* vm = (view_matrix_t*)(mainModule + 0x5B0D580);
	int64_t* entityListPointer = (int64_t*)(mainModule + 0x02F48B50);

	if (!*entityListPointer)
	{
		return;
	}

	for (size_t i = 0; i < 32; i++)
	{
		int64_t* entityPointer = (int64_t*)(*entityListPointer + 0x18 + 0x10 * i);
		uint64_t* entTemp = (uint64_t*)(*entityPointer + 0x808);
		idAI2* enemyHealth = (idAI2*)(*entityPointer);

		if (IsBadReadPtr((PVOID)entTemp, sizeof(PVOID)))
		{
			continue;
		}

		idPhysics_AI* ent = (idPhysics_AI*)(*entTemp);

		if (ent->origin == Vector3(0, 0, 0))
		{
			continue;
		}
		if (enemyHealth->health <= 0)
		{
			continue;
		}

		Vector3 bottomPoint;
		Vector3 topPoint;
		float maxHealth = enemyHealth->maxHealth;
		float percent = enemyHealth->health / maxHealth;
		float horizontalOffset = 5;

		if (worldToScreenPoint(ent->origin + Vector3(0, 0, 50), *vm, topPoint) && worldToScreenPoint(ent->origin, *vm, bottomPoint))
		{
			float width = (bottomPoint.y - topPoint.y) / 5;
			drawList->AddLine(topPoint + Vector3(width, 0, 0), topPoint, IM_COL32(255, 0, 0, 255), 1);
			drawList->AddQuad(
				topPoint - Vector3(width, 0, 0),
				topPoint + Vector3(width, 0, 0),
				bottomPoint + Vector3(width, 0, 0),
				bottomPoint - Vector3(width, 0, 0),
				IM_COL32(0, 0, 0, 255), 3
			);
			drawList->AddQuad(
				topPoint - Vector3(width, 0, 0),
				topPoint + Vector3(width, 0, 0),
				bottomPoint + Vector3(width, 0, 0),
				bottomPoint - Vector3(width, 0, 0),
				IM_COL32(255, 0, 0, 255), 2
			);
			drawList->AddLine(Vector2(clientWidth / 2, clientHeight), bottomPoint, IM_COL32(255, 0, 0, 255), 1);

			if (config.ESPHealthBarBool)
			{
				float boxWidth = (bottomPoint.y - topPoint.y) / 5;
				float lineHeight = (bottomPoint.y - topPoint.y) * percent;
				float lineWidth = (bottomPoint.x - topPoint.x) * percent;
				//drawList->AddCircle(bottomPoint + Vector3(-boxWidth, 0, 0), 5, IM_COL32(255, 0, 0, 255), 10, 3);
				//drawList->AddCircle(topPoint + Vector3(-boxWidth, 0, 0), 5, IM_COL32(255, 0, 0, 255), 10, 3);
				//drawList->AddCircle(bottomPoint + Vector3(-boxWidth - lineWidth, -lineHeight, 0), 5, IM_COL32(255, 0, 0, 255), 10, 3);
				drawList->AddLine(
					bottomPoint + Vector3(-boxWidth - horizontalOffset, 1, 0),
					topPoint + Vector3(-boxWidth - horizontalOffset, -1, 0),
					IM_COL32(0, 0, 0, 255), 6);
				drawList->AddLine(
					bottomPoint + Vector3(-boxWidth - horizontalOffset, 0, 0),
					bottomPoint + Vector3(-boxWidth - lineWidth - horizontalOffset, -lineHeight, 0),
					GetBarColor(percent), 4);
			}
		}
	}
}