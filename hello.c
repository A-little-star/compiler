int main() {
    // ignore me.
    int a = 0;
    int b = 234;
    int i = 100;
    for (int i = 1; i <= 10; i = i + 1) {
        for (int j = 1; j <= 10; j = j + 1) {
            a = a + 1;
            b = b + a;
        }
    }
    return 0;
}