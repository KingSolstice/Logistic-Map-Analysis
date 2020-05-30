#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
#include <iomanip>
#include <math.h>

std::vector<float> logisticIterator(float lambda,int iterations){
    float x_n = 0.5;
    std::vector<float> last1024;

    //std::cout << x_n << std::endl;
    for(int curIter = 0; curIter<iterations; curIter++){
        x_n = 4*lambda*x_n*(1-x_n);
        
        //std::cout << x_n << std::endl;
        //If there are only N iterations remaining
        if((iterations-curIter)<=500){
            //Check if the current x_n value exists within the lastN vector
            if(std::find(last1024.begin(),last1024.end(),x_n) != last1024.end()){
                //Do nothing if its found
                continue;
            } else {
                //Add it if it doesnt already exist within the vector
                last1024.push_back(x_n);
            }
        }
    }
    return last1024;
}

void fileBuilder(std::vector<float> appendVec, float curLambda){
    std::ofstream outfile;

    outfile.open("logisticmapTest.dat", std::ios_base::app);
    for(int i=0; i<appendVec.size();i++){
        outfile << curLambda << " " << appendVec[i] << std::endl;
    }
    outfile.close();
}

void lambdaSweeper(int iterations){
    float lambda = 0.0;
    float lambdaInc = 0.0001;
    while(lambda<1){
        std::vector<float> appendVec = logisticIterator(lambda,iterations);
        fileBuilder(appendVec,lambda);
        lambda+=lambdaInc;
    }
}

std::vector<float> cleanVector(std::vector<float> dirtyVector,int cycleCount){
    std::vector<float> cleaningVector = dirtyVector;
    
    if(cleaningVector.size()>=64){
        for(int i=0; i<cleaningVector.size(); i++){
            cleaningVector[i] = (floor(cleaningVector[i]*1000000))/1000000;
        }
        std::set<float> s;
        unsigned size = cleaningVector.size();
        for(unsigned i = 0; i < size; i++) s.insert(cleaningVector[i]);
        cleaningVector.assign(s.begin(),s.end());
    }
    if(cleaningVector.size()>=32&&cleaningVector.size()<64){
        for(int i=0; i<cleaningVector.size(); i++){
            cleaningVector[i] = (floor(cleaningVector[i]*100000))/100000;
        }
        std::set<float> s;
        unsigned size = cleaningVector.size();
        for(unsigned i = 0; i < size; i++) s.insert(cleaningVector[i]);
        cleaningVector.assign(s.begin(),s.end());
    }
    if(cleaningVector.size()>=16&&cleaningVector.size()<32){
        for(int i=0; i<cleaningVector.size(); i++){
            cleaningVector[i] = (floor(cleaningVector[i]*10000))/10000;
        }
        std::set<float> s;
        unsigned size = cleaningVector.size();
        for(unsigned i = 0; i < size; i++) s.insert(cleaningVector[i]);
        cleaningVector.assign(s.begin(),s.end());
    }
    if(cleaningVector.size()>=8&&cleaningVector.size()<16){
        for(int i=0; i<cleaningVector.size(); i++){
            cleaningVector[i] = (floor(cleaningVector[i]*1000))/1000;
        }
        std::set<float> s;
        unsigned size = cleaningVector.size();
        for(unsigned i = 0; i < size; i++) s.insert(cleaningVector[i]);
        cleaningVector.assign(s.begin(),s.end());
    }
    if(cleaningVector.size()>=4&&cleaningVector.size()<8){
        for(int i=0; i<cleaningVector.size(); i++){
            cleaningVector[i] = (floor(cleaningVector[i]*100))/100;
        }
        std::set<float> s;
        unsigned size = cleaningVector.size();
        for(unsigned i = 0; i < size; i++) s.insert(cleaningVector[i]);
        cleaningVector.assign(s.begin(),s.end());
    }
    if(cleaningVector.size()>=2&&cleaningVector.size()<4){
        for(int i=0; i<cleaningVector.size(); i++){
            cleaningVector[i] = (floor(cleaningVector[i]*1000))/1000;
        }
        std::set<float> s;
        unsigned size = cleaningVector.size();
        for(unsigned i = 0; i < size; i++) s.insert(cleaningVector[i]);
        cleaningVector.assign(s.begin(),s.end());
    }  

    
    
    return cleaningVector;

}

