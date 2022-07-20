#include "observer.h"
#include <unordered_set>

void Subject::notify_observers() { // modified
    for(auto x : observers) x->notify(*this);
}

std::unordered_set<Observer*> Subject::get_observers() const {
    return observers;
}
void Subject::attach( Observer *o) {
    observers.insert(o);
}

void Subject::detach( Observer *o) {
    observers.erase(o);
}
