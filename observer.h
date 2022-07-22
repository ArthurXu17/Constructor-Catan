#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <unordered_set>
// for enum resources
#include "components.h"

class Subject;
class Player;
class Observer {
    public: 
        virtual void notify(Subject &s) = 0;
        virtual ~Observer();
        virtual Player *get_Owner() const = 0;
        virtual int get_resource_gain() const = 0; // added
};

class Subject {
    std::unordered_set<Observer*> observers;
    public:
        void notify_observers();
        std::unordered_set<Observer*> get_observers() const;
        void attach( Observer *o);
        void detach( Observer *o);
        virtual Resource get_resource() const = 0;
        virtual ~Subject() = default;
};

#endif
