 void DuplicateAll() override {
        for (int i = 0; i < count * 2; i += 2) {
            Insert(buf[i], i + 1);
        }
