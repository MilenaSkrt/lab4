 void DuplicateAll() override {
        for (int i = 0; i < count * 2; i += 2) {
            Insert(buf[i], i + 1);
        }

 void DuplicateAll() override {
        Node* current = head;
        int index = 0;
        while (current != nullptr) {
            Insert(current->Data, index + 1);
            current = current->Next;
            index += 2;
        }
