#pragma once
#include "TcpSocket.h"
#include "nlohmann.hpp"
#include <memory>
//#include <qstringview.h>
using namespace nlohmann;
class Socket
{
public:
	enum class Command : uint8_t
	{
		SIGN_UP,
		LOG_IN,
		POST,
		COMMENT,
		REACT,
		FOLLOW,
		UNFOLLOW,
		IS_FOLLOWING,
		LOAD_POSTS,
		POSTS_WITH_HASHTAG,
		FEED,
		USERNAME_SEARCH,
		GET_USER
	};

	Socket(const Socket& other) = delete;
	void operator=(const Socket& other) = delete;

	static Socket* Get();

	json SendReceiveRequest(const json& message);


private:
	Socket();
	static Socket* s_instance;

private:
	TcpSocket m_tcpSocket;
};

