#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class MainWindowPrivate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    //bool eventFilter(QObject *obj, QEvent *event);

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    void colorPicked();
    void clear();
    void on_actionSaveAs_triggered();
    //void keyPressEvent(QKeyEvent *) override;

    // void sizePickTriggered();

private:
    //Ui::MainWindow *ui;
    MainWindowPrivate *ui;
};

#endif // MAINWINDOW_H







