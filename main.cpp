#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>
#include <ctime>
#include "Patterns.h"

using namespace std;

// ���� ������� GPU

enum class GPU_BodyColor : int {
    Black,
    White,
    Silver,
    Unknown
};

// ���� GPU

enum class GPUType {
    GeForce = 1,
    Quadro = 2,
    Tesla = 3
};

// ������� �����

class GPU {
private:
    bool isWorking;
    GPU_BodyColor bodyColor;

protected:
    int memory;

public:
    GPU() : memory(rand() % 16 + 4), bodyColor(static_cast<GPU_BodyColor>(rand() % 4)) {
        isWorking = static_cast<bool>(rand() % 2);
    }

    virtual ~GPU() {}

    bool IsWorking() const { return isWorking; }
    int GetMemory() const { return memory; }
    virtual GPUType GetType() const = 0;
    GPU_BodyColor GetBodyColor() const { return bodyColor; }

    virtual void Use() = 0;
};

class GeForce : public GPU {
public:
    GeForce() : GPU() {}
    void Use() override
    {
    if (IsWorking())
        cout << "Using WORKING GPU... Gaming with GeForce!\n";
    else
        cout << "GeForce is broken, can't use it.\n";
    }
    GPUType GetType() const override { return GPUType::GeForce; }
};

class Quadro : public GPU {
public:
    Quadro() : GPU() {}
    void Use() override
    {
    if (IsWorking())
        cout << "Using WORKING GPU... Rendering with Quadro!\n";
    else
        cout << "Quadro is broken, can't use it.\n";
    }

    GPUType GetType() const override { return GPUType::Quadro; }
};

class Tesla : public GPU {
public:
    Tesla() : GPU() {}
    void Use() override
    {
    if (IsWorking())
        cout << "Using WORKING GPU... Training AI on Tesla!\n";
    else
        cout << "Tesla is broken, can't use it.\n";
    }
    GPUType GetType() const override { return GPUType::Tesla; }
};



// ��������� ����� �������� ���������

GPU* CreateGPU(GPUType type)
{
    GPU *newGPU = nullptr;

    if (type == GPUType::GeForce)
    {
        newGPU = new GeForce;
    }

    if (type == GPUType::Quadro)
    {
        newGPU = new Quadro;
    }

    if (type == GPUType::Tesla)
    {
        newGPU = new Tesla;
    }

    return newGPU;
}

// ��������� �� ����� ������� ����������

class GPUColorDecorator : public IteratorDecorator<GPU*> {
private:
    GPU_BodyColor targetColor;

public:
    GPUColorDecorator(Iterator<GPU*> *it, GPU_BodyColor color)
        : IteratorDecorator<GPU*>(it), targetColor(color) {}

    void First() override {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->GetBodyColor() != targetColor) {
            It->Next();
        }
    }

    void Next() override {
        do {
            It->Next();
        } while (!It->IsDone() && It->GetCurrent()->GetBodyColor() != targetColor);
    }
};

// ��������� �� ���� ����������

class GPUTypeDecorator : public IteratorDecorator<GPU*> {
private:
    GPUType targetType;

public:
    GPUTypeDecorator(Iterator<GPU*>* it, GPUType type)
        : IteratorDecorator<GPU*>(it), targetType(type) {}

    void First() override {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->GetType() != targetType)
            It->Next();
    }

    void Next() override {
        do {
            It->Next();
        } while (!It->IsDone() && It->GetCurrent()->GetType() != targetType);
    }
};


// ��������� �� ����������� ����������

class GPUWorkingDecorator : public IteratorDecorator<GPU*> {
private:
    bool requiredState;

public:
    GPUWorkingDecorator(Iterator<GPU*>* it, bool isWorking)
        : IteratorDecorator<GPU*>(it), requiredState(isWorking) {}

    void First() override {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->IsWorking() != requiredState)
            It->Next();
    }

    void Next() override {
        do {
            It->Next();
        } while (!It->IsDone() && It->GetCurrent()->IsWorking() != requiredState);
    }
};

// ������� ��� ����������� ����� �������

const char* GPUColorToString(GPU_BodyColor color) {
    switch (color) {
        case GPU_BodyColor::Black: return "Black";
        case GPU_BodyColor::White: return "White";
        case GPU_BodyColor::Silver: return "Silver";
        default: return "Unknown";
    }
}

