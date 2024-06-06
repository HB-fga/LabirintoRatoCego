#include "map.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRgb>
#include <QDebug>
#include <QPainterPath>
#include <QCursor>
#include <QJsonArray>

Map::Map(QWidget *parent) : QWidget(parent)
{
    rows = 30;
    columns = 30;
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
    for(int i=0;i<rows;i++){
        for(int j=0;j<columns;j++){
            Cell* cell = getCell(i, j);
            if(cell->isVisible())
                painter->drawPixmap(cell->pos(), cell->getCellImage());
        }
    }
}

void Map::setVisibleCols(int w){
    while(w > this->visibleCols)increaseCols();
    while(w < this->visibleCols) decreaseCols();
}

void Map::setVisibleRows(int h){
    while(h > this->visibleRows) increaseRows();
    while(h < this->visibleRows) decreaseRows();
}

void Map::setCellAtGrid(int i, int j, cellType type){
    Cell* cell = qobject_cast<Cell*>(cellGrid.itemAtPosition(i, j)->widget());

    if(type == cellType::Start){
        startPos.setX(i);
        startPos.setY(j);
    }
    else if(type == cellType::End)
    {
        endPos.setX(i);
        endPos.setY(j);
    }
    cell->setCellType(type);
}

QString Map::findMoves(int row, int col){
    QString moves = "";

    for (int i = row - 1; i >= 0; --i) {
        Cell* cell = getCell(i, col);
        if (cell->getCellType() == cellType::Wall) break;
        if (cell->getCellType() == cellType::Decision || cell->getCellType() == cellType::End || cell->getCellType() == cellType::Start)
        {
            moves.append("N");
            break;
        }
    }

    for (int i = row + 1; i < this->visibleRows; ++i) {
        Cell* cell = getCell(i, col);
        if (cell->getCellType() == cellType::Wall) break;
        if (cell->getCellType() == cellType::Decision || cell->getCellType() == cellType::End || cell->getCellType() == cellType::Start)
        {
            moves.append("S");
            break;
        }
    }

    for (int j = col + 1; j < this->visibleCols; ++j) {
        Cell* cell = getCell(row, j);
        if (cell->getCellType() == cellType::Wall) break;
        if (cell->getCellType() == cellType::Decision || cell->getCellType() == cellType::End || cell->getCellType() == cellType::Start)
        {
            moves.append("E");
            break;
        }
    }

    for (int j = col - 1; j >= 0; --j) {
        Cell* cell = getCell(row, j);
        if (cell->getCellType() == cellType::Wall) break;
        if (cell->getCellType() == cellType::Decision || cell->getCellType() == cellType::End || cell->getCellType() == cellType::Start)
        {
            moves.append("W");
            break;
        }
    }

    return moves;
}

QJsonObject Map::getJSON(){
    QJsonObject json;

    json.insert("cellSize", 60);
    json.insert("height", this->visibleRows);
    json.insert("width", this->visibleCols);

    QJsonObject exit;
    exit.insert("row", this->endPos.x());
    exit.insert("col", this->endPos.y());
    json.insert("exit", exit);

    QJsonArray map;
    QJsonArray decisions;
    int dCount = 0;
    for(int i=0;i<visibleRows;i++){
        QJsonArray line;
        for(int j=0;j<visibleCols;j++){
            Cell* cell = getCell(i, j);
            line.push_back(static_cast<int>(cell->getCellType()));
            if(cell->getCellType() == cellType::Decision or cell->getCellType() == cellType::Start){
                QJsonObject decision;
                decision.insert("row", i);
                decision.insert("col", j);
                decision.insert("moves", findMoves(i, j));
                dCount++;
                decisions.push_back(decision);
            }
        }
        map.append(line);
    }
    json.insert("map", map);
    json.insert("decisionCount", dCount);
    json.insert("decisions", decisions);

    return json;
}
