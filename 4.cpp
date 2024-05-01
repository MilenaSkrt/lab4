#include <iostream>
#include <vector>
#include <random>

class BaseList {
protected:
    int count = 0;

public:
    int Count() { return count; }
    virtual void Add(int a) = 0;
    virtual void Insert(int a, int pos) = 0;
    virtual void Delete(int pos) = 0;
    virtual void Clear() = 0;

    virtual int& operator[](int i) = 0;

    void Print() {
        for (int i = 0; i < Count(); i++) {
            std::cout << (*this)[i] << " ";
        }
        std::cout << std::endl;
    }

    virtual void AssignTo(BaseList* dest) {
        dest->Assign(this);
    }

    virtual BaseList* Clone() {
        BaseList* clone_list = EmptyClone();
        clone_list->Assign(this);
        return clone_list;
    }

    virtual void Sort() {
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

    virtual bool Equals(BaseList* list) {
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
        Clear();
        if (source == nullptr) {
            return;
        }

        for (int i = 0; i < source->Count(); i++) {
            Add((*source)[i]);
        }
    }

protected:
    virtual BaseList* EmptyClone() = 0;
};

class ArrList : public BaseList {
private:
    int* buf;
    int Length;

public:
    ArrList() {
        Length = 2;
        buf = new int[Length];
        count = 0;
    }

    ~ArrList() {
        delete[] buf;
    }

    void Add(int a) override {
        if (count >= Length) {
            Length *= 2;
            int* newbuf = new int[Length];
            std::copy(buf, buf + count, newbuf);
            delete[] buf;
            buf = newbuf;
        }
        buf[count] = a;
        count++;
    }

    void Insert(int a, int pos) override {
        if (pos < 0 || pos > count) {
            return;
        }

        if (Length == count) {
            Length *= 2;
            int* newbuf = new int[Length];
            std::copy(buf, buf + count, newbuf);
            delete[] buf;
            buf = newbuf;
        }

        for (int i = count; i > pos; i--) {
            buf[i] = buf[i - 1];
        }

        buf[pos] = a;
        count++;
    }

    void Delete(int pos) override {
        if (pos < 0 || pos >= count) {
            return;
        }

        for (int i = pos; i < count - 1; i++) {
            buf[i] = buf[i + 1];
        }

        count--;
    }

    void Clear() override {
        delete[] buf;
        Length = 2;
        buf = new int[Length];
        count = 0;
    }

    int& operator[](int i) override {
        if (i >= count || i < 0) {
            throw std::out_of_range("Element is out of range");
        }
        return buf[i];
    }

protected:
    BaseList* EmptyClone() override {
        return new ArrList();
    }
};

class ChainList : public BaseList {
private:
    struct Node {
        int Data;
        Node* Next;

        Node(int data) : Data(data), Next(nullptr) {}
    };

    Node* head;

public:
    ChainList() : head(nullptr) {}

    void Add(int data) override {
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->Next != nullptr) {
                current = current->Next;
            }
            current->Next = newNode;
        }
        count++;
    }

    void Insert(int data, int pos) override {
        if (pos < 0 || pos > count) {
            return;
        }

        Node* newNode = new Node(data);
        if (pos == 0) {
            newNode->Next = head;
            head = newNode;
        } else {
            Node* current = head;
            for (int i = 0; i < pos - 1; i++) {
                current = current->Next;
            }
            newNode->Next = current->Next;
            current->Next = newNode;
        }
        count++;
    }

    void Delete(int pos) override {
        if (pos < 0 || pos >= count) {
            return;
        }

        Node* current = head;
        if (pos == 0) {
            head = head->Next;
            delete current;
        } else {
            for (int i = 0; i < pos - 1; i++) {
                current = current->Next;
            }
            Node* temp = current->Next;
            current->Next = temp->Next;
            delete temp;
        }
        count--;
    }

    void Clear() override {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->Next;
            delete temp;
        }
        head = nullptr;
        count = 0;
    }

    int& operator[](int i) override {
        Node* current = head;
        for (int j = 0; j < i; j++) {
            if (current != nullptr) {
                current = current->Next;
            } else {
                throw std::out_of_range("Index out of range");
            }
        }
        return current->Data;
    }

protected:
    BaseList* EmptyClone() override {
        return new ChainList();
    }
};

class Tester {
private:
    static std::vector<BaseList*> lists;
    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_int_distribution<int> op_dist;
    static std::uniform_int_distribution<int> val_dist;
    static std::uniform_int_distribution<int> index_dist;

public:
    static void Test() {
        ArrList* list1 = new ArrList();
        ChainList* list2 = new ChainList();

        lists.push_back(list1);
        lists.push_back(list2);

        gen = std::mt19937(rd());
        op_dist = std::uniform_int_distribution<int>(0, 5);
        val_dist = std::uniform_int_distribution<int>(1, 1000);
        index_dist = std::uniform_int_distribution<int>(1, 100);

        for (int i = 0; i < 10000; i++) {
            int operation = op_dist(gen);
            int value = val_dist(gen);
            int index = index_dist(gen);

            try {
                switch (operation) {
                    case 0:
                        list1->Add(value);
                        list2->Add(value);
                        break;
                    case 1:
                        if (list1->Count() > 0 && list2->Count() > 0) {
                            list1->Insert(value, index);
                            list2->Insert(value, index);
                        }
                        break;
                    case 2:
                        if (list1->Count() > 0 && list2->Count() > 0) {
                            list1->Delete(index);
                            list2->Delete(index);
                        }
                        break;
                    case 3:
                        list1->Clear();
                        list2->Clear();
                        break;
                    case 4:
                        if (list1->Count() > 0 && list2->Count() > 0) {
                            (*list1)[index] = value;
                            (*list2)[index] = value;
                        }
                        break;
                    case 5:
                        list1->Sort();
                        list2->Sort();
                        break;
                }
            } catch (std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
                break;
            }
        }

        std::cout << "Check for equality: " << std::endl;
        std::cout << list1->Count() << std::endl;
        std::cout << list2->Count() << std::endl;

        for (int i = 0; i < list1->Count(); i++) {
            if ((*list1)[i] != (*list2)[i]) {
                std::cout << "Lists are different" << std::endl;
            } else {
                std::cout << "Lists are the same" << std::endl;
            }
        }

        for (auto list : lists) {
            delete list;
        }
        lists.clear();
    }
};

std::vector<BaseList*> Tester::lists;
std::random_device Tester::rd;
std::mt19937 Tester::gen;
std::uniform_int_distribution<int> Tester::op_dist;
std::uniform_int_distribution<int> Tester::val_dist;
std::uniform_int_distribution<int> Tester::index_dist;

int main() {
    std::cout << "Dynamic list" << std::endl;
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
