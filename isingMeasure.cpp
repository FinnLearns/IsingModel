#include "Ising.h"

void isingMeasure(const char * fileName)
{
    // declare instance of Ising and get class parameters
    Ising myMeasure(fileName);
    string id = myMeasure.getID();
    int NThermal = myMeasure.getNThermal();
    
    int thermCounter = 0; // to count past thermalization time

    // create path to magnetization values and create directory to store measurement results
    fs::path magPath = id + "/magnetizations/";
    fs::path resultsFolder = id + "/results";
    fs::create_directories(resultsFolder);
    fs::path resultsPath = resultsFolder / "results.txt";
    string line;
    ofstream resultsFile(resultsPath);

    if (!(fs::exists(magPath)))
    {
        cout << "Invalid path to magnetizations\n";
        exit(0);
    } else
    {
        for (const auto& entry : fs::directory_iterator(magPath))
        {
            myMeasure.recordMag(-1); // reset array
            const auto file = entry.path();
            ifstream magFile(file);
            if (magFile.is_open())
            {
                thermCounter = 0;
                
                // load magnetization values for given beta
                while (getline(magFile, line))
                {
                    if (thermCounter > NThermal)
                    {
                        double doubleLine = stod(line);
                        myMeasure.recordMag(doubleLine);
                    }
                    thermCounter += 1;
                }
            } else
            {
                cout << "Could not open magentization file, exiting program...";
                exit(0);
            }

            // perform calculations
            double mean = myMeasure.calculateMoment(1, 0);
            double mu_2 = myMeasure.calculateMoment(2, mean);
            double mu_3 = myMeasure.calculateMoment(3, mean);
            double mu_4 = myMeasure.calculateMoment(4, mean);
            double skewness = mu_3 / static_cast<double>(pow(mu_2, 1.5));
            double kurtosis = mu_4 / static_cast<double>(pow(mu_2, 2));
            double susceptibility = mu_2 * myMeasure.getVolume();

            string temperature = entry.path().stem().string();

            if (resultsFile.is_open())
            {
                resultsFile << temperature << " " << abs(mean) << " " << susceptibility 
                            << " " << skewness << " " << kurtosis << endl; 
            } else
            {
                cout << "Could not open results file, exiting program...";
                exit(0);
            }
        }
        resultsFile.close();
    }
}