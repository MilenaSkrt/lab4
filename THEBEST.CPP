#include <iostream>
#include <vector>
#include <random>

class BaseList {
protected: // доступ protected для переменной count
    int count = 0; // объявление переменной count и присвоение значения 0

public: // открытые методы класса BaseList
    int Count() { return count; } // метод для получения значения переменной count
    virtual void Add(int a) = 0; // чисто виртуальный метод для добавления элемента
    virtual void Insert(int a, int pos) = 0; // чисто виртуальный метод для вставки элемента
    virtual void Delete(int pos) = 0; // чисто виртуальный метод для удаления элемента
    virtual void Clear() = 0;  // чисто виртуальный метод для очистки списка
    virtual void DuplicateAll() = 0; // чисто виртуальный метод дублирования всех элементов

    virtual int& operator[](int i) = 0; // виртуальный метод для доступа к элементу по индексу

    void Print() { // метод для печати элементов списка
        for (int i = 0; i < Count(); i++) {
            std::cout << (*this)[i] << " "; // выводим элементы списка
        }
        std::cout << std::endl; // переход на новую строку
    }

    virtual void AssignTo(BaseList* dest) { // Метод для копирования элементов в переданный список
        dest->Assign(this);
    }

    virtual BaseList* Clone() { // Метод для создания клонированного списка
        BaseList* clone_list = EmptyClone();
        clone_list->Assign(this);
        return clone_list;
    }

    virtual void Sort() { // Метод для сортировки списка
        bool swapped;
        for (int i = 0; i < Count() - 1; i++) {
            swapped = false;
            for (int j = 0; j < Count() - i - 1; j++) {
                if ((*this)[j] > (*this)[j + 1]) {
                    int temp = (*this)[j];
                    (*this)[j] = (*this)[j + 1];
                    (*this)[j + 1] = temp;
                    swapped = true;
                }
            }
            if (!swapped) {
                break;
            }
        }
    }

    virtual bool Equals(BaseList* list) { // Метод для сравнения двух списков
        if (list == nullptr || dynamic_cast<BaseList*>(list) == nullptr || this->Count() != list->Count()) {
            return false;
        }

        for (int i = 0; i < this->Count(); i++) {
            if ((*this)[i] != (*list)[i]) {
                return false;
            }
        }

        return true;
    }

    virtual void Assign(BaseList* source) {
        // Метод для присвоения значений из source в текущий список
            Clear();
        if (source == nullptr) {
            return;
        }

        for (int i = 0; i < source->Count(); i++) {
            Add((*source)[i]);
        }
    }

protected:
    virtual BaseList* EmptyClone() = 0; // Абстрактный метод для создания пустого клонированного списка
};

class ArrList : public BaseList { // Класс ArrList, который наследуется от базового класса BaseList
private:
    int* buf;  // указатель на массив
    int Length; //длина массива

public:
    ArrList() {
        Length = 2; // начальная длина массива
        buf = new int[Length]; // выделение памяти под массив
        count = 0; // инициализация счетчика элементов
    }
    void DuplicateAll() override { //переопределение
        for (int i = 0; i < count * 2; i += 2) { //цикл начинается с 0 и продолжается пока, каждая итерация увеличивает i на 2
            Insert(buf[i], i + 1); //вставляет элемент buf на позицию i+1 
        }
    }
    ~ArrList() { // Деструктор класса ArrList
        delete[] buf; // освобождение памяти
    }

    void Add(int a) override { // Метод для добавления элемента в массив
        if (count >= Length) { // Проверка на переполнение массива
            Length *= 2; // увеличиваем длину массива
            int* newbuf = new int[Length]; // создаем новый массив большей длины
            std::copy(buf, buf + count, newbuf); // копируем содержимое старого массива в новый
            delete[] buf; // удаляем старый массив
            buf = newbuf; // присваиваем указателю на массив новый адрес
        }
        buf[count] = a; // добавляем элемент в массив
        count++; // увеличиваем счетчик элементов
    }

    void Insert(int a, int pos) override { // Метод для вставки элемента в заданную позицию
        if (pos < 0 || pos > count) { // Проверка валидности позиции
            return; // выход из функции
        }

        if (Length == count) { // Проверка на переполнение массива
            Length *= 2; // увеличиваем длину массива
            int* newbuf = new int[Length];  // создаем новый массив большей длины
            std::copy(buf, buf + count, newbuf); // копируем содержимое старого массива в новый
            delete[] buf; // удаляем старый массив
            buf = newbuf; // присваиваем указателю на массив новый адрес
        }

        for (int i = count; i > pos; i--) { // Сдвигаем элементы массива для вставки нового элемента
            buf[i] = buf[i - 1];
        }

        buf[pos] = a; // вставляем элемент в массив
        count++; // увеличиваем счетчик элементов
    }

    void Delete(int pos) override { // Метод для удаления элемента из массива
        if (pos < 0 || pos >= count) { // Проверка валидности позиции
            return; // выход из функции
        }

        for (int i = pos; i < count - 1; i++) { // Сдвигаем элементы массива для удаления элемента
            buf[i] = buf[i + 1];
        }

        count--; // уменьшаем счетчик элементов
    }

