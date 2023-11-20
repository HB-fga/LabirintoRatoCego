#ifndef RC_RATSELECTION_H
#define RC_CONFIGSELECTION_H

#include <vector>
#include <string>

namespace game {

    class RatSelection {
    private:
        int selectedQuantity;
        const int minQuantity;
        const int maxQuantity;
    
    public:
        RatSelection();
        int getSelectedQuantity() const;
        void navigateRight();
        void navigateLeft();
        void writeQuantitySelection() const;
    };

}
#endif