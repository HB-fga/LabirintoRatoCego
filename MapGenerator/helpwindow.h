#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>

class HelpWindow : public QDialog
{
    Q_OBJECT
public:
    explicit HelpWindow(QDialog *parent = nullptr);

private:
    void loadMarkdown(QString filepath);
signals:
};

#endif // HELPWINDOW_H
