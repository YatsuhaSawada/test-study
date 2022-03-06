#ifndef _STACK_H_
#define _STACK_H_

/*
@invariant _count < _capacity
@invariant _pBody != nullptr
*/
class Stack
{
public:

    Stack();
    ~Stack();

    Stack(const Stack&) = delete;
    Stack(Stack&&) = delete;

    Stack& operator=(const Stack&&) = delete;
    Stack& operator=(Stack&&) = delete;

    
    /*
    @requier non
    @ensure non
    */
    int count() const;

    /*
    @requier non
    @ensure count() is incriment
    */
    bool push(int v);
    /*
    @requier count() > 0
    @ensure count() is decrement
    */
    bool pop(int& v);

private:

    int* GetBody();
    int GetCount();

    int _capacity;
    int _count;
    int* _pBody;
};

#endif
