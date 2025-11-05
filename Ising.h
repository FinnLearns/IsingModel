#include <stdio.h>
#include <string.h>
#include <cmath>
#include <ctime>
#include <fstream>
// #include <sstream>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
// #include <numeric>
// #include <sys/stat.h>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

void isingGenerate(const char * fileName);

void isingMeasure(const char * fileName);

class Ising 
{
private:
    // parameters loaded from input file
    int N;
    int L;
    double betaLower;
    double betaUpper;
    double betaStep;
    int startParameter;
    string id;
    bool outputConfig;
    int NThermal;

    // storage elements
    vector<int> latticeSpin;
    vector<vector<int>> neighbourArray;
    vector<double> magnetsArray;
    int currMag; // running tally of magnetization of lattice

public:
    Ising(char const * fileName);
    void init();
    void setInitialConfig();
    void findNeighbours();
    void updateLattice(double currentBeta, int rand1, double rand2);
    void recordMag(double magEntry);
    void outputLattice(ofstream& latticeFile);
    double calculateMoment(int moment, double mean);
    
    double getBetaLower();
    double getBetaUpper();
    double getBetaStep();
    double getCurrMag();
    int getSteps();
    int getVolume();
    int getNThermal();
    bool getOutputConfig();
    string getID();
};