#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QColorDialog>
#include <QLayout>
#include <iostream>
#include <QFileDialog>


class MainWindowPrivate {
public:
    MainWindowPrivate(MainWindow *owner);
    ~MainWindowPrivate();

    void clear();
    void pickSize();

private slots:
    void ColorPick_triggered();

    void on_centralWidget_customContextMenuRequested(const QPoint &pos);

    void on_sizeIncrease_pressed();

public slots:
    void on_actionPick_triggered();

private:
    Map *m_map;
    MainWindow *m_Owner;
    Ui::MainWindow *m_ui;
};

MainWindowPrivate::MainWindowPrivate(MainWindow *owner) :
    m_Owner(owner),
    m_map(new Map(owner)),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(m_Owner);
    m_ui->centralwidget->setLayout(new QVBoxLayout(m_Owner));
    m_ui->centralwidget->layout()->addWidget(m_map);
}

void MainWindowPrivate::on_actionPick_triggered()
{
    m_map->setBrushSize(50);
    QColor color = QColorDialog::getColor(
        m_map->drawColor(),
        m_Owner,
        QString("Select a draw color"),
        QColorDialog::ShowAlphaChannel
        );
    m_map->setBrushColor(color);
}

void MainWindowPrivate::clear()
{
    m_map->clear();
}

MainWindowPrivate::~MainWindowPrivate(){
    delete m_ui;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new MainWindowPrivate(this))
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::colorPicked()
{
    ui->on_actionPick_triggered();
}

void MainWindow::clear()
{
    ui->clear();
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString name = QFileDialog::getSaveFileName(this, "Save File as", "../../../assets/maps", "JSON Files (*.json)");
    std::cout << name.toStdString() << std::endl;
}

// void MainWindow::keyPressEvent(QKeyEvent *event){
//     std::cout << "KeyEvent\n";
    // if(event->key() == Qt::Key_Right)
    //     size++;
    // else if(event->key() == Qt::Key_Left)
    //     size--;
// }


