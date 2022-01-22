#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Client_Twitter.h"
#include "Log_In.h"
#include "Sign_Up.h"
#include<qtimer.h>
#include<qgraphicseffect.h>
#include<qpropertyanimation.h>
#include "User.h"
#include "Socket.h"


class Client_Twitter : public QMainWindow
{
	Q_OBJECT

public:
	using SgnUpPtr = std::unique_ptr<Sign_Up>;
	Client_Twitter(QWidget* parent = Q_NULLPTR);
	~Client_Twitter();
private slots:
	void start_up_dissapear();
	void on_Log_In_clicked();
	void on_Sign_Up_clicked();

private:
	Ui::Client_TwitterClass ui;
	Log_In* m_Log_In;
	SgnUpPtr m_Sign_Up;
	Socket* m_socket;
};
