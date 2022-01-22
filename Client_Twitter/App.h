#pragma once
#include "User.h"

class App
{

public:
	App(const App& other) = delete;
	void operator=(const App& other) = delete;

	static App* Get();
	static User* GetUser();
	static void SetUser(User* newUser);

private:
	User* GetUserImpl();
	App();
	static App* s_instance;

private:
	User* m_user;

};

