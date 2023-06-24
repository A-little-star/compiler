int x, y;
int sum;

void sum(int n) {
    for (int i = 1; i <= n; i = i + 1 ) {
        for (int j = 1; j <= n; j = j + 1 ) {
            for (int k = 1; k <= n; k = k + 1 )
                sum = (sum + x) * y;
        }
    }
}

int main() {
    x = 1;
    y = 2;
    sum(999);
    return 0;
}