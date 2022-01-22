#include "Client_Twitter.h"

Client_Twitter::Client_Twitter(QWidget* parent)
	: QMainWindow(parent),
	m_socket(Socket::Get()),
	m_Log_In(nullptr),
	m_Sign_Up(nullptr)
{
	ui.setupUi(this);
	QPixmap pix("../TwitterBackground");

	connect(ui.Log_In, SIGNAL(clicked()), this, SLOT(on_Log_In_clicked()));
	int h = ui.background->height();
	int w = ui.background->width();
	ui.background->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
	start_up_dissapear();
}

Client_Twitter::~Client_Twitter()
{
	delete m_Log_In;
}

void Client_Twitter::on_Sign_Up_clicked()
{
	if (m_Sign_Up == nullptr)
		m_Sign_Up = std::make_unique<Sign_Up>(new Sign_Up(this));

	m_Sign_Up->show();
}

void Client_Twitter::start_up_dissapear()
{
	QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(ui.logoLabel);
	ui.logoLabel->setGraphicsEffect(effect);
	QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
	animation->setDuration(3500);
	animation->setStartValue(1.0);
	animation->setEndValue(0.0);
	animation->setEasingCurve(QEasingCurve::OutQuad);
	connect(animation, SIGNAL(finished()), this, SLOT(FadeLivePauseInfoEnded()));
	animation->start(QAbstractAnimation::DeleteWhenStopped);
	QTimer::singleShot(3500, ui.logoLabel, &QLabel::hide);
}

void Client_Twitter::on_Log_In_clicked()
{
	if (m_Log_In == nullptr)
		m_Log_In = new Log_In(this);
	m_Log_In->show();
}
