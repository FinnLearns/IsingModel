### Ising Model Simulation

## About
The Ising model represents magnetism in a material as the spins of individual moments on a lattice. This program explores how the magnetism changes with temperature using the Metropolis-Hastings algorithm. Read more at finnlearns.netlify.app

## Running
You first need to define the parameters in the ```input.txt``` file:
    Number of lattice updates
    Size of one side of square lattice
    Lower bound of beta values
    Upper bound of beta values
    Step size of beta interval
    Initial lattice configuration (1 or -1)
    String for output file
    Output configuration: 1 for creating configurations and 0 otherwise
    Number of configurations to wait before starting measurements

You can see the included ```input.txt``` file for what this looks like exactly.

You'll want to compile the code using ```-O3```. After compilation you'll generate the lattice configurations by running the code with ```--generate``` as the first argument and ```input.txt``` as the second argument. To measure perform measurements of the data you'll run the code with the arguments ```--measure``` and ```input.txt```. 

To plot the output you'll find ```plot.txt``` inside the ```scripts``` folder. This file is a gnuplot script. To run you first need gnuplot installed, then in the terminal navigate to the scripts folder, activate gnuplot, and type the command ```load 'plot.txt'```. Inside the script there's also a spot marked by a comment to write the name of your output folder that you defined in the input file. The generated plot will appear in the ```results``` folder, which is inside the output folder for the given run.

If you want to generate configuration files for animating gifs, you first need to define which beta values you want configurations generated for. This can be done by writing the desired values inside the ````toAnimate``` vector in ```isingGenerate.cpp```. Note that the desired beta values be included in the beta interval you specified. Once the program is finished running, run ```animate.txt``` in gnuplot the same way you ran the plotting script. You will need to run this script seperately for each beta value that has a configuration, and again you will need to specify the name of your output folder and the exact beta value for which you've generated a configuration. These spots are also labelled by comments in the script. The generated gifs will be placed in the ```results``` folder, which is inside the output folder for the given run.