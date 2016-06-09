#include "help_dialog.h"
#include "ui_help_dialog.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

// Replaces image url on html to the Qt internal URL format.
const QRegularExpression kUrlRegex("src\\s*=\\s*(\\'|\\\")");
const QString kUrlReplacement("src=\\1:/");

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
    QString html = stream.readAll();
    html = html.replace(kUrlRegex,kUrlReplacement);
    ui->textBrowser->setHtml(html);
    connect(ui->textBrowser,SIGNAL(anchorClicked(QUrl)),this,SLOT(HandleAnchor(QUrl)));
}

void HelpDialog::HandleAnchor(const QUrl &url)
{
    qDebug() << "opening" << url.fileName();
    OpenPage(url.fileName());
}
