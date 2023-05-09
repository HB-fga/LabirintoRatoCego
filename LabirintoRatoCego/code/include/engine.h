#ifndef RC_ENGINE_H
#define RC_ENGINE_H

namespace engine {

    bool init(int w, int h);
    void close();

    namespace draw {

        void rect(int x, int y, int w, int h);

    }

    namespace screen {

        void clear();
        void show();

    }
}

#endif