// ������������� ������������� ���� ���������

void UseAllGPUs(Iterator<GPU*>* it) {
    for (it->First(); !it->IsDone(); it->Next()) {
        GPU* currentGPU = it->GetCurrent();
        cout << "Body color: " << GPUColorToString(currentGPU->GetBodyColor()) << " | ";
        currentGPU->Use();
    }
}


// ������������� ������ ������� ���������

void UseWorkingGPUs(Iterator<GPU*>* it) {
    for (it->First(); !it->IsDone(); it->Next()) {
        if (it->GetCurrent()->IsWorking())
            it->GetCurrent()->Use();
    }
}

// ������������� ������ GeForce

void UseGeForce(Iterator<GPU*>* it) {
    for (it->First(); !it->IsDone(); it->Next()) {
        if (it->GetCurrent()->GetType() == GPUType::GeForce)
            it->GetCurrent()->Use();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(nullptr));

    const size_t N = 30;

    // ������ ���������

    ArrayClass<GPU*> gpuArray;
    for (size_t i = 0; i < N; i++)
    {
        int gpu_num = rand() % 3 + 1;
        GPUType gpu_type = static_cast<GPUType>(gpu_num);
        GPU *newGPU = CreateGPU(gpu_type);
        gpuArray.Add(newGPU);
    }

    cout << "������ ������� ���������: " << gpuArray.Size() << endl;

    // ������ ���������

    list<GPU*> gpuVector;
    for(size_t i=0; i<N; i++)
    {
        int gpu_num = rand()%3+1;
        GPUType gpu_type = static_cast<GPUType>(gpu_num);
        GPU *newGPU = CreateGPU(gpu_type);
        gpuVector.push_back(newGPU);
    }

    cout << "������ ������ ���������: " << gpuVector.size() << endl;

    // ����� � ������� �����

    cout << "\n���������� ��� ���������� � �����:\n";
    for (size_t i = 0; i < gpuArray.Size(); i++){
        cout << "Body color: " << GPUColorToString(gpuArray[i]->GetBodyColor()) << " | ";
        gpuArray[i]->Use();
    }

    // ����� ���� ��������� ��� ������ ���������

    cout << "\n���������� ��� ����� ��������:\n";
    Iterator<GPU*>* allIt = gpuArray.GetIterator();
    UseAllGPUs(allIt);
    delete allIt;

    // ����� ���x ��������� ���������

    cout << "\n���������� ������ ���������:\n";
    Iterator<GPU*>* goodIt = new GPUWorkingDecorator(gpuArray.GetIterator(), true);
    UseAllGPUs(goodIt);
    delete goodIt;

    // ����� ���� ��������� GeForce

    cout << "\n���������� ������ GeForce:\n";
    Iterator<GPU*>* geForceIt = new GPUTypeDecorator(gpuArray.GetIterator(), GPUType::GeForce);
    UseAllGPUs(geForceIt);
    delete geForceIt;

    // ����� ���x ��������� ��������� Quadro

    cout << "\n���������� ��������� Quadro:\n";
    Iterator<GPU*>* goodQuadro = new GPUWorkingDecorator(new GPUTypeDecorator(gpuArray.GetIterator(), GPUType::Quadro), true);
    UseAllGPUs(goodQuadro);
    delete goodQuadro;

    // ����� ���x ��������� � ������ ������ �������

    cout << "\n���������� ������ ������ ����������:\n";
    Iterator<GPU*>* blackIt = new GPUColorDecorator(gpuArray.GetIterator(), GPU_BodyColor::Black);
    UseAllGPUs(blackIt);
    delete blackIt;

    // ������������ ������ ��������
    // ����� ���x ��������� ��������� Tesla

    cout << "\n���������� ������� ��� ��������� Tesla:\n";
    Iterator<GPU*>* adapted = new ConstIteratorAdapter<list<GPU*>, GPU*>(&gpuVector);
    Iterator<GPU*>* workingTesla = new GPUWorkingDecorator(new GPUTypeDecorator(adapted, GPUType::Tesla), true);
    UseAllGPUs(workingTesla);
    delete workingTesla;

    return 0;
}
