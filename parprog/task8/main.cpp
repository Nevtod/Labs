#include "omp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <string>
#include <iterator>
#include <fstream>

using std::cout;
using std::endl;
using std::vector;
using std::cin;
using std::string;
// bool is_already_sorted(vector<int> arr)
// {
// // auto& it = arr.begin();
// }


void quick_sort(vector<int32_t>::iterator begin, vector<int32_t>::iterator end)
{
    int i = 0, j = end - begin - 1;
    int m = (end - begin) / 2;
    cout << m << endl;
    int32_t pivot = *(begin + m);
    do 
    {
        while (*(begin + i) < pivot) i++;
        while (*(begin + j) > pivot) j--;
        if (i <= j) 
        {
            std::swap(*(begin + i), *(begin + j));
            i++; j--;
        }
    } while (i <= j);
    #pragma omp task firstprivate(begin)
    {
        if (j > 0) quick_sort(begin, begin + j + 1);
    }
    #pragma omp task firstprivate(begin, end)
    {
        if (i < end - begin) quick_sort(begin + i, end);
    }
    #pragma omp taskwait
}

int main (int argc, char* argv[])
{
    int n;
    vector<int32_t> v;
    cout << "Использовать файл для ввода массива? Если да, введите yes: ";
    string cmd;
    cin >> cmd;
    if (cmd == "yes")
    {
        string filename;
        cout << "Введите имя файла для чтения массива: ";
        cin >> filename;

        std::ifstream fin;
        fin.open(filename);
        if (!fin.is_open())
        {
            cout << "Некорректное имя файла" << endl;
            return -1;
        }

        int32_t value;
        while (fin >> value) v.push_back(value);
        fin.close();
    }
    else
    {
        cout << "Введите размер массива: ";
        cin >> n;
        cout << "Введите ваш массив:" << endl;
        int32_t value;
        for (int i = 0; i < n; i++)
        {
            cin >> value;
            v.push_back(value);
        }
    }
    
    cout << "Ваш массив:" << endl;
    for (int32_t val : v)
        cout << val << " ";
    cout << endl;

    // Проверим, не является ли массив отсортированным
    bool notLess = true;
    bool notMore = true;
    for (size_t i = 1; i < v.size(); i++)
    {
        notLess = notLess && (v.at(i - 1) >= v.at(i));
        notMore = notMore && (v.at(i - 1) <= v.at(i));
    }

    if (v.size() == 1)
    {
        cout << "Исходный массив размера 1, он уже отсортирован";
        return 0;
    }
    else if (notLess)
    {
        cout << "Исходный массив уже отсортирован в обратном порядке";
        return 0;    
    }
    else if (notMore)
    {
        cout << "Исходный массив уже отсортирован в прямом порядке";
        return 0;    
    }

    #pragma omp parallel shared(v)
    {
        #pragma omp single nowait
        {
            quick_sort(v.begin(), v.end());
        }
        
        #pragma omp barrier
    } 

    int32_t prev = INT32_MIN;
    cout << "Отсортированный массив: " << endl;
    for (int32_t val : v)
    {
        cout << val << " ";
        if (prev > val)
        {
            cout << "NOTSORTED";
            break;
        }
        prev = val;
    }
}

