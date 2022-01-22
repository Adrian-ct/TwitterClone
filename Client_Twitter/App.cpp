#include "App.h"

App::App()
    :m_user(nullptr)
{
}

void App::SetUser(User* newUser)
{
    s_instance->m_user = newUser;
}

App* App::Get()
{
    if (!s_instance)
    {
        s_instance = new App;
    }
    return s_instance;
}

User* App::GetUser()
{
    return Get()-> GetUserImpl();
}

User* App::GetUserImpl()
{
    return m_user;
}

App* App::s_instance = nullptr;