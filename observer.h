#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <unordered_set>
enum class Resource {Brick, Energy, Glass, Heat, Wifi, Park};
class Subject;
class Observer {
    public: 
        virtual void notify(Subject &s) = 0;
        virtual ~Observer();
};

class Subject {
    std::unordered_set<Observer*> observers;
    public:
        void notify_observers();
        void attach( Observer *o);
        void detach( Observer *o);
        virtual ~Subject() = default;
};

#endif
