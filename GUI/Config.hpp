#pragma once
/*
	Web API shit
*/
#define LOGIN_FLAG_SERVER_ACKNOWLEDGE       1 << 0  // does the server recognize this as a valid request
#define LOGIN_FLAG_LOGIN_VALID              1 << 1  // was the sent login valid
#define LOGIN_FLAG_ACCOUNT_PAID             1 << 2  // is the account paid or free
#define LOGIN_FLAG_ACCOUNT_ALREADY_IN_USE   1 << 3  // is the cheat being used elsewhere (this is someone trying to share an account with a friend)
#define LOGIN_FLAG_ACCOUNT_IS_DEVELOPER     1 << 4  // is this login the dev@a4g4.com account
#define LOGIN_FLAG_BANNED                   1 << 5  // is this account banned (do something like: print "fuck you" then crash csgo, lol)

#define AUTH_STATUS_NONE                    0 // not logged in
#define AUTH_STATUS_PROCESSING              1 // login is processing
#define AUTH_STATUS_COMPLETE                2 // logged in

struct UserInfoStruct {
	char AuthStatus = AUTH_STATUS_NONE;
	bool Paid = false;
	bool Developer = false;
	int UserID = 69; // todo
	char Email[257] = "";
};


/*
	Actual user config
*/
namespace Config {
	extern UserInfoStruct UserInfo;
}