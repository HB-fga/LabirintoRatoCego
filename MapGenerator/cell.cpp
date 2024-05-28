#include "cell.h"
#include <QDebug>

Cell::Cell()
{
    img.load("../../assets/path.png");
    this->setFixedSize(30, 30);
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
