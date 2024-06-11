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
    this->rows = 30;
    this->columns = 30;
    this->visibleRows = 10;
    this->visibleCols = 10;
    this->startPos = QPoint(0, 0);
    this->endPos = QPoint(9, 9);
    this->selectedCellType = cellType::Wall;

    for(int i=0; i<this->rows; i++)
    {
        for(int j=0; j<this->columns; j++){
            Cell* cell = new Cell;
            if(i >= this->visibleRows || j >= this->visibleCols)
                cell->setVisible(false);
            cellGrid.addWidget(cell, i, j);
        }
    }

    Cell* startCell = getCell(this->startPos.x(), this->startPos.y());
    startCell->setCellType(cellType::Start);
    Cell* endCell = getCell(this->endPos.x(), this->endPos.y());
    endCell->setCellType(cellType::End);

    this->cellGrid.setSpacing(0);
    this->setLayout(&cellGrid);
}

Map::~Map()
{

}

void Map::changeCursor(QString asset){
    QPixmap p = QPixmap(asset);
    p = p.scaled(QSize(20, 20),  Qt::KeepAspectRatio);
    QCursor c = QCursor(p, 0, 0);
    setCursor(c);
}

void Map::changeCellTypeWall(){
    this->selectedCellType = cellType::Wall;
    // this->changeCursor("../../assets/wall.png");
}

void Map::changeCellTypeStart(){
    this->selectedCellType = cellType::Start;
    // this->changeCursor("../../assets/start.png");
}

void Map::changeCellTypeEnd(){
    this->selectedCellType = cellType::End;
    // this->changeCursor("../../assets/end.png");

}

void Map::changeCellTypeDecision(){
    this->selectedCellType = cellType::Decision;
    // this->changeCursor("../../assets/decision.png");
}

void Map::changeCellTypePath(){
    this->selectedCellType = cellType::Path;
    // this->changeCursor("../../assets/path.png");
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
    if(event->buttons() & Qt::LeftButton) setGridCellType(cell); // Paint
    else if(event->buttons() & Qt::RightButton) this->selectedCellType = cell->getCellType(); // Pick Cell
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
        if(cell->getCellType() == this->selectedCellType) return;

        cell->setCellType(this->selectedCellType);
        if(this->selectedCellType == cellType::Start)
        {
            Cell* oldStartCell = getCell(startPos.x(), startPos.y());
            if(oldStartCell->getCellType()==cellType::Start) oldStartCell->setCellType(cellType::Path);

            int newCellRow = cellGrid.indexOf(cell)/rows;
            int newCellCol = cellGrid.indexOf(cell)%rows;

            this->startPos.setX(newCellRow);
            this->startPos.setY(newCellCol);
        }
        else if(selectedCellType == cellType::End)
        {
            Cell* oldEndCell = getCell(endPos.x(), endPos.y());
            if(oldEndCell->getCellType()==cellType::End) oldEndCell->setCellType(cellType::Path);

            int newCellRow = cellGrid.indexOf(cell)/rows;
            int newCellCol = cellGrid.indexOf(cell)%rows;

            this->endPos.setX(newCellRow);
            this->endPos.setY(newCellCol);
        }
        repaint();
    }
}

void Map::increaseRows(){
    if(this->visibleRows+1 > this->rows) return;
    for(int j=0; j<this->visibleCols; j++){
        Cell* cell = getCell(this->visibleRows, j);
        cell->setVisible(true);
    }
    this->visibleRows++;

    repaint();
}

void Map::increaseCols(){
    if(this->visibleCols+1 > this->columns) return;
    for(int i=0; i<this->visibleRows; i++){
        Cell* cell = getCell(i, this->visibleCols);
        cell->setVisible(true);
    }
    this->visibleCols++;

    repaint();
}

void Map::decreaseRows(){
    if(this->visibleRows-1 < 2) return;
    for(int j=0; j<this->visibleCols; j++){
        Cell* cell = getCell(this->visibleRows-1, j);
        cell->setVisible(false);
    }
    this->visibleRows--;

    repaint();
}

void Map::decreaseCols(){
    if(visibleCols-1 < 2) return;
    for(int i=0; i<this->visibleRows; i++){
        Cell* cell = getCell(i, this->visibleCols-1);
        cell->setVisible(false);
    }
    this->visibleCols--;

    repaint();
}

void Map::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    paintGrid(&painter);

}

void Map::paintGrid(QPainter* painter){
    for(int i=0; i<this->rows; i++){
        for(int j=0; j<this->columns; j++){
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
        this->startPos.setX(i);
        this->startPos.setY(j);
    }
    else if(type == cellType::End)
    {
        this->endPos.setX(i);
        this->endPos.setY(j);
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
