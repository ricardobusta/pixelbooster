#include "help_dialog.h"
#include "ui_help_dialog.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    OpenPage("index.html");
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::OpenPage(const QString &file_name)
{
    QFile file(":/"+file_name);
    file.open(QFile::ReadOnly|QFile::Text);
    QTextStream stream(&file);
    ui->textBrowser->setHtml(stream.readAll());
    connect(ui->textBrowser,SIGNAL(anchorClicked(QUrl)),this,SLOT(HandleAnchor(QUrl)));
}

void HelpDialog::HandleAnchor(const QUrl &url)
{
    OpenPage(url.fileName());
}