    void Clear() override { // Метод для очистки массива
        delete[] buf; // удаляем массив
        Length = 2; // сбрасываем длину массива
        buf = new int[Length];  // выделяем новую память под массив
        count = 0; // сбрасываем счетчик элементов
    }

    int& operator[](int i) override { // Перегрузка оператора [] для доступа к элементу по индексу
        if (i >= count || i < 0) { // Проверка на выход за границы массива
            throw std::out_of_range("Element is out of range"); // выброс исключения
        }
        return buf[i]; // возвращаем элемент массива
    }

protected:
    BaseList* EmptyClone() override { // Метод для создания клона пустого объекта
        return new ArrList(); // возвращаем новый пустой объект ArrList
    }
};

class ChainList : public BaseList { // Определение класса ChainList, который наследуется от класса BaseList
private:
    struct Node { // Определение структуры Node
        int Data;
        Node* Next;

        Node(int data) : Data(data), Next(nullptr) {} // Конструктор узла Node
    };

    Node* head; // Указатель на голову списка

public:
    ChainList() : head(nullptr) {} // Конструктор класса ChainList

    void Add(int data) override { // Метод добавления элемента в конец списка
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->Next != nullptr) {
                current = current->Next;
            }
            current->Next = newNode;
        }
        count++;
    }
    void DuplicateAll() override { //переопределение
        Node* current = head; //создание указателя и присваивание ему адрес первого эл-та
        int index = 0; //обьявление переменной index равной нулю
        while (current != nullptr) { //пока не конец списка
            Insert(current->Data, index + 1); //встака нового узла с данными текущего узла и индексом 
            current = current->Next; //передвигаем указатель на следующий эл-т списка
            index += 2; //увеличение значение индекса на 2
        }
    }
    void Insert(int data, int pos) override { // Метод добавления элемента по указанной позиции в списке
        if (pos < 0 || pos > count) {
            return;
        }

        Node* newNode = new Node(data);
        if (pos == 0) {
            newNode->Next = head;
            head = newNode;
        }
        else {
            Node* current = head;
            for (int i = 0; i < pos - 1; i++) {
                current = current->Next;
            }
            newNode->Next = current->Next;
            current->Next = newNode;
        }
        count++;
    }

    void Delete(int pos) override { // Метод удаления элемента по указанной позиции в списке
        if (pos < 0 || pos >= count) {
            return;
        }

        Node* current = head;
        if (pos == 0) {
            head = head->Next;
            delete current;
        }
        else {
            for (int i = 0; i < pos - 1; i++) {
                current = current->Next;
            }
            Node* temp = current->Next;
            current->Next = temp->Next;
            delete temp;
        }
        count--;
    }

    void Clear() override { // Метод очистки списка
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->Next;
            delete temp;
        }
        head = nullptr;
        count = 0;
    }

    int& operator[](int i) override { // Перегрузка оператора [] для доступа к элементу по индексу
        Node* current = head;
        for (int j = 0; j < i; j++) {
            if (current != nullptr) {
                current = current->Next;
            }
            else {
                throw std::out_of_range("Index out of range");
            }
        }
        return current->Data;
    }

protected:
    BaseList* EmptyClone() override { // Метод создания пустой копии списка
        return new ChainList();
    }
};

