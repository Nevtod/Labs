#include "omp.h"
#include <iostream>

using std::cout;
using std::endl;


// This program meets additional conditions.
// It prints threads IDs in backward order
int main ()
{
    int curProcessID = omp_get_max_threads() - 1;
    cout << "curProcessID = " << curProcessID << endl;
    #pragma omp parallel
    {
        int myid = omp_get_thread_num();
        while (myid < curProcessID);

        cout << myid << endl;
        curProcessID--;
    }
}