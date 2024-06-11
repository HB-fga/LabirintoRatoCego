#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "helpwindow.h"

#include <QColorDialog>
#include <QLayout>
#include <QFileDialog>
#include <QFrame>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    // m_map(new Map(this)),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_ui->centralwidget->setLayout(new QVBoxLayout(this));
    this->setWindowTitle("Criador de Labirinto");

    // ToolBar
    QWidget* toolbarWidget = new QWidget();
    QGridLayout* toolbarLayout = new QGridLayout(toolbarWidget);

    // TODO: Verificar se é possível fazer de outra forma
    toolbarLayout->addWidget(makeButton("Path", SLOT(changeCellTypePath()), QKeySequence(Qt::Key_P)), 0, 0);
    toolbarLayout->addWidget(makeButton("Decision", SLOT(changeCellTypeDecision()), QKeySequence(Qt::Key_D)), 0, 1);
    toolbarLayout->addWidget(makeButton("Start", SLOT(changeCellTypeStart()), QKeySequence(Qt::Key_S)), 1, 0);
    toolbarLayout->addWidget(makeButton("End", SLOT(changeCellTypeEnd()), QKeySequence(Qt::Key_E)), 1, 1);
    toolbarLayout->addWidget(makeButton("Wall", SLOT(changeCellTypeWall()), QKeySequence(Qt::Key_W)), 2, 0);

    // QSpacerItem *spacer;
    // spacer = new QSpacerItem(0, 50);
    // toolbarLayout->addItem(spacer, 3, 0);

    toolbarLayout->addItem(new QSpacerItem(0, 50), 3, 0);

    QFrame *line;
    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    toolbarLayout->addWidget(line, 3, 0, 1, 2);

    toolbarLayout->addWidget(makeButton("Expand-Vertical", SLOT(increaseRows()), QKeySequence(Qt::Key_Up)), 4, 0);
    toolbarLayout->addWidget(makeButton("Collapse-Vertical", SLOT(decreaseRows()), QKeySequence(Qt::Key_Down)), 4, 1);
    toolbarLayout->addWidget(makeButton("Expand-Horizontal", SLOT(increaseCols()), QKeySequence(Qt::Key_Right)), 5, 0);
    toolbarLayout->addWidget(makeButton("Collapse-Horizontal", SLOT(decreaseCols()), QKeySequence(Qt::Key_Left)), 5, 1);

    toolbarWidget->setLayout(toolbarLayout);
    m_ui->toolBar->addWidget(toolbarWidget);

    // Menu Help
    QAction *helpAction = new QAction("Help");
    helpAction->setShortcut(QKeySequence(Qt::Key_H));
    // helpAction->setToolTip("Help (H)");
    connect(helpAction, SIGNAL(triggered()), this, SLOT(helpWindow()));

    m_ui->menubar->addAction(helpAction);

    this->mapName = "";
}

MainWindow::~MainWindow()
{

}

QPushButton* MainWindow::makeButton(QString name, const char* slot, QKeySequence key)
{
    QPushButton* button = new QPushButton;

    QPalette pal = button->palette();
    pal.setColor(QPalette::Button, QColor(Qt::white));
    button->setPalette(pal);

    button->setIcon(QIcon("../../assets/buttons/" + name.toLower() + ".png"));
    button->setShortcut(key);
    button->setToolTip(name + " (" + button->shortcut().toString() + ")");

    button->setIconSize(QSize(52, 52));
    button->setFixedSize(64, 64);
    connect(button, SIGNAL(clicked()), m_ui->map, slot);
    return button;
}


void MainWindow::helpWindow()
{
    HelpWindow *helpWindow = new HelpWindow();
    helpWindow->showMaximized();
}

void MainWindow::saveMap()
{
    QFile file(this->mapName);
    // TODO: Tratamento de erro
    if(!file.open(QIODevice::WriteOnly))
    {
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
