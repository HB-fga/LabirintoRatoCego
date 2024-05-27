#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QColorDialog>
#include <QLayout>
#include <iostream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_map(new Map(this)),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_ui->centralwidget->setLayout(new QVBoxLayout(this));
}

MainWindow::~MainWindow()
{

}

void MainWindow::colorPicked()
{
    m_map->setBrushSize(50);
    QColor color = QColorDialog::getColor(
        m_map->drawColor(),
        this,
        QString("Select a draw color"),
        QColorDialog::ShowAlphaChannel);
    m_map->setBrushColor(color);
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString name = QFileDialog::getSaveFileName(this, "Save File as", "../../../assets/maps", "JSON Files (*.json)");
    std::cout << name.toStdString() << std::endl;
}



