#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPushButton>
#include "map.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void actionSaveAs();
    void actionOpen();
    void helpWindow();
    void actionSave();

private:
    QString mapName;
    QPushButton* makeButton(QString name, const char* slot, QKeySequence key);
    void saveMap();
    Map *m_map;
    Ui::MainWindow *m_ui;
};

#endif // MAINWINDOW_H







