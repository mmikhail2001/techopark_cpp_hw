#include "set.h"

template <typename T>
class Iterator
{

private:
    std::shared_ptr<typename Set<T>::Node> node;
};