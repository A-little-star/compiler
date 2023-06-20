// /* This is a comment.*/
int main()
{
    // ignore me
    // -1 + 5 * (3 / 2)
    // const int x = 1 + 1;
    int a = 1;
    {
        a = 2;
        int a = 3;
        {
            a = 4;
            int b = 5;
        }
    }
    return 0;
}