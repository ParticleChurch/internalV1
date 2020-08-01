#pragma once

#define AUTHENTICATION_NONE        0
#define AUTHENTICATION_PROCESSING  1
#define AUTHENTICATION_COMPLETE    2
struct UserInfoStruct {
	char AuthenticationStatus = AUTHENTICATION_NONE;
	bool SubscriptionStatus = false;
	char Email[257] = "";
};

namespace Config {
	extern UserInfoStruct UserInfo;
}