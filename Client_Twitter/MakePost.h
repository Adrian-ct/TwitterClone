#pragma once

#include "qdialog.h"
#include "ui_MakePost.h"

class MakePost : public QDialog
{
	Q_OBJECT

public:
	MakePost(QWidget* parent = Q_NULLPTR);
	~MakePost();
	void Initialize();
	void SetParent(uint32_t parentId);
	void Reset();
private slots:
	void CreatePost();
	void CountChar(const QString& text);
signals:
	void textChanged(const QString& text);
private:
	Ui::MakePost ui;
	std::optional<uint32_t> m_parentPost;
};
