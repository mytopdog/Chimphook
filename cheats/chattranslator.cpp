#include "chattranslator.h"
#include "../hooks.hpp"
#include "../settings.hpp"
#include "json.hpp"
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

std::string join(const std::vector<std::string>& v, const std::string& delimiter = ",")
{
	std::string out;
	if (auto i = v.begin(), e = v.end(); i != e)
	{
		out += *i++;
		for (; i != e; ++i) out.append(delimiter).append(*i);
	}
	return out;
}

EChatTranslatorResult chat_translator::client_cmd(const char* cmd, std::string& buf)
{
	if (!Settings::Misc::TranslateBot)
		return ECTR_None;

	if (auto command = std::string_view(cmd); !command.compare(0, 3, "say") && strstr(cmd, "\""))
	{
		std::string tmp = std::string(strstr(cmd, "\""));
		std::string msg = tmp.substr(1, tmp.size() - 2);

		bool is_say_team = command[3] == '_';
		
		std::vector<std::string> args = Utils::Split(msg, " ");

		if (args.size() > 2)
		{
			if (args[0] == "tsay")
			{
				std::string lang = args[1];
				args.erase(args.begin());
				args.erase(args.begin());

				std::string translated = translate(join(args, " "), lang);

				buf = (is_say_team ? std::string("say_team \"") : std::string("say \"")) + translated + std::string("\"");
				return ECTR_Translate;
			}
		}
	}
	return ECTR_None;
}

bool chat_translator::dispatch_user_message(int entity_id, const std::string& msg,
	void* ecx, int type, unsigned int a3, unsigned int length, const void* msg_data)
{
	if (!Settings::Misc::TranslateBot)
		return false;

	C_BasePlayer* chatter = C_BasePlayer::GetPlayerByIndex(entity_id);

	Utils::ConsolePrint(msg.c_str());

	std::string tr = "null";
	std::string translated = translate(msg, "en", &tr);

	if (!chatter)
		return false;

	player_info_t info = chatter->GetPlayerInfo();

	if (translated != msg)
	{
		std::string joined;
		joined += " \x03[";
		joined += info.szName;
		joined += ", ";
		joined += tr;
		joined += "]: \x0A";
		joined += translated;

		static auto oDispatchUserMessage = Hooks::hlclient_hook.get_original<Hooks::tDispatchUserMessage>(index::DispatchUserMessage);
		oDispatchUserMessage(ecx, type, a3, length, msg_data);

		g_HUDChat->ChatPrintf(0, 0, joined.c_str());

		return true;
	}

	return false;
}

std::string chat_translator::translate(const std::string & text, const std::string & lang, std::string* tr)
{
	// https://github.com/nlohmann/json
	using json = nlohmann::json;
	const std::string url = "https://translate.yandex.net/api/v1.5/tr.json/translate";
	const std::string key = "trnsl.1.1.20190709T200408Z.652cfb3b6a75a9e2.ac95fc1998aba2868afcf04dedf1fa0ce586da7e";

	auto _url = url + std::string("?key=") + key;
	_url += std::string("&text=") + text;
	_url += std::string("&lang=") + lang;

	if (auto connect = InternetOpenA("GoogleChrome", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0); connect)
	{
		if (auto address = InternetOpenUrlA(connect, _url.c_str(), NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0); address)
		{
			char data_received[1024];
			DWORD number_of_bytes_read = 0;

			if (InternetReadFile(address, data_received, 1024, &number_of_bytes_read) && number_of_bytes_read)
			{
				std::string output(data_received, number_of_bytes_read);
				InternetCloseHandle(address);
				InternetCloseHandle(connect);

				auto json = json::parse(output.c_str());
				//std::cout << json.dump() << std::endl;
				auto code = json["code"].get<int>();
				if (code == 200) {
					if (tr) *tr = json["lang"].get<std::string>();
					return json["text"][0].get<std::string>();
				}
			}
		}
		else
		{
			InternetCloseHandle(connect);
		}
	}

	return std::string();
}