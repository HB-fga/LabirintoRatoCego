#include "cell.h"
#include "pixmapstorage.h"
#include <QDebug>

Cell::Cell()
{
    this->setCellType(cellType::Path);
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
            this->img = PixmapStorage::getGlobalStorage().value("wall");
            break;
        case cellType::Path:
            this->img = PixmapStorage::getGlobalStorage().value("path");
            break;
        case cellType::Decision:
            this->img = PixmapStorage::getGlobalStorage().value("decision");
            break;
        case cellType::Start:
            this->img = PixmapStorage::getGlobalStorage().value("start");
            break;
        case cellType::End:
            this->img = PixmapStorage::getGlobalStorage().value("end");
            break;
    }

    this->img = this->img.scaled(QSize(30, 30),  Qt::KeepAspectRatio);
    this->type = newType;
}

QPixmap Cell::getCellImage()
{
    return this->img;
}
