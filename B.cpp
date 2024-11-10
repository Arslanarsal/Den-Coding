#include <iostream>
using namespace std;

int main()
{
    int rows = 21, columns = 20;
    int m = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (i < 3 || j < 3 || i >= 18 || j >= 17)
            {
                cout << "* ";
            }

            else if (i > 10 && j <= 16 && j >= (8 + m) && j <= (10 + m))
            {
                cout << "  ";
            }

            else if (j == 3 || j == 4 || i == 3 || i == 4 || i == 9 || i == 10 || ((i <= 10 && j == 16) || (i <= 10 && j == 15)))
            {
                cout << "  ";
            }
            else
            {
                cout << "* ";
            }
        }
        if (i > 10)
        {
            m++;
        }

        cout << endl;
    }

    return 0;
}
