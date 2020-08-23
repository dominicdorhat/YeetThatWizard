#ifndef HELPER_H
#define HELPER_H

namespace Helper {
    
    // Java's instanceof, performance expensive from the constant dynamic_cast-ing
    template<typename Base, typename T>
    inline bool instanceof(const T* ptr) {
        return dynamic_cast<const Base*>(ptr) != nullptr;
    }

}

#endif