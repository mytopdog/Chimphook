#pragma once
#include "utils.hpp"
#include "../valve_sdk/sdk.hpp"
#include <vector>

struct paint_kit
{
	int id;
	std::string name;
	bool operator < (const paint_kit& other)
	{
		return name < other.name;
	}
};

class CKitParser
{
public:
	void setup() noexcept;
};

extern std::vector<paint_kit> parser_skins;
extern std::vector<paint_kit> parser_gloves;
extern CKitParser KitParser;