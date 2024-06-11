#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>

class HelpWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HelpWindow(QWidget *parent = nullptr);

private:
    void loadMarkdown(QString filepath);
signals:
};

#endif // HELPWINDOW_H