float findCycle(int cycleCount, float leftBracket, float rightBracket,int iterations){
    
    //Initialize bracket variables 
    float curLeft = leftBracket;
    float curRight = rightBracket;
    float curMid = rightBracket-((curRight-curLeft)/2);
    
    //Iterate over left and right brackets to recieve finalized list
    std::vector<float> rightCycles = logisticIterator(curRight,10000);
    std::vector<float> leftCycles = logisticIterator(curLeft,10000);
    std::vector<float> middleCycles = logisticIterator(curMid,10000);
    rightCycles = cleanVector(rightCycles,cycleCount);
    leftCycles = cleanVector(leftCycles,cycleCount);
    middleCycles = cleanVector(middleCycles,cycleCount);
    
    //Hone in on the period doubling point via the bracketing method
    for(int iter = 0; iter < iterations; iter++){
        if(middleCycles.size()>=cycleCount){
            curRight = curMid;
            curMid = curRight-((curRight-curLeft)/2);
            rightCycles = logisticIterator(curRight,10000);
            middleCycles = logisticIterator(curMid,10000);
            rightCycles = cleanVector(rightCycles,cycleCount);
            middleCycles = cleanVector(middleCycles,cycleCount);
        } else {
            curLeft = curMid;
            curMid = curRight-((curRight-curLeft)/2);
            leftCycles = logisticIterator(curLeft,10000);
            middleCycles = logisticIterator(curMid,10000);
            rightCycles = cleanVector(rightCycles,cycleCount);
            middleCycles = cleanVector(middleCycles,cycleCount);
        }
    }

    //Hybrid aspect that if the bracketing method lands on a point that doesnt have the correct number of bifurcations, it increments until it does
    while(middleCycles.size()!=cycleCount){
        curMid+=0.0000001;
        middleCycles = logisticIterator(curMid,10000);
        //middleCycles = cleanVector(middleCycles,cycleCount);
    }
    std::cout << "Current Lambda: " << cycleCount << " Cycle Stop Size: " << middleCycles.size() << std::endl;
    

    std::ofstream outfile;
    outfile.open("bifurcationFixedPoints.dat", std::ios_base::app);
    for(int cur=0; cur<middleCycles.size();cur++)
        outfile << middleCycles.at(cur) << ' ';
    outfile << std::endl;
    outfile.close();
    return curMid;
}

float errorCalc(float approxVal, float exactVal){


}

void sweepCycles(){
    std::cout << "Sweeping through cycles 2-64..." << std::endl;
    
    float chaos = 0.89248;
    
    
    std::ofstream outfile;
    outfile.open("cycleLocations.dat", std::ios_base::app);
    outfile << findCycle(2,0.0,chaos,1000) << std::endl;
    outfile << findCycle(4,0.0,chaos,1000) << std::endl;
    outfile << findCycle(8,0.0,chaos,1000) << std::endl;
    outfile << findCycle(16,0.0,chaos,1000) << std::endl;
    outfile << findCycle(32,0.0,chaos,1000) << std::endl;
    outfile << findCycle(64,0.0,chaos,1000) << std::endl;
    outfile.close();

}

float feigenbaum(float n2, float n1){
    float chaos = 0.89248;
    return (chaos-n1)/(chaos-n2);
}

float approximateFeigenbaum(int upperBound){
    
    float chaos = 0.89248;

    std::ifstream ifile("cycleLocations.dat", std::ios::in);
    float num = 0.0;
    ifile >> num;
    float cycle2 = num;
    ifile >> num;
    float cycle4 = num;
    ifile >> num;
    float cycle8 = num;
    ifile >> num;
    float cycle16 = num;
    ifile >> num;
    float cycle32 = num;
    ifile >> num;
    float cycle64 = num;

    switch(upperBound){
        case 4:
            return feigenbaum(cycle4,cycle2);
            break;
        case 8:
            return feigenbaum(cycle8,cycle4);
            break;
        case 16:
            return feigenbaum(cycle16,cycle8);
            break;
        case 32:
            return feigenbaum(cycle32,cycle16);
            break;
        case 64:
            return feigenbaum(cycle64,cycle32);
            break;
    }

}

int main(){

    sweepCycles();
    float chaos = 0.89248;
    std::ofstream outfile;
    outfile.open("feigenbaum.dat", std::ios_base::app);
    for(int i=4; i<=64; i*=2){
        outfile << approximateFeigenbaum(i) << std::endl;
    }
    outfile.close();

    std::cout << std::endl;
    std::cout << "+====================================+" << std::endl;
    std::cout << "| Data finished being generated...   |" << std::endl;
    std::cout << "|                                    |" << std::endl;
    std::cout << "| Please run the python script to    |" << std::endl;
    std::cout << "| access and visualize the generated |" << std::endl;
    std::cout << "| data.                              |" << std::endl;
    std::cout << "+====================================+" << std::endl;
    return EXIT_SUCCESS;

}