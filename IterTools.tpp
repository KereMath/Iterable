#pragma once

#include "Iterable.tpp"

template<typename T>
class repeatIterator : public IIterator<int>
{
    private:
        int value;
    public:
        repeatIterator(int value) : value(value) {}
        int next()
        {
            return value;
        }
};

template<typename T>
class repeatIterable : public IIterable<int>
{
    private:
        int value;
    public:
        repeatIterable(int value) : value(value) {}
        IIterator<int> * iter() const
        {
            return new repeatIterator<T>(value);
        }
};


template <typename T>
IIterable<T> *repeat(T v)
{
    return new repeatIterable<T>(v);
}

template <typename T>
class CountIterator : public IIterator<T>
{
    private:
        T start, delta, current;
    public:
        CountIterator(T start, T delta) : delta(delta), current(start) {}
        T next()
        {
            current += delta;
            return current;
        }
};
template <typename T>
class CountIterable : public IIterable<T>
{
    private:
        T start, delta;
    public:
        CountIterable(T start, T delta) : start(start), delta(delta) {}
        IIterator<T> * iter() const
        {
            return new CountIterator<T>(start, delta);
        }
};

template<typename T>
IIterable<T> * count(T start, T delta){
    return new CountIterable<T>(start - delta, delta);
}

template <typename T>
class SkipIterator : public IIterator<T>
{
    private:
        unsigned k;
        IIterator<T> *i;
    public:
        SkipIterator(unsigned k, IIterator<T> *i) : k(k), i(i){}
        T next()
        {
            return i->next();
        }
        ~SkipIterator()
        {
            delete i;
        }
};
template <typename T>
class SkipIterable : public IIterable<T>
{
    private:
        unsigned k;
        IIterable<T> *i;
    public:
        SkipIterable(unsigned k, IIterable<T> *i) : k(k), i(i) {}
        IIterator<T> * iter() const
        {
            SkipIterator<T> *S = new SkipIterator<T>(k, i->iter());
            for(int j=0; j<k; j++){
                try {S->next();}
                catch(StopIteration) {break;}
            }
            return S;
        }
        ~SkipIterable()
        {
            delete i;
        }

};


template<typename T>
IIterable<T> * skip(unsigned k, IIterable<T> *i){
    return new SkipIterable<T>(k, i);
}


template <typename T>
class TakeIterator : public IIterator<T>
{
    private:
        unsigned k, count;
        IIterator<T> *i;
    public:
        TakeIterator(unsigned k, IIterator<T> *i) : k(k), i(i), count(0) {}
        T next()
        {
            if (count < k)
            {
                count++;
                return i->next();
            }
            else
                throw StopIteration();
        }
        ~TakeIterator()
        {
            delete i;
        }
};
template <typename T>
class TakeIterable : public IIterable<T>
{
    private:
        unsigned k;
        IIterable<T> *i;
    public:
        TakeIterable(unsigned k, IIterable<T> *i) : k(k), i(i) {}
        IIterator<T> * iter() const
        {
            return new TakeIterator<T>(k, i->iter());
        }
        ~TakeIterable()
        {
            delete i;
        }
};

template<typename T>
IIterable<T> * take(unsigned k, IIterable<T> *i){
    return new TakeIterable<T>(k, i);
}

template <typename T>
class CycleIterator : public IIterator<T>
{
    private:
        IIterator<T> *i;
        IIterable<T> *i2;
    public:
        CycleIterator(IIterable<T> *i) : i(i->iter()), i2(i){}
        T next()
        {
            try
            {
                return i->next();
            }
            catch (StopIteration)
            {
                delete i;
                i = i2->iter();
                return i->next();
            }
        }
        ~CycleIterator(){
            delete i;
        }
};
template <typename T>
class CycleIterable : public IIterable<T>
{
    private:
        IIterable<T> *i;
    public:
        CycleIterable(IIterable<T> *i) : i(i) {}
        IIterator<T> * iter() const
        {
            return new CycleIterator<T>(i);
        }
        ~CycleIterable(){delete i;}
};
template<typename T>
IIterable<T> * cycle(IIterable<T> *i){
    return new CycleIterable<T>(i);
}


template <typename T>
class ConcatIterator : public IIterator<T>
{
    private:
        IIterator<T> *i, *i2;
    public:
        ConcatIterator(IIterable<T> *i, IIterable<T> *i2) : i(i->iter()), i2(i2->iter()) {}
        T next()
        {
            try
            {
                return i->next();
            }
            catch (StopIteration)
            {
                return i2->next();
            }
        }
        ~ConcatIterator(){delete i, delete i2;}
};
template <typename T>
class ConcatIterable : public IIterable<T>
{
    private:
        IIterable<T> *i, *i2;
    public:
        ConcatIterable(IIterable<T> *i, IIterable<T> *i2) : i(i), i2(i2) {}
        IIterator<T> * iter() const
        {
            return new ConcatIterator<T>(i, i2);
        }
        ~ConcatIterable(){delete i, delete i2;}
};

template<typename T>
IIterable<T> * concat(IIterable<T> *i, IIterable<T> *j){
    return new ConcatIterable<T>(i, j);
}


template<typename T>
class alternateIterator : public IIterator<T>
{
    private:
        IIterator<T> *i, *i2;
        bool flag;
    public:
        alternateIterator(IIterable<T> *i, IIterable<T> *i2) : i(i->iter()), i2(i2->iter()), flag(true) {}
        T next()
        {
            if (flag)
            {
                flag = false;
                try{return i->next();}
                catch (StopIteration)
                {
                    return i2->next();
                }
            }
            else
            {
                flag = true;
                try{return i2->next();}
                catch (StopIteration)
                {
                    return i->next();
                }
            }
        }
        ~alternateIterator(){delete i, delete i2;}
};

template<typename T>
class alternateIterable : public IIterable<T>
{
    private:
        IIterable<T> *i;
        IIterable<T> *i2;
    public:
        alternateIterable(IIterable<T> *i, IIterable<T> *i2) : i(i), i2(i2) {}
        IIterator<T> * iter() const
        {
            return new alternateIterator<T>(i, i2);
        }
        ~alternateIterable(){delete i, delete i2;}
};

template<typename T>
IIterable<T> * alternate(IIterable<T> *i, IIterable<T> *j){
    return new alternateIterable<T>(i, j);
}

template<typename T>
class filterIterator : public IIterator<T>
{
    private:
        IIterator<T> *i;
        IIterator<bool> *filter;

    public:
        filterIterator(IIterable<T> *i, IIterable<bool> *filter) : i(i->iter()), filter(filter->iter()) {}
        T next()
        {
            while (!filter->next()) i->next();
            return i->next();
        }
        ~filterIterator(){delete i, delete filter;}
};
template<typename T>
class filterIterable : public IIterable<T>
{
    private:
        IIterable<T> *i;
        IIterable<bool> *filter;
    public:
        filterIterable(IIterable<T> *i, IIterable<bool> *filter) : i(i), filter(filter) {}
        IIterator<T> * iter() const
        {
            return new filterIterator<T>(i, filter);
        }
        ~filterIterable(){delete i, delete filter;}
};

template <typename T>
IIterable<T> *filter(IIterable<T> *i, IIterable<bool> *f){
    return new filterIterable<T>(i, f);
}