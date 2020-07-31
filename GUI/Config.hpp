#pragma once

struct UserInfoStruct {
	bool IsAuthenticated = false;
	bool SubscriptionStatus = false;
	char Email[256];
};

namespace Config {
	extern UserInfoStruct UserInfo;
}