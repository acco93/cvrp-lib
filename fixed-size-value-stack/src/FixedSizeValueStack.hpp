//
// Created by acco on 5/26/18.
//

#ifndef FIXEDSIZESTACK_HPP
#define FIXEDSIZESTACK_HPP

#include <cassert>
#include <functional>

    template<class T>
    class FixedSizeValueStack {

    private:

        T *array = nullptr;
        int begin = 0;
        int capacity;
        std::function<T(int)> initializer = nullptr;

    public:

        explicit FixedSizeValueStack(int dimension, std::function<T(int index)> array_initializer) {
            assert(dimension>0);
            array = new T[dimension];
            capacity = dimension;
            begin = 0;
            initializer = array_initializer;
            reset();
        }

        virtual ~FixedSizeValueStack() {
            delete [] array;
        }

        FixedSizeValueStack<T>&operator=(const FixedSizeValueStack<T>& other) {
            assert(capacity == other.capacity);

            for(int i = 0; i < capacity; i++){
                array[i] = other.array[i];
            }
            begin = other.begin;
            initializer = other.initializer;
            return *this;
        }

        T get(){
            assert(begin < capacity);
            auto item = array[begin];
            begin++;
            return item;
        }

        void push(T item) {
            begin--;
            assert(begin >= 0);
            array[begin] = item;
        }

        void reset() {
            for(int i = 0; i < capacity; i++) {
                array[i] = initializer(i);
            }
            begin = 0;
        }

        int size() const {
            return capacity - begin;
        }

        bool is_empty() {
            return begin == capacity;
        }
    };


#endif //FIXEDSIZESTACK_HPP
