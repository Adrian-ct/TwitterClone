#pragma once

#include <QWidget>
#include "ui_Log_In.h"
#include "Feed.h"
#include "Socket.h"
#include "App.h"

class Log_In : public QWidget
{
	Q_OBJECT

public:
	Log_In(QWidget *parent = Q_NULLPTR);
	~Log_In();
	bool verifyData();

private slots:

	void on_LogIn_button_clicked();
	void on_showPass_clicked();
	void on_BackToMain_clicked();
	
private:
	Ui::Log_In ui;
	Feed* m_Feed;
	bool showPassword;
	Socket* m_socket;
	QWidget* parent;
};
