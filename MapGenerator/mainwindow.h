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
    void on_actionSaveAs_triggered();
    void on_actionOpen_triggered();
    void helpWindow();
    void on_actionSave_triggered();
    void changeCell();

private:
    QString mapName;
    QPushButton* makeButton(QString name, const char* slot, QKeySequence key, bool selectable);
    bool checkSave();
    void saveMap();
    Map *m_map;
    Ui::MainWindow *m_ui;
    QPushButton* selectedButton;
};

#endif // MAINWINDOW_H







