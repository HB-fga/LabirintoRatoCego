#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QColorDialog>
#include <QLayout>
#include <iostream>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    // m_map(new Map(this)),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_ui->centralwidget->setLayout(new QVBoxLayout(this));
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_actionSaveAs_triggered()
{
    QString name = QFileDialog::getSaveFileName(this, "Save File as", "../../../assets/maps", "JSON Files (*.json)");
    std::cout << name.toStdString() << std::endl;
}




void MainWindow::on_actionOpen_triggered()
{

    QFile file;
    QString name = QFileDialog::getOpenFileName(this, "Open File", "../../../assets/maps", "JSON Files (*.json)");

    file.setFileName(name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString text = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject obj = doc.object();

    int w = obj.value(QString("width")).toInt();
    int h = obj.value(QString("height")).toInt();
    QJsonArray map = obj.value(QString("map")).toArray();

    m_ui->map->setVisibleCols(w); // Maze Width
    m_ui->map->setVisibleRows(h); // Maze Height

    for (int row = 0; row < h; ++row)
        for (int col = 0; col < w; ++col)
            m_ui->map->setCellAtGrid(row, col, cellType(map[row][col].toInt()));

    repaint();
}

