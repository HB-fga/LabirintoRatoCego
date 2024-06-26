#include "helpwindow.h"
#include <QHBoxLayout>
#include <QFile>
#include <QLabel>
#include <QTextBrowser>

HelpWindow::HelpWindow(QDialog *parent)
    : QDialog{parent}
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("Help");
    QHBoxLayout *helpLayout = new QHBoxLayout();

    this->setLayout(helpLayout);

    this->loadMarkdown(":/help.md");

}

void HelpWindow::loadMarkdown(QString filepath)
{
    QFile file = QFile(filepath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return;
    QString text = file.readAll();
    file.close();

    QTextBrowser *browser = new QTextBrowser();
    browser->setMarkdown(text);

    this->layout()->addWidget(browser);
}
