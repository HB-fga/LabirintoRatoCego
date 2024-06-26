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
#include <QMessageBox>
#include <QCryptographicHash>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    this->setWindowTitle("Criador de Labirinto");

    // ToolBar
    QWidget* toolbarWidget = new QWidget();
    QGridLayout* toolbarLayout = new QGridLayout(toolbarWidget);

    toolbarLayout->addWidget(makeButton("Path", SLOT(changeCellTypePath()), QKeySequence(Qt::Key_P), true), 0, 0);
    toolbarLayout->addWidget(makeButton("Decision", SLOT(changeCellTypeDecision()), QKeySequence(Qt::Key_D), true), 0, 1);
    toolbarLayout->addWidget(makeButton("Start", SLOT(changeCellTypeStart()), QKeySequence(Qt::Key_S), true), 1, 0);
    toolbarLayout->addWidget(makeButton("End", SLOT(changeCellTypeEnd()), QKeySequence(Qt::Key_E), true), 1, 1);

    QPushButton* selectedButton = makeButton("Wall", SLOT(changeCellTypeWall()), QKeySequence(Qt::Key_W), true);
    QPalette pal = selectedButton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::blue));
    selectedButton->setPalette(pal);
    toolbarLayout->addWidget(selectedButton, 2, 0);
    this->selectedButton = selectedButton;

    toolbarLayout->addItem(new QSpacerItem(0, 50), 3, 0);

    QFrame *line;
    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    toolbarLayout->addWidget(line, 3, 0, 1, 2);

    toolbarLayout->addWidget(makeButton("Expand-Vertical", SLOT(increaseRows()), QKeySequence(Qt::Key_Up), false), 4, 0);
    toolbarLayout->addWidget(makeButton("Collapse-Vertical", SLOT(decreaseRows()), QKeySequence(Qt::Key_Down), false), 4, 1);
    toolbarLayout->addWidget(makeButton("Expand-Horizontal", SLOT(increaseCols()), QKeySequence(Qt::Key_Right), false), 5, 0);
    toolbarLayout->addWidget(makeButton("Collapse-Horizontal", SLOT(decreaseCols()), QKeySequence(Qt::Key_Left), false), 5, 1);

    toolbarWidget->setLayout(toolbarLayout);
    m_ui->toolBar->addWidget(toolbarWidget);

    // Menu Help
    QAction *helpAction = new QAction("&Help");
    helpAction->setShortcut(QKeySequence(Qt::Key_H));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(helpWindow()));

    m_ui->menubar->addAction(helpAction);

    this->mapName = "";
    //qDebug() << qgetenv("HOME");
}

MainWindow::~MainWindow()
{

}

QPushButton* MainWindow::makeButton(QString name, const char* slot, QKeySequence key, bool selectable)
{
    QPushButton* button = new QPushButton;

    QPalette pal = button->palette();
    pal.setColor(QPalette::Button, QColor(Qt::white));
    button->setPalette(pal);
    button->setIcon(QIcon(":/button/" + name.toLower()));
    button->setShortcut(key);
    button->setToolTip(name + " (" + button->shortcut().toString() + ")");

    button->setIconSize(QSize(52, 52));
    button->setFixedSize(64, 64);
    connect(button, SIGNAL(clicked()), m_ui->map, slot);

    if(selectable)
    {
        connect(button, SIGNAL(clicked()), this, SLOT(changeCell()));
    }
    return button;
}

void MainWindow::changeCell()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());

    QPalette oldPal = this->selectedButton->palette();
    oldPal.setColor(QPalette::Button, QColor(Qt::white));
    this->selectedButton->setPalette(oldPal);

    QPalette pal = clickedButton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::blue));
    clickedButton->setPalette(pal);

    this->selectedButton = clickedButton;
    repaint();
}


void MainWindow::helpWindow()
{
    HelpWindow *helpWindow = new HelpWindow();
    helpWindow->exec();
}

void MainWindow::saveMap()
{
    QByteArray json = QJsonDocument(m_ui->map->getJSON()).toJson();

    if(m_ui->map->isConnected())
    {
        QFile file(this->mapName);
        // TODO: Tratamento de erro
        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "File open error";
            return;
        }

        QCryptographicHash hash = QCryptographicHash(QCryptographicHash::Md5);
        hash.addData(json);
        QByteArray hashResult = hash.result().toHex();

        file.write(hashResult);
        file.write("\n");

        file.write(json);
        file.close();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setFixedSize(500,200);
        messageBox.critical(0, "Não é possível salvar este labirinto", "O Labirinto não possui todos os pontos de decisão conectados");
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    if(!this->checkSave()) return;
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RatoCego";
    if (!QDir(path).exists()) QDir().mkdir(path);
    QFileDialog saveDialog = QFileDialog (this, "Save File as", path, "RCMAP Files (*.rcmap)");
    saveDialog.setDefaultSuffix(".rcmap");
    saveDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveDialog.exec();

    QStringList result = saveDialog.selectedFiles();

    if(result.isEmpty()) return;

    QString name = result.front();

    this->mapName = name;
    this->saveMap();
    this->setWindowTitle(this->mapName);
}