class Tester { // Класс для тестирования различных операций над списками
private:
    static std::vector<BaseList*> lists; // Статический вектор указателей на базовый класс списка
    static std::random_device rd;  // Генератор случайных чисел для инициализации
    static std::mt19937 gen; // Генератор случайных операций
    static std::uniform_int_distribution<int> op_dist; // Равномерное распределение для операций
    static std::uniform_int_distribution<int> val_dist;  // Равномерное распределение для значений
    static std::uniform_int_distribution<int> index_dist;  // Равномерное распределение для индексов

public:
    static void Test() { // Статический метод для проведения тестирования
        ArrList* list1 = new ArrList(); // Создание динамического массива 
        ChainList* list2 = new ChainList(); // Создание списка цепочек

        lists.push_back(list1); // Добавление массива в вектор
        lists.push_back(list2);  // Добавление списка цепочек в вектор

        gen = std::mt19937(rd()); // Инициализация генератора случайных чисел
        op_dist = std::uniform_int_distribution<int>(0, 5); // Операции от 0 до 5
        val_dist = std::uniform_int_distribution<int>(1, 1000); // Значения от 1 до 1000
        index_dist = std::uniform_int_distribution<int>(1, 100); // Индексы от 1 до 100

        for (int i = 0; i < 10000; i++) { // Цикл по 10000 итераций
            int operation = op_dist(gen); // Генерация случайной операции
            int value = val_dist(gen); // Генерация случайного значения
            int index = index_dist(gen); // Генерация случайного индекса

            try { // Обработка исключений
                switch (operation) { // Выбор операции
                case 0:
                    list1->Add(value); // Добавление значения в массив
                    list2->Add(value); // Добавление значения в список цепочек
                    break;
                case 1:
                    if (list1->Count() > 0 && list2->Count() > 0) {
                        list1->Insert(value, index); // Вставка значения по индексу в массив
                        list2->Insert(value, index); // Вставка значения по индексу в список цепочек
                    }
                    break;
                case 2:
                    if (list1->Count() > 0 && list2->Count() > 0) {
                        list1->Delete(index); // Удаление значения по индексу из массива
                        list2->Delete(index); // Удаление значения по индексу из списка цепочек
                    }
                    break;
                case 3:
                    list1->Clear(); // Очистка массива
                    list2->Clear(); // Очистка списка цепочек
                    break;
                case 4:
                    if (list1->Count() > 0 && list2->Count() > 0) {
                        (*list1)[index] = value; // Присваивание значения по индексу в массиве
                        (*list2)[index] = value; // Присваивание значения по индексу в списке цепочек
                    }
                    break;
                case 5:
                    list1->Sort(); // Сортировка массива
                    list2->Sort(); // Сортировка списка цепочек
                    break;
                }
                case 6:
                    if (list1->Count() > 0 && list2->Count() > 0) {
                        list1->DuplicateAll();
                        list2->DuplicateAll();
                    }
                        break;
            }

            catch (std::exception& e) { // Обработка исключений
                std::cout << "Error: " << e.what() << std::endl; // Вывод сообщения об ошибке
                break;
            }
        }

        std::cout << "Check for equality: " << std::endl; // Вывод проверки на равенство
        std::cout << list1->Count() << std::endl; // Вывод количества элементов в массиве
        std::cout << list2->Count() << std::endl; // Вывод количества элементов в списке цепочек

        for (int i = 0; i < list1->Count(); i++) { // Цикл по количеству элементов в массиве
            if ((*list1)[i] != (*list2)[i]) { // Проверка на неравенство элементов
                std::cout << "Lists are different" << std::endl; // Списки различны
            }
            else {
                std::cout << "Lists are the same" << std::endl; // Списки идентичны
            }
        }

        for (auto list : lists) { // Цикл по вектору списков
            delete list; // Освобождение памяти

        }
        lists.clear(); // Очистка вектора
    }
};

std::vector<BaseList*> Tester::lists; // объявление вектора указателей на объекты класса BaseList
std::random_device Tester::rd; // генератор случайных чисел для тестирования
std::mt19937 Tester::gen; // инициализация генератора случайных чисел
std::uniform_int_distribution<int> Tester::op_dist; // определение распределения операций
std::uniform_int_distribution<int> Tester::val_dist; // определение распределения значений
std::uniform_int_distribution<int> Tester::index_dist; // определение распределения индексов

int main() {
    std::cout << "Dynamic list" << std::endl; // создание объекта класса ArrList и добавление элементов 
    BaseList* dynamicArray = new ArrList();
    dynamicArray->Add(1);
    dynamicArray->Add(2);
    dynamicArray->Add(3);
    dynamicArray->Add(4);
    dynamicArray->Add(5);
    std::cout << "After adding: " << std::endl;
    dynamicArray->Print();
    dynamicArray->Insert(9, 2);
    std::cout << "After insertion: " << std::endl;
    dynamicArray->Print();
    dynamicArray->Delete(1);
    std::cout << "After deletion: " << std::endl;
    dynamicArray->Print();
    dynamicArray->Clear();
    std::cout << "After clearing: " << std::endl;
    dynamicArray->Print();
    dynamicArray->Add(1);
    dynamicArray->Add(2);
    std::cout << "Added two elements" << std::endl;
    dynamicArray->Print();
    dynamicArray->operator[](0) = 9;
    int value = dynamicArray->operator[](0);
    std::cout << "Changed the first element to 9" << std::endl;
    dynamicArray->Print();
    std::cout << "Value of the first element: " << value << std::endl;
    int currentCount = dynamicArray->Count();
    std::cout << "Current count of elements: " << currentCount << std::endl;

    std::cout << "Chain list" << std::endl;
    BaseList* list = new ChainList();
    list->Add(1);
    list->Add(2);
    list->Add(3);
    std::cout << "After adding: " << std::endl;
    list->Print();
    list->Insert(6, 2);
    std::cout << "After insertion: " << std::endl;
    list->Print();
    list->Delete(1);
    std::cout << "After deletion: " << std::endl;
    list->Print();
    list->Clear();
    std::cout << "After clearing: " << std::endl;
    list->Print();
    list->Add(1);
    list->Add(2);
    std::cout << "Added two elements" << std::endl;
    list->Print();
    list->operator[](1) = 45;
    int valueOfChainList = list->operator[](1);
    std::cout << "Changed the second element to 45" << std::endl;
    list->Print();
    std::cout << "Value of the second element: " << valueOfChainList << std::endl;
    int currentCountOfChainList = list->Count();
    std::cout << "Current count of elements: " << currentCountOfChainList << std::endl;

    std::cout << "***TESTING***" << std::endl;
    Tester::Test();

    delete dynamicArray;
    delete list;

    return 0;
}
//виртуальный метод дублирование всех элементов , абстрактный в базовом классе и переопределен в обоих наследниках

