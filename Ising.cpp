#include "Ising.h"

Ising::Ising(char const * fileName)
{
    // load user defined parameters
    ifstream file(fileName);   
    if (file >> this->N
            >> this->L
            >> this->betaLower
            >> this->betaUpper
            >> this->betaStep
            >> this->startParameter
            >> this->id
            >> this->outputConfig
            >> this->NThermal) {
                cout << "Parameters loaded successfully\n";
            } 
    else 
    {
        cout << "Unable to load parameters\n";
    }

    file.close();
};

void Ising::init()
{
    // allocate array sizes based on lattice size parameter
    long arrayDim = this->L * this->L;
    this->latticeSpin.resize(arrayDim);
    this->neighbourArray.resize(arrayDim);
    for (int i = 0; i < arrayDim; i++)
    {
        this->neighbourArray[i].resize(4, 0);
    }

    findNeighbours();
}

void Ising::setInitialConfig()
{
    // set to cold start with value determined by user
    fill(latticeSpin.begin(), latticeSpin.end(), this->startParameter);
    currMag = L * L * startParameter; // start lattice's current magnetism at cold start value
}

void Ising::findNeighbours()
{
    // populate array with appropriate superindeces
    for (int superI = 0; superI < L*L; superI++)
    {
        if (superI - 1 >= 0) neighbourArray[superI][0] = superI - 1;
        else neighbourArray[superI][0] = superI + (L-1);
        
        if (superI + L < L*L) neighbourArray[superI][1] = superI + L;
        else neighbourArray[superI][1] = superI - (L*(L-1));
        
        if (superI + 1 < L*L) neighbourArray[superI][2] = superI + 1;
        else neighbourArray[superI][2] = superI - (L-1);
        
        if (superI - L >= 0) neighbourArray[superI][3] = superI - L;
        else neighbourArray[superI][3] = superI + L*(L-1);
    }
}

void Ising::updateLattice(double currentBeta, int rand1, double rand2)
{
    // choose random spin to flip
    int oppositeSpin = -1 * latticeSpin[rand1];
    
    // sum spins of chosen spin's neighbours
    int neighbourSum = latticeSpin[neighbourArray[rand1][0]] + latticeSpin[neighbourArray[rand1][1]]
                        + latticeSpin[neighbourArray[rand1][2]] + latticeSpin[neighbourArray[rand1][3]];
                        
    int deltaEnergy = latticeSpin[rand1] * neighbourSum * 2; // compute difference in energy

    // acceptance step
    if (exp(-1 * deltaEnergy * currentBeta) > rand2)
    {
        latticeSpin[rand1] = oppositeSpin;
        currMag += 2 * oppositeSpin; // change running tally of lattice magnetization
    }
}

double Ising::calculateMoment(int moment, double mean)
{
    // function to calculate central moments of magnetization
    
    double momentSum = 0;
    for (auto mag : magnetsArray)
    {
        momentSum += pow(mag - mean, moment);
    }

    return (momentSum / static_cast<double>(magnetsArray.size()));
}

void Ising::recordMag(double entry)
{ 
    if (entry == -1) magnetsArray = {};
    else magnetsArray.push_back(entry);
}

void Ising::outputLattice(ofstream& latticeFile)
{
    // loops through lattice and creates text representation 
    for (size_t i = 1; i <= L*L; i++)
    {
        latticeFile << latticeSpin[i-1] << " ";
        if (i % L == 0)
        {
            latticeFile << "\n";
        }
    }
    latticeFile << "\n\n"; // double lines at end of each lattice so gnuplot can parse data
}

double Ising::getBetaLower()
{
    return this->betaLower;
}

double Ising::getBetaUpper()
{
    return this->betaUpper;
}

double Ising::getBetaStep()
{
    return this->betaStep;
}

double Ising::getCurrMag()
{
    return currMag / (static_cast<double>(L) * L);
}

int Ising::getSteps()
{
    return this->N;
}

int Ising::getVolume()
{
    return this->L * this->L;
}

int Ising::getNThermal()
{
    return this->NThermal;
}

bool Ising::getOutputConfig()
{
    return this->outputConfig;
}

string Ising::getID()
{
    return this->id;
}