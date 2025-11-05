#include "Ising.h"

void isingGenerate (char const * fileName)
{
    Ising myGen(fileName); // declare instantiation of Ising class
    myGen.init();
    
    // retrieve and declare variables
    double betaLower = myGen.getBetaLower();
    double betaUpper = myGen.getBetaUpper();
    double betaStep = myGen.getBetaStep();
    int N = myGen.getSteps();
    int volume = myGen.getVolume();
    bool outputConfig = myGen.getOutputConfig();
    string id = myGen.getID();
    
    // multiplier to convert doubles to integers
    const long MULT = pow(10,6);

    // random number generator random number variables
    minstd_rand engine;
    int rand1;
    double rand2;

    // storage elements for magnetization of each configuration
    double magnet;
    vector<double> magnetArray;
    
    // vector that stores beta values to output a configuration file for gif generation
    vector<double> toAnimate = {};
    for (auto& a : toAnimate) a = static_cast<long>(a*MULT); // convert to integers for comparison with current beta
    bool found = false; // flag for if current beta is in toAnimate
    
    // create a directory to store magnetization files for each beta
    fs::path magDir = id + "/magnetizations";
    fs::create_directories(magDir);

    // define path to directory to store configurations
    ofstream latticeFile;
    fs::path configsDir = id + "/configurations";
    fs::path configsPath;
    if (outputConfig) fs::create_directories(configsDir); // only create if user sets outputConfig
    
    
    // enter beta loop, looping through beta values in user defined range and step sizes
    for (double b = betaLower; b <= betaUpper; b+= betaStep)
    {
        // reset flag
        found = false;
        if (outputConfig) 
        /* If user sets outputConfig for every beta value compare current beta with every value user wants animated
        If found set found flag to true, create configurations file, and open file*/
        {
            auto look = find(toAnimate.begin(), toAnimate.end(), static_cast<long>(b*MULT));
            if (look != toAnimate.end())
            {
                found = true;   
                configsPath = configsDir / (to_string(b) + ".txt");
                latticeFile.open(configsPath);
            }
        }
        
        // clear array and reset lattice to cold start
        magnetArray = {};
        myGen.setInitialConfig();
            
        for(size_t n = 0; n < N; ++n)
        {
            // if beta value is one to animate load lattice configurations into latticeFile 
            if (outputConfig && found) myGen.outputLattice(latticeFile);

            magnet = myGen.getCurrMag(); // get average magnetization of configuration
            magnetArray.push_back(magnet); // store average magnetization of configuration
            
            for (size_t l = 0; l < volume; ++l)
            {
                rand1 = engine() % volume; // random integer in (0,volume-1)
                rand2 = static_cast<double>(engine()) / RAND_MAX; // random double in (0,1)
                myGen.updateLattice(b, rand1, rand2); // perform Metropolis-Hastings algorithm
            }
        }

        latticeFile.close();
        
        // generate output file of magnetizations for current beta
        fs::path resultsPath = magDir / (to_string(b) + ".txt");
        ofstream resultsFile(resultsPath);

        if (resultsFile.is_open())
        {
            // load in magnetization values for each recorded configuration
            for (auto entry : magnetArray)
            {
                resultsFile << entry << endl;
            }

            resultsFile.close();
        } else
        {
            cout << "Could not open file, exiting program...";
            exit(0);
        }
    }
}