//  "Copyright [year] <Copyright Owner>"
#pragma once

/**
@invariant _count < _capacity
@invariant _pBody != nullptr
*/
class Stack {
 public:
    Stack();
    ~Stack();

    Stack(const Stack&) = delete;
    Stack(Stack&&) = delete;

    Stack& operator=(const Stack&&) = delete;
    Stack& operator=(Stack&&) = delete;
    /*
    @pre non
    @post non
    */
    int count() const;

    /*
    @pre non
    @post count() is incriment
    */
    bool push(int v);
    /*
    @pre count() > 0
    @post count() is decrement
    */
    bool pop(int& v);

 private:
    int* GetBody();
    int GetCount();

    int _capacity;
    int _count;
    int* _pBody;
};
