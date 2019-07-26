#include <cstdint>
#include "../netvars.hpp"

enum ECSPlayerRank
{
	UnRanked,
	SilverI,
	SilverII,
	SilverIII,
	SilverIV,
	SilverElite,
	SilverEliteMaster,
	GoldNovaI,
	GoldNovaII,
	GoldNovaIII,
	GoldNovaMaster,
	MasterGuardianI,
	MasterGuardianII,
	MasterGuardianElite,
	DistinguishedMasterGuardian,
	LegendaryEagle,
	LegendaryEagleMaster,
	SupremeMasterFirstClass,
	TheGlobalElite
};

class CPlayerResource {
};

class C_CSPlayerResource : public CPlayerResource
{
public:
	int GetPing(int index)
	{
		return *(int*)((uintptr_t)this + 0x00000B28 + index * 4);
	}

	int GetAssists(int index)
	{
		return *(int*)((uintptr_t)this + 0x00000D30 + index * 4);
	}

	int GetDeaths(int index)
	{
		return *(int*)((uintptr_t)this + 0x00000E34 + index * 4);
	}

	bool GetConnected(int index)
	{
		return *(bool*)((uintptr_t)this + 0x00000AE4 + index);
	}

	int GetPendingTeam(int index)
	{
		return *(int*)((uintptr_t)this + 0x0000103C + index * 4);
	}

	bool GetAlive(int index)
	{
		return *(bool*)((uintptr_t)this + 0x00001140 + index);
	}

	int GetHealth(int index)
	{
		return *(int*)((uintptr_t)this + 0x00001184 + index * 4);
	}

	int GetMVPs(int index)
	{
		return *(int*)((uintptr_t)this + 0x000016F4 + index * 4);
	}

	int GetArmor(int index)
	{
		return *(int*)((uintptr_t)this + 0x0000187C + index * 4);
	}

	int GetScore(int index)
	{
		return *(int*)((uintptr_t)this + 0x00001980 + index * 4);
	}

	int* GetCompetitiveRanking(int index)
	{
		return (int*)((uintptr_t)this + 0x00001A84 + index * 4);
	}

	int* GetCompetitiveWins(int index)
	{
		return (int*)((uintptr_t)this + 0x00001B88 + index * 4);
	}

	int GetCompTeammateColor(int index)
	{
		return *(int*)((uintptr_t)this + 0x00001CD0 + index * 4);
	}

	const char* GetClan(int index)
	{
		return (const char*)((uintptr_t)this + 0x000043AC + index * 16);
	}

	int* GetActiveCoinRank(int index)
	{
		return (int*)((uintptr_t)this + 0x00004AC8 + index * 4);
	}

	int* GetMusicID(int index)
	{
		return (int*)((uintptr_t)this + 0x00004BCC + index * 4);
	}

	int* GetPersonaDataPublicCommendsLeader(int index)
	{
		return (int*)((uintptr_t)this + 0x00004DD4 + index * 4);
	}

	int* GetPersonaDataPublicCommendsTeacher(int index)
	{
		return (int*)((uintptr_t)this + 0x00004ED8 + index * 4);
	}

	int* GetPersonaDataPublicCommendsFriendly(int index)
	{
		return (int*)((uintptr_t)this + 0x00004FDC + index * 4);
	}
};