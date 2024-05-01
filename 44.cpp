// Класс, реализующий абстрактный список
class BaseList {
protected:
    int count = 0; // количество элементов в списке

public:
    // Возвращает количество элементов в списке
    int Count() { return count; }

    // Абстрактные методы для добавления, вставки, удаления, очистки элементов списка
    virtual void Add(int a) = 0;
    virtual void Insert(int a, int pos) = 0;
    virtual void Delete(int pos) = 0;
    virtual void Clear() = 0;

    // Перегрузка оператора [] для доступа к элементам списка
    virtual int& operator[](int i) = 0;

    // Вывод элементов списка
    void Print() {
        for (int i = 0; i < Count(); i++) {
            std::cout << (*this)[i] << " ";
        }
        std::cout << std::endl;
    }

    // Присваивание элементов данного списка другому списку
    virtual void AssignTo(BaseList* dest) {
        dest->Assign(this);
    }

    // Клонирование списка
    virtual BaseList* Clone() {
        BaseList* clone_list = EmptyClone();
        clone_list->Assign(this);
        return clone_list;
    }

    // Сортировка списка
    virtual void Sort() {
        // алгоритм сортировки пузырьком
    }

    // Проверка на равенство двух списков
    virtual bool Equals(BaseList* list) {
        // сравнение поэлементно
    }

    // Присваивание элементов из другого списка
    virtual void Assign(BaseList* source) {
        // очистить текущий список, скопировать элементы из source
    }

protected:
    // Создание пустой копии списка
    virtual BaseList* EmptyClone() = 0;
};

// Класс, реализующий список на основе динамического массива
class ArrList : public BaseList {
// реализация методов Add, Insert, Delete, Clear, operator[], EmptyClone
};

// Класс, реализующий список на основе цепочки узлов
class ChainList : public BaseList {
// реализация методов Add, Insert, Delete, Clear, operator[], EmptyClone
};

// Класс для тестирования списков
class Tester {
// метод Test для проведения тестирования списков
};

// Функция main для демонстрации работы списков
int main() {
    // Демонстрация работы с динамическим списком
    BaseList* dynamicArray = new ArrList();
    dynamicArray->Add(1);
    dynamicArray->Print();
    
    // Демонстрация работы с цепочечным списком
    BaseList* chainList = new ChainList();
    chainList->Add(1);
    chainList->Print();
    
    // Тестирование списков
    Tester::Test();
    
    delete dynamicArray;
    delete chainList;

    return 0;
}
