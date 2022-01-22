#include "Client_Twitter.h"
#include <QtWidgets/QApplication>
#include "nlohmann.hpp"
#include "TcpSocket.h"
#include <string>
#include <iostream>
#include "Socket.h"
#include "Post.h"
#include <regex>

int main(int argc, char* argv[])
{
	//Debug for Loading User's Posts
	/*Socket::Get();
	PostsContainer profile;
	profile.LoadPosts(1);*/

	//std::regex m_usernameRegex(R"([A-Za-z0-9]*)");
	//std::string s;
	//while (s != "end")
	//{
	//	std::cin >> s;
	//	if (std::regex_match(s, m_usernameRegex))
	//		std::cout << "ok\n";
	//	else std::cout << "not ok\n";
	//}


	QApplication a(argc, argv);

	//TcpSocket socket;
	//auto instance = Socket::Get();

	Client_Twitter w;
	w.show();

	return a.exec();
}
