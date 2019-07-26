#pragma once
#include "../../hooks.hpp"
#include "pbwrap.hpp"
#include "ProtoParse.h"

#define k_EMsgGCCStrike15_v2_MatchmakingGC2ClientReserve 9107
#define k_EMsgGCClientWelcome 4004
#define k_EMsgGCClientHello 4006
#define k_EMsgGCAdjustItemEquippedState 1059
#define k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello 9109
#define k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello 9110
#define k_EMsgGCCStrike15_v2_ClientGCRankUpdate  9194

//FORMAT: constexpr static Tag field_name = {field_id, field_type};

using namespace pbwrap;

struct CMsgClientHello : pbmsg<8>
{
	PBMSG_CTOR;
	PBFIELD(3, types::Uint32, client_session_need);
};

// ProfileChanger
struct MatchmakingGC2ClientHello : pbmsg<20>
{
	struct PlayerRankingInfo : pbmsg<6>
	{
		PBMSG_CTOR;
		PBFIELD(1, types::Uint32, account_id);
		PBFIELD(2, types::Uint32, rank_id);
		PBFIELD(3, types::Uint32, wins);
		PBFIELD(6, types::Uint32, rank_type_id);
	};
	struct PlayerCommendationInfo : pbmsg<4>
	{
		PBMSG_CTOR;
		PBFIELD(1, types::Uint32, cmd_friendly);
		PBFIELD(2, types::Uint32, cmd_teaching);
		PBFIELD(4, types::Uint32, cmd_leader);
	};

	PBMSG_CTOR;
	PBFIELD(7, PlayerRankingInfo, ranking);
	PBFIELD(8, PlayerCommendationInfo, commendation);
	PBFIELD(17, types::Int32, player_level);
};

struct CMsgGCCStrike15_v2_ClientGCRankUpdate : pbmsg<1>
{
	PBMSG_CTOR;
	PBFIELD(1, MatchmakingGC2ClientHello::PlayerRankingInfo, ranking);
};

// InvChanger
struct CSOEconItemEquipped : pbmsg<2>
{
	PBMSG_CTOR;
	PBFIELD(1, types::Int32, new_class);
	PBFIELD(2, types::Int32, new_slot);
};

struct CSOEconItemAttribute : pbmsg<3>
{
	PBMSG_CTOR;
	PBFIELD(1, types::Uint32, def_index);
	PBFIELD(2, types::Uint32, value);
	PBFIELD(3, types::Bytes, value_bytes);
};

struct CSOEconItem : pbmsg<19>
{
	PBMSG_CTOR;
	PBFIELD(1, types::Uint64, id);
	PBFIELD(2, types::Uint32, account_id);
	PBFIELD(3, types::Uint32, inventory);
	PBFIELD(4, types::Int32, def_index);
	PBFIELD(5, types::Uint32, quantity);
	PBFIELD(6, types::Uint32, level);
	PBFIELD(7, types::Uint32, quality);
	PBFIELD(8, types::Uint32, flags);
	PBFIELD(9, types::Uint32, origin);
	PBFIELD(10, types::String, custom_name);
	PBFIELD(12, CSOEconItemAttribute, attribute); //check
	PBFIELD(14, types::Bool, in_use);
	PBFIELD(15, types::Uint32, style);
	PBFIELD(16, types::Uint64, original_id);
	PBFIELD(18, CSOEconItemEquipped, equipped_state);
	PBFIELD(19, types::Uint32, rarity);
};

struct CMsgClientWelcome : pbmsg<11>
{
	struct SubscribedType : pbmsg<2>
	{
		PBMSG_CTOR;
		PBFIELD(1, types::Int32, type_id);
		PBFIELD(2, CSOEconItem, object_data);
	};

	struct CMsgSOCacheSubscribed : pbmsg<4>
	{
		PBMSG_CTOR;
		PBFIELD(2, SubscribedType, objects);
	};

	PBMSG_CTOR;
	PBFIELD(3, CMsgSOCacheSubscribed, outofdate_subscribed_caches);
};

struct CMsgAdjustItemEquippedState : pbmsg<4>
{
	PBMSG_CTOR;
	PBFIELD(1, types::Uint64, item_id);
	PBFIELD(2, types::Uint32, new_class);
	PBFIELD(3, types::Uint32, new_slot);
	PBFIELD(4, types::Bool, swap);
};

/*
struct CSUsrMsg_VoteStart : pbmsg<7>
{
	PBMSG_CTOR;

	PBFIELD(1, types::Uint32, team);
	PBFIELD(2, types::Uint32, ent_idx);
	PBFIELD(3, types::Uint32, vote_type);
	PBFIELD(4, types::String, disp_str);
	PBFIELD(5, types::String, details_str);
	PBFIELD(6, types::String, other_team_str);
	PBFIELD(7, types::Bool, is_yes_or_no_vote);
};*/