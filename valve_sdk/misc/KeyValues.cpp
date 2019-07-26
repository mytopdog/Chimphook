#include "KeyValues.hpp"
#include <string>
#include <vector>
#include <Windows.h>

std::uint8_t* PatternScan(void* module, const char* signature)
{
	static auto pattern_to_byte = [](const char* pattern)
	{
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else
			{
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = pattern_to_byte(signature);
	auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return &scanBytes[i];
		}
	}
	return nullptr;
}

KeyValues* KeyValues::FindKey(const char* keyName, bool bCreate)
{
	static auto key_values_find_key = reinterpret_cast<KeyValues * (__thiscall*)(void*, const char*, bool)>(PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 83 EC 1C 53 8B D9 85 DB"));
	return key_values_find_key(this, keyName, bCreate);
}

void KeyValues::SetString(const char* keyName, const char* value)
{
	auto key = FindKey(keyName, true);
	if (key)
	{
		static auto key_values_set_string = reinterpret_cast<void(__thiscall*)(void*, const char*)>(PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01"));
		key_values_set_string(key, value);
	}
}
void KeyValues::InitKeyValues(const char* name)
{
	static auto key_values = reinterpret_cast<void(__thiscall*)(void*, const char*)>(PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 51 33 C0 C7 45 ? ? ? ? ? 56 8B F1 81 26 ? ? ? ? C6 46 03 ? 89 46 10 89 46 18 89 46 14 89 46 1C 89 46 04 89 46 08 89 46 0C FF 15 ? ? ? ? 6A 01 FF 75 08 8D 4D FC 8B 10 51 8B C8 FF 52 24 8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03"));
	key_values(this, name);
}
void KeyValues::SetUint64(const char* keyName, int value, int value2)
{
	static auto key_values_set_uint64 = reinterpret_cast<void(__thiscall*)(void*, const char*, int, int)>(PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 56 6A 01 FF 75 08"));
	key_values_set_uint64(this, keyName, value, value2);
}
const char* KeyValues::GetString(const char* keyName, const char* defaultValue)
{
	static auto key_values_get_string = reinterpret_cast<const char* (__thiscall*)(void*, const char*, const char*)>(PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08"));
	return key_values_get_string(this, keyName, defaultValue);
}
int KeyValues::GetInt(const char* keyName, int defaultValue)
{
	static auto key_values_get_int = reinterpret_cast<int(__thiscall*)(void*, const char*, int)>(PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 6A ? FF 75 08 E8 ? ? ? ? 85 C0 74 45"));
	return key_values_get_int(this, keyName, defaultValue);
}
void KeyValues::SetInt(const char* keyName, int Value)
{
	auto key_int = FindKey(keyName, true);
	if (key_int)
	{
		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(key_int) + 0xC) = Value;
		*reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(key_int) + 0x10) = 2;
	}
}