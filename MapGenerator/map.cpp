#include "map.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRgb>
#include <QDebug>
#include <QPainterPath>
#include <QCursor>

Map::Map(QWidget *parent) : QWidget(parent)
{
    rows = 20;
    columns = 20;
    visibleRows = 10;
    visibleCols = 10;
    startPos = QPoint(0, 0);
    endPos = QPoint(9, 9);
    selectedCell = cellType::Wall;

    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<columns; j++){
            Cell* cell = new Cell;
            if(i >= visibleRows || j >= visibleCols)
                cell->setVisible(false);
            cellGrid.addWidget(cell, i, j);
        }
    }

    Cell* startCell = getCell(startPos.x(), startPos.y());
    startCell->setCellType(cellType::Start);
    Cell* endCell = getCell(endPos.x(), endPos.y());
    endCell->setCellType(cellType::End);

    this->cellGrid.setSpacing(0);
    this->setLayout(&cellGrid);
}

Map::~Map()
{

}

void Map::changeCellTypeWall(){
    selectedCell = cellType::Wall;
}

void Map::changeCellTypeStart(){
    selectedCell = cellType::Start;
}

void Map::changeCellTypeEnd(){
    selectedCell = cellType::End;
}

void Map::changeCellTypeDecision(){
    selectedCell = cellType::Decision;
}

void Map::changeCellTypePath(){
    selectedCell = cellType::Path;
}

Cell* Map::getCell(int x, int y)
{
    return qobject_cast<Cell*>(cellGrid.itemAtPosition(x, y)->widget());
}

Cell* Map::getCell(QMouseEvent *event)
{
    QWidget * const widget = childAt(event->pos());
    if (widget) return qobject_cast<Cell*>(widget);
    else return NULL;
}

void Map::mousePressEvent(QMouseEvent *event)
{
    Cell* cell = getCell(event);
    setGridCellType(cell);
}

void Map::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        Cell* cell = getCell(event);
        setGridCellType(cell);
    }
}

void Map::setGridCellType(Cell* cell)
{
    if (cell != NULL) {
        cell->setCellType(selectedCell);
        if(selectedCell == cellType::Start)
        {
            Cell* oldStartCell = getCell(startPos.x(), startPos.y());
            oldStartCell->setCellType(cellType::Path);

            int newCellRow = cellGrid.indexOf(cell)/rows;
            int newCellCol = cellGrid.indexOf(cell)%rows;

            startPos.setX(newCellRow);
            startPos.setY(newCellCol);
        }
        else if(selectedCell == cellType::End)
        {
            Cell* oldEndCell = getCell(endPos.x(), endPos.y());
            oldEndCell->setCellType(cellType::Path);

            int newCellRow = cellGrid.indexOf(cell)/rows;
            int newCellCol = cellGrid.indexOf(cell)%rows;

            endPos.setX(newCellRow);
            endPos.setY(newCellCol);
        }
        repaint();
    }
}

// void Map::keyPressEvent( QKeyEvent* event ) {
//     std::cout << "KeyEvent Enter\n";
//     switch ( event->key() ) {
//     case Qt::Key_Right:
//         std::cout << "KeyEvent Right\n";
//         size++;
//         break;
//     case Qt::Key_Left:
//         std::cout << "KeyEvent Left\n";
//         size--;
//         break;
//     default:
//         event->ignore();
//         break;
//     }
// }

void Map::increaseRows(){
    if(visibleRows+1 > rows) return;
    for(int j=0;j<visibleCols;j++){
        Cell* cell = getCell(visibleRows, j);
        cell->setVisible(true);
    }
    visibleRows++;

    repaint();
}

void Map::increaseCols(){
    if(visibleCols+1 > columns) return;
    for(int i=0;i<visibleRows;i++){
        Cell* cell = getCell(i, visibleCols);
        cell->setVisible(true);
    }
    visibleCols++;

    repaint();
}

void Map::decreaseRows(){
    if(visibleRows-1 < 2) return;
    for(int j=0;j<visibleCols;j++){
        Cell* cell = getCell(visibleRows-1, j);
        cell->setVisible(false);
    }
    visibleRows--;

    repaint();
}

void Map::decreaseCols(){
    if(visibleCols-1 < 2) return;
    for(int i=0;i<visibleRows;i++){
        Cell* cell = getCell(i, visibleCols-1);
        cell->setVisible(false);
    }
    visibleCols--;

    repaint();
}

void Map::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    paintGrid(&painter);

}

void Map::paintGrid(QPainter* painter){
    int cellSize = 30;
    for(int i=0;i<rows;i++){
        for(int j=0;j<columns;j++){
            Cell* cell = getCell(i, j);

            if(cell->isVisible())
                painter->drawPixmap(cell->pos(), cell->getCellImage());
        }
    }
}
