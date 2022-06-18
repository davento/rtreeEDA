#include "Node.h"

template <typename T, unsigned ORDER>
Node<T,ORDER>::Node(): father(nullptr), myBound(new boundType()){

}
