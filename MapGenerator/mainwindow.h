#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "map.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    void on_actionSaveAs_triggered();
    //void keyPressEvent(QKeyEvent *) override;

private:
    Map *m_map;
    Ui::MainWindow *m_ui;
};

#endif // MAINWINDOW_H







