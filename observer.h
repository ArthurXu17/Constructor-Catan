#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <unordered_set>
class Subject;
enum class Resource {Brick, Energy, Glass, Heat, Wifi, Park};

class Observer {
    public: 
        virtual void notify(Subject &s) = 0;
        virtual ~Observer();
};

class Subject {
    std::unordered_set<Observer*> observers;
    public:
        void notify_observers() {
            for(auto x : observers) x->notify(*this);
        };
        void attach( Observer *o) {
            observers.insert(o);
        }
        void detach( Observer *o) {
            observers.erase(o);
        }
        virtual ~Subject() = default;
};
#endif
