#include "Sign_Up.h"

Sign_Up::Sign_Up(QWidget* parent)
	:QDialog(parent), ui(new Ui::Sign_Up),
	m_socket(Socket::Get()),
	m_usernameRegex(R"([A-Za-z0-9]*)")

{
	ui->setupUi(this);
	connect(ui->backMainPageBtn, SIGNAL(clicked()), this, SLOT(clickBackMainPage()));
	connect(ui->signUpBtn, SIGNAL(clicked()), this, SLOT(signUp()));
	ui->labelMessage->hide();
	ui->passwordText->setEchoMode(QLineEdit::Password);
	ui->confirmPasswordText->setEchoMode(QLineEdit::Password);
}

Sign_Up::~Sign_Up()
{
}

void Sign_Up::signUp()
{
	if (std::regex_match(ui->userNameText->text().toStdString(), m_usernameRegex))
	{
		if (ui->confirmPasswordText->text() == ui->passwordText->text()) {

			json members;
			members["username"] = ui->userNameText->text().toStdString();
			members["password"] = ui->passwordText->text().toStdString();
			members["first_name"] = ui->firstNameText->text().toStdString();
			members["last_name"] = ui->lastNameText->text().toStdString();
			members["birthday"] = ui->birthDateText->text().toStdString();
			members["command"] = Socket::Command::SIGN_UP;

			std::string receivedMessage = m_socket->SendReceiveRequest(members)["message"];

			ui->labelMessage->setText(QString::fromStdString(receivedMessage));
			ui->labelMessage->show();
		}
		else
		{
			ui->labelMessage->setText("Register failed, please reinter the password again");
			ui->labelMessage->show();
		}
	}
	else
	{
		ui->labelMessage->setText("Register failed, please enter an username formed only of letters and numbers");
		ui->labelMessage->show();
	}
}

void Sign_Up::clickBackMainPage()
{
	hide();
}
