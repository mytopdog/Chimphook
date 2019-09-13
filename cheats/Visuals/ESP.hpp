#pragma once
#include "../common_includes.hpp"

class Visuals : public Singleton<Visuals>
{
public:
	class Player
	{
	public:
		struct
		{
			C_BasePlayer* pl;
			bool          is_enemy;
			bool          is_teammate;
			bool          is_localplayer;
			bool          is_visible;
			float         distance;
			float         font_size;
			ImVec2        cursor;
			Color         clr;
			Vector        head_pos;
			Vector        feet_pos;
			RECT          bbox;
		}ctx;

		void Begin(C_BasePlayer* pl);
		void RenderBox(Color col);
		void RenderName(Color col);
		void RenderSkeleton(Color col);
		void RenderBacktrackSkeleton(Color col);
		void RenderHealth(Color col);
		void RenderWeapon(Color col);
		void RenderSnapline(Color col);
		void RenderPosition(Color col);
		void RenderEyeLine(Color col);
	};
public:
	void AddToDrawList();
};

void BulletShots(IGameEvent* ev);