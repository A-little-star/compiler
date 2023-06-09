#include <vector>
#include <iostream>
#include <cstdio>

using namespace std;

int main()
{
    vector<int> b;
    b.push_back(1);
    b.push_back(2);
    vector<int> a = b;
    for (int i = 0; i < a.size(); i ++ )
        printf("%d ", a[i]);

    puts("");
    return 0;
}