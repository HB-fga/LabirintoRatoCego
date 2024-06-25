#ifndef PIXMAPSTORAGE_H
#define PIXMAPSTORAGE_H

#include <QPixmap>
#include <QString>
#include <QMap>

class PixmapStorage
{
public:
    static QMap<QString, QPixmap>& getGlobalStorage();
};

#endif // PIXMAPSTORAGE_H
