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

    // ToolBar
    QWidget* toolbarWidget = new QWidget();
    QGridLayout* toolbarLayout = new QGridLayout(toolbarWidget);

    // TODO: Verificar se é possível fazer de outra forma
    toolbarLayout->addWidget(makeButton("Path", SLOT(changeCellTypePath()), QKeySequence(Qt::Key_P)), 0, 0);
    toolbarLayout->addWidget(makeButton("Decision", SLOT(changeCellTypeDecision()), QKeySequence(Qt::Key_D)), 0, 1);
    toolbarLayout->addWidget(makeButton("Start", SLOT(changeCellTypeStart()), QKeySequence(Qt::Key_S)), 1, 0);
    toolbarLayout->addWidget(makeButton("End", SLOT(changeCellTypeEnd()), QKeySequence(Qt::Key_E)), 1, 1);
    toolbarLayout->addWidget(makeButton("Wall", SLOT(changeCellTypeWall()), QKeySequence(Qt::Key_W)), 2, 0);
    toolbarWidget->setLayout(toolbarLayout);
    m_ui->toolBar->addWidget(toolbarWidget);

    this->mapName = "";

}

MainWindow::~MainWindow()
{

}

QPushButton* MainWindow::makeButton(QString name, const char* slot, QKeySequence key)
{
    QPushButton* button = new QPushButton;
    button->setIcon(QIcon("../../assets/" + name.toLower() + ".png"));
    button->setShortcut(key);
    button->setToolTip(name + " (" + button->shortcut().toString() + ")");
    // TODO: Verificar tamanhos depois de trocar os ícones
    button->setIconSize(QSize(64, 64));
    button->setFixedSize(64, 64);
    connect(button, SIGNAL(clicked()), m_ui->map, slot);
    return button;
}

void MainWindow::saveMap(){
    QFile file(this->mapName);
    // TODO: Tratamento de erro
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "File open error";
        return;
    }

    file.write(QJsonDocument(m_ui->map->getJSON()).toJson());
    file.close();
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString name = QFileDialog::getSaveFileName(this, "Save File as", "../../../assets/maps", "JSON Files (*.json)");
    if(name.isEmpty() or name.isNull()) return;
    this->mapName = name;
    this->saveMap();
    this->setWindowTitle(this->mapName);
}

void MainWindow::on_actionOpen_triggered()
{

    QFile file;
    QString name = QFileDialog::getOpenFileName(this, "Open File", "../../../assets/maps", "JSON Files (*.json)");
    if(name.isEmpty() or name.isNull()) return;
    this->mapName = name;
    this->setWindowTitle(this->mapName);

    file.setFileName(this->mapName);
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


void MainWindow::on_actionSave_triggered()
{
    if(this->mapName.size() > 0) this->saveMap();
    else this->on_actionSaveAs_triggered();
}

