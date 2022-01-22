#include "Log_In.h"
#include <iostream>

Log_In::Log_In(QWidget* parent)
	: QWidget(parent),
	parent(parent),
	m_socket(Socket::Get())
{
	ui.setupUi(this);
	QPixmap pix("../TwitterBackground");

	int h = ui.background->height();
	int w = ui.background->width();
	ui.background->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
	ui.BackToMain->setIcon(QIcon("../BackArrow"));
	showPassword = false;
	ui.wrong->hide();

}

Log_In::~Log_In()
{
}

bool Log_In::verifyData()
{
	if (ui.password_line_2->text() == "Test")
		if (ui.username_line->text() == "Test")
		{
			return true;
		}

	return false;
}

void Log_In::on_showPass_clicked()
{
	showPassword = (!showPassword);
	if (showPassword == true)
		ui.password_line_2->setEchoMode(QLineEdit::Normal);
	else ui.password_line_2->setEchoMode(QLineEdit::Password);
}

void Log_In::on_BackToMain_clicked()
{
	hide();
}

void Log_In::on_LogIn_button_clicked()
{
	ui.LogIn_button->setEnabled(false);

	json members;
	members["username"] = ui.username_line->text().toStdString();
	members["password"] = ui.password_line_2->text().toStdString();
	members["command"] = Socket::Command::LOG_IN;

	json user = m_socket->SendReceiveRequest(members);

	User* mainUser;

	//std::cout << std::setw(4) << user<<"\n";

	if (user["success"] == 1)
	{
		
		///*this->LogIn_button->setEnabled(false);*/
		mainUser = new User(user["user_id"], user["username"], user["first_name"], user["last_name"],
			util::StringToDateStruct(user["birthday"]), util::StringToTimePoint(user["account_creation_time"], "%Y-%m-%d %H:%M:%S"));
		App::Get()->SetUser(mainUser);

		//std::cout << "\n";
		//std::cout << "Birthday in user " << util::DateStructToString(mainUser->GetBirthday()) << "\n";
		//std::cout << "Date of creation in user " << util::TimePointToString(mainUser->GetCreationTime(), "%Y-%m-%d %H:%M:%S") << "\n";
		
		ui.background->clear();
		ui.BackToMain->deleteLater();
		ui.horizontalLayout->deleteLater();
		ui.horizontalLayout_2->deleteLater();
		ui.layoutWidget->deleteLater();
		ui.LogIn_button->deleteLater();
		ui.log_in_label->deleteLater();
		ui.password_label->deleteLater();
		ui.password_line_2->deleteLater();
		ui.showPass->deleteLater();
		ui.username_label->deleteLater();
		ui.username_line->deleteLater();
		ui.verticalLayout->deleteLater();
		ui.wrong->deleteLater();
		hide();
		m_Feed = new Feed();
		m_Feed->show();

		parent->hide();

	}
	else
	{
		ui.wrong->show();
		ui.LogIn_button->setEnabled(true);
		//std::cout << "Credentiale invalide!";
		//mesaj ca nu-i bine
		//buton clickable
	}
}