void MainWindow::on_actionOpen_triggered()
{
    QFile file;
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RatoCego";
    if (!QDir(path).exists()) QDir().mkdir(path);
    QString name = QFileDialog::getOpenFileName(this, "Open File", path, "RCMAP Files (*.rcmap)");
    if(name.isEmpty() or name.isNull()) return;

    file.setFileName(name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray checksum = file.readLine();
    checksum.chop(1);

    // Le o json
    QString text = file.readAll();
    file.close();

    // Calcula o hash do json
    QCryptographicHash hash = QCryptographicHash(QCryptographicHash::Md5);
    hash.addData(text.toUtf8());

    // Check Sum
    if(checksum != hash.result().toHex())
    {
        QMessageBox messageBox;
        messageBox.setFixedSize(500,200);
        messageBox.critical(0, "Arquivo Corrompido", "Não é possivel carregar o labirinto. O arquivo está corrompido");
        return;
    }

    this->mapName = name;
    this->setWindowTitle(this->mapName);

    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject obj = doc.object();

    int w = obj.value(QString("width")).toInt();
    int h = obj.value(QString("height")).toInt();

    QJsonArray map = obj.value(QString("map")).toArray();

    m_ui->map->setVisibleCols(w); // Maze Width
    m_ui->map->setVisibleRows(h); // Maze Height

    m_ui->map->clearGrid();

    for(int row = 0; row < h; row++)
    {
        for (int col = 0; col < w; col++)
        {
            m_ui->map->setCellAtGrid(row, col, cellType(map[row][col].toInt()));

            if( cellType( map[row][col].toInt() ) == cellType::Start )
                m_ui->map->setStartPos(col, row);

            if( cellType( map[row][col].toInt() ) == cellType::End )
                m_ui->map->setEndPos(col, row);
        }
    }

    repaint();
}

void MainWindow::on_actionSave_triggered()
{

    if(this->mapName.size() > 0)
    {
        if(!this->checkSave()) return;
        this->saveMap();
    }
    else
    {
        this->on_actionSaveAs_triggered();
    }
}

bool MainWindow::checkSave()
{
    bool canSave = true;
    QPoint start = m_ui->map->getStartPos();
    QPoint end = m_ui->map->getEndPos();
    int visibleCols = m_ui->map->getVisibleCols();
    int visibleRows = m_ui->map->getVisibleRows();

    QMessageBox messageBox;
    messageBox.setFixedSize(500,200);

    // qDebug() << "##### CHECK SAVE DEBUG #####";
    // qDebug() << "START CELL TYPE: " << (int)m_ui->map->getCell(start.y(), start.x())->getCellType();
    // qDebug() << "START POS: " << start.x() << start.y();
    // qDebug() << "END CELL TYPE: " << (int)m_ui->map->getCell(end.y(), end.x())->getCellType();
    // qDebug() << "END POS: " << end.x() << end.y();
    // qDebug() << "COLS + ROWS: " << visibleCols << visibleRows;

    if(m_ui->map->getCell(start.y(), start.x())->getCellType() != cellType::Start)
    {
        messageBox.critical(0, "Não é possível salvar este labirinto", "O Labirinto não possui célula de entrada (1)");
        canSave = false;
    }
    else if(start.x() >= visibleCols || start.y() >= visibleRows)
    {
        messageBox.critical(0, "Não é possível salvar este labirinto", "O Labirinto não possui célula de entrada (2)");
        canSave = false;
    }
    else if(m_ui->map->getCell(end.y(), end.x())->getCellType() != cellType::End)
    {
        messageBox.critical(0, "Não é possível salvar este labirinto", "O Labirinto não possui célula de saída (3)");
        canSave = false;
    }
    else if(end.x() >= visibleCols || end.y() >= visibleRows)
    {
        messageBox.critical(0, "Não é possível salvar este labirinto", "O Labirinto não possui célula de saída (4)");
        canSave = false;
    }

    return canSave;
}
