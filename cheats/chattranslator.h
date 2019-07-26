#include <deque>
#include "../valve_sdk/sdk.hpp"
#include "../valve_sdk/csgostructs.hpp"

enum EChatTranslatorResult
{
	ECTR_None,
	ECTR_DontSend,
	ECTR_Translate
};

class chat_translator : public Singleton<chat_translator>
{
public:
	EChatTranslatorResult client_cmd(const char* cmd, std::string& buf);

	bool dispatch_user_message(int entity_id, const std::string& msg, void* ecx,  int type, unsigned int a3, unsigned int length, const void* msg_data);

	std::string translate(const std::string& text, const std::string& lang, std::string* tr = nullptr );

private:
	int cur;

	struct msg_t
	{
		C_BasePlayer* player;
		std::string msg;
	};

	std::vector<msg_t> msgs;
};