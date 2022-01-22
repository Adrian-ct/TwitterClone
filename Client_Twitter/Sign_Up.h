#pragma once

#include<QDialog>
#include "ui_Sign_Up.h"
#include "nlohmann.hpp"
#include "Socket.h"
#include <regex>
using namespace nlohmann;

class Sign_Up : public QDialog
{
	
	Q_OBJECT
public:

	Sign_Up(QWidget* parent = Q_NULLPTR);
	~Sign_Up();


private slots:
   void clickBackMainPage();
   void signUp();

private:
	Ui::Sign_Up *ui;
	bool showPassword;
	Socket* m_socket;
	std::regex m_usernameRegex;
};
