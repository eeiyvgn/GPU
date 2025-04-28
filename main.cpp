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

// ���������� �������� "���������"

enum class GPUUsageMannerEnum : int
{
    Gaming,
    Rendering,
    AITraining,
    None
};

class GPUUsageStrategy
{
public:
    virtual ~GPUUsageStrategy() {}
    virtual void UseGPU() = 0;
};

class GamingUsageStrategy : public GPUUsageStrategy
{
public:
    void UseGPU() override
    {
        cout << "Using GPU for high-performance gaming..." << endl;
    }
};

class RenderingUsageStrategy : public GPUUsageStrategy
{
public:
    void UseGPU() override
    {
        cout << "Using GPU for professional rendering tasks..." << endl;
    }
};

class AITrainingUsageStrategy : public GPUUsageStrategy
{
public:
    void UseGPU() override
    {
        cout << "Using GPU for AI model training and deep learning..." << endl;
    }
};

class MiningUsageStrategy : public GPUUsageStrategy
{
public:
    void UseGPU() override
    {
        cout << "Using GPU for cryptocurrency mining..." << endl;
    }
};

// ��������� ����� ��� �������� ��������� �������������

GPUUsageStrategy* CreateGPUUsageStrategy(GPUUsageMannerEnum usageManner)
{
    switch (usageManner)
    {
    case GPUUsageMannerEnum::Gaming:
        return new GamingUsageStrategy;
    case GPUUsageMannerEnum::Rendering:
        return new RenderingUsageStrategy;
    case GPUUsageMannerEnum::AITraining:
        return new AITrainingUsageStrategy;
    default:
        return nullptr;
    }
}

// ������� �����

class GPU {
private:
    bool isWorking;
    GPU_BodyColor bodyColor;
    GPUUsageStrategy* usageStrategy; // ��������� ���������

protected:
    int memory;

    void DoUseStrategy()
    {
        if (usageStrategy != nullptr)
            usageStrategy->UseGPU();
        else
            cout << "No usage strategy defined!\n";
    }

    virtual void PreUseMessage()
    {
        cout << "Using WORKING GPU... ";
    }

    virtual void PostUseMessage()
    {

    }

public:
    GPU() : memory(rand() % 16 + 4), bodyColor(static_cast<GPU_BodyColor>(rand() % 4)), usageStrategy(nullptr)
    {
        isWorking = static_cast<bool>(rand() % 2);
    }

    virtual ~GPU()
    {
        if (usageStrategy != nullptr)
            delete usageStrategy;
    }

    bool IsWorking() const { return isWorking; }
    int GetMemory() const { return memory; }
    virtual GPUType GetType() const = 0;
    virtual const char* GetGPUTypeName() const = 0;

    GPU_BodyColor GetBodyColor() const { return bodyColor; }

    virtual void Use()
    {
        if (IsWorking())
        {
            PreUseMessage();
            DoUseStrategy();
            PostUseMessage();
        }
        else
        {
            cout << GetGPUTypeName() << " is broken, can't use it.\n";
        }
    }

    void SetUsageStrategy(GPUUsageStrategy* strategy)
    {
        if (usageStrategy != nullptr)
            delete usageStrategy;
        usageStrategy = strategy;
    }
};

class GeForce : public GPU {
public:
    GeForce()
    {
        SetUsageStrategy(CreateGPUUsageStrategy(GPUUsageMannerEnum::Gaming));
    }

    GPUType GetType() const override { return GPUType::GeForce; }

    const char* GetGPUTypeName() const override { return "GeForce"; }

protected:
    void PreUseMessage() override
    {
        cout << "Using for gaming: ";
    }
};

class Quadro : public GPU {
public:
    Quadro()
    {
        SetUsageStrategy(CreateGPUUsageStrategy(GPUUsageMannerEnum::Rendering));
    }

    GPUType GetType() const override { return GPUType::Quadro; }

    const char* GetGPUTypeName() const override { return "Quadro"; }

protected:
    void PreUseMessage() override
    {
        cout << "Rendering graphics: ";
    }
};

class Tesla : public GPU {
public:
    Tesla()
    {
        SetUsageStrategy(CreateGPUUsageStrategy(GPUUsageMannerEnum::AITraining));
    }

    GPUType GetType() const override { return GPUType::Tesla; }

    const char* GetGPUTypeName() const override { return "Tesla"; }

protected:
    void PreUseMessage() override
    {
        cout << "Training AI models: ";
    }
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
