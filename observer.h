#include <unordered_set>
class Subject;
enum class Resource {Park, Brick, Energy, Glass, Heat, Wifi};

class Observer {
    public: 
        virtual void notify(Subject &s) = 0;
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
        virtual ~Subject() = 0;
        virtual Resource get_resource() = 0;
};