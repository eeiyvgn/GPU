#ifndef PatternTemplatesH
#define PatternTemplatesH

#include <iostream>
#include <vector>

using namespace std;

// Итератор

template<typename T>
class Iterator
{
protected:
    Iterator() {}

public:
    virtual ~Iterator() {}
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T GetCurrent() const = 0;
};


// Итератор для обхода вектора

template<typename T>
class StackIterator : public Iterator<T>
{
private:
    const vector<T*> *StackContainer;
    size_t Pos;

public:
    StackIterator(const vector<T*> *container) : StackContainer(container), Pos(0) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= StackContainer->size()); }
    T* GetCurrent() const override { return &(*StackContainer)[Pos]; }
};

// Вектор

const size_t MaxSize = 30;

template <typename T>
class StackClass
{
private:
    vector<T*> Items;

public:
    bool IsEmpty() const { return Items.empty(); }

    size_t Size() const { return Items.size(); }

    void Push(T* newObject) { Items.push_back(newObject); }

    T* Pop()
    {
        T* top = Items.back();
        Items.pop_back();
        return top;
    }

    T* INDEX(size_t i) const { return Items[i]; }

    Iterator<T> *GetIterator()
    {
        return new StackIterator<T>(&Items);
    }
};

// Итератор для обхода массива

template<typename T>
class ArrayIterator : public Iterator<T>
{
private:
    const T *ArrayContainer;
    size_t Pos;

public:
    ArrayIterator(const T *container) : ArrayContainer(container), Pos(0) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= MaxSize); }
    T GetCurrent() const override { return ArrayContainer[Pos]; }
};

// Массив

template <typename T>
class ArrayClass
{
private:
    T Items[MaxSize];
    size_t ArraySize;

public:
    void Add(T newObject) { Items[ArraySize++] = newObject; }
    size_t Size() const { return ArraySize; }

    T operator[](size_t index) const { return Items[index]; }

    Iterator<T> *GetIterator()
    {
        return new ArrayIterator<T>(Items);
    }

    ArrayClass() : ArraySize(0) {}
};

// Декоратор для итератора (пустой)

template<typename T>
class IteratorDecorator : public Iterator<T>
{
protected:
    Iterator<T> *It;

public:
    IteratorDecorator(Iterator<T> *it) : It(it) {}
    virtual ~IteratorDecorator() { delete It; }
    virtual void First() { It->First(); }
    virtual void Next() { It->Next(); }
    virtual bool IsDone() const { return It->IsDone(); }
    virtual T GetCurrent() const { return It->GetCurrent(); }
};

// Адаптер для контейнерных классов STL

template<typename ContainerType, typename ItemType>
class ConstIteratorAdapter : public Iterator<ItemType>
{
protected:
    ContainerType *Container;
    typename ContainerType::const_iterator It;

public:
    ConstIteratorAdapter(ContainerType* container)
        : Container(container)
    {
        It = Container->begin();
    }

    virtual ~ConstIteratorAdapter() {}
    virtual void First() override { It = Container->begin(); }
    virtual void Next() override { ++It; }
    virtual bool IsDone() const override { return It == Container->end(); }
    virtual ItemType GetCurrent() const override { return *It; }
};


#endif // PatternTemplatesH
