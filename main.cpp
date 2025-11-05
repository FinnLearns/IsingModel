#include "Ising.h"

int main(int argc, char const * argv[])
{
    if (strcmp(argv[1], "--generate") == 0)
    {
        isingGenerate(argv[2]);
    } 
    else if (strcmp(argv[1], "--measure") == 0)
    {
        isingMeasure(argv[2]);
    } 
    else 
    {
        cout << "Mode " << argv[1] << " not recognized\n";
    }

    return 0;
}