#include "cell.h"
#include <QDebug>
#include <QFileDialog>

Cell::Cell()
{
    img.load("../../assets/path.png");
}

cellType Cell::getCellType()
{
    return this->type;
}

void Cell::setCellType(cellType newType)
{
    if(this->type == newType) return;

    switch(newType)
    {
        case cellType::Wall:
            this->img.load("../../assets/wall.png");
            break;
        case cellType::Path:
            this->img.load("../../assets/path.png");
            break;
        case cellType::Decision:
            this->img.load("../../assets/decision.png");
            break;
        case cellType::Start:
            this->img.load("../../assets/start.png");
            break;
        case cellType::End:
            this->img.load("../../assets/end.png");
            break;
    }

    this->type = newType;
}

QPixmap Cell::getCellImage()
{
    return this->img;
}

void Cell::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "cell clicada: " << this->pos();
    this->setCellType(cellType::Start);
    repaint();
}
