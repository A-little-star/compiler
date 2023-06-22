int half(int x) {
    return x / 2;
}

void f(int y) {
    int t = half(y);
    return;
}

int main() {
    int x = 1;
    f();
    return half(x);
}