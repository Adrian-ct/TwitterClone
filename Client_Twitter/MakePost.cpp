#include "MakePost.h"
#include "App.h"

MakePost::MakePost(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.makePostButton, SIGNAL(clicked()), this, SLOT(CreatePost()));
	ui.usernameLabel->setText(QString::fromStdString(App::Get()->GetUser()->GetUsername()));
	ui.nameInitLabel->setText(QString::fromStdString(App::Get()->GetUser()->GetUsername()).at(0).toUpper());
	//connect(ui.makePostButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.makePostText, &QLineEdit::textChanged, this, &MakePost::CountChar);
	connect(ui.makePostText, &QLineEdit::textChanged, this, &MakePost::textChanged);
	ui.makePostText->setMaxLength(140);
	CountChar("");
}
void MakePost::CreatePost() {
	App::GetUser()->MakePost(ui.makePostText->text().toStdString(), m_parentPost);
	
	ui.makePostText->setText("");
	close();
}
MakePost::~MakePost()
{
}


void MakePost::CountChar(const QString& text)
{
	QString text_label = QString("%1/%2").arg(text.size()).arg(ui.makePostText->maxLength());
	ui.char_counter->setText(text_label);
}

void MakePost::Initialize()
{

}

void MakePost::SetParent(uint32_t parentId)
{
	m_parentPost = parentId;
}

void MakePost::Reset()
{   
	ui.makePostText->clear();
	m_parentPost = std::nullopt;
}


