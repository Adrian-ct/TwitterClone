#include "Socket.h"
#include <string>
#include <iostream>

//TcpSocket Socket::m_tcpSocket = TcpSocket();

Socket* Socket::Get()
{
	if (s_instance == nullptr)
		s_instance = new Socket();
	return s_instance;
}


json Socket::SendReceiveRequest(const json& messageJson)
{
	std::string message = messageJson.dump();
	bool result = m_tcpSocket.Send(message.c_str(), message.size());
	std::array<char, 10000> receiveBuffer;
	int received;
	json j;

	if (result)
	{
		m_tcpSocket.Receive(receiveBuffer.data(), receiveBuffer.size(), received);
		j = json::parse(std::string(receiveBuffer.begin(), receiveBuffer.begin() + received));
		//std::cout << std::string(receiveBuffer.begin(), receiveBuffer.begin() + received);
	}
	return j;
}

Socket::Socket()
{

	m_tcpSocket.Connect("localhost", 27015);
	//m_tcpSocket.Connect("25.59.39.211", 27015);

}

Socket* Socket::s_instance = 0;

