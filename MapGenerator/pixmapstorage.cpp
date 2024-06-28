#include "pixmapstorage.h"

QMap<QString, QPixmap>& PixmapStorage::getGlobalStorage() {
    static QMap<QString, QPixmap> pixmaps;
    return pixmaps;
}
