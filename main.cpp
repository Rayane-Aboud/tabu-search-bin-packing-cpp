#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <random>
#include <fstream>
#include <time.h>
#include <utility>
class Bin{
    public:
    
        Bin(int maxBinSize){
            this->maxBinSize = maxBinSize;
            this->spaceFilled = 0;
            this->itemList = std::vector<int>();
            //std::cout<<"bin successfully created !"<<"\n";
            //printBinStatus();
             
        }
        void printBinStatus(){
            std::cout<<"maxSize : "<<maxBinSize<<"\n";
            std::cout<<"space filled : "<< spaceFilled <<"\n";
            this->printBin();
            std::cout<<"\n---------------------------------\n";
        }      
        bool itemFitsInBin(int itemWeight){
            if (this->spaceFilled + itemWeight > this->maxBinSize){
                return false;
            }
            return true;
        }
        int spaceLeftIfFitted(int itemWeight){
            return this->spaceFilled + itemWeight - this->maxBinSize;
        } 
        bool additemToBin(int itemWeight){
            if (this->spaceFilled + itemWeight > this->maxBinSize){
                return false;
            }
            

            this->itemList.push_back(itemWeight);
            this->spaceFilled += itemWeight;

            //std::cout<<"element successfully added to the bin !"<<"\n";
            //printBinStatus();

            return true;
            
        }
        int removeitemFromBin(int index){
            if (index < 0 || index >= itemList.size()) {
                return false; // Index out of range case
            }

            int removedWeight = itemList[index];
            itemList.erase(itemList.begin() + index);
            spaceFilled -= removedWeight;
            
            //std::cout<<"element successfully removed from the bin !"<<"\n";
            //printBinStatus();

            return removedWeight;
        }
        std::vector<int> getItemList() const{
            return this->itemList;
        }
        int getNbrOccItem(int itemTarget) const{
            int nbOcc=0;
            for(auto& item:itemList){
                if(itemTarget==item)
                    nbOcc++;
            }
            return nbOcc;
        }
        void printBin(){
            for(auto& itemWeight:itemList){
                
                std::cout <<itemWeight<<",";
            }
        }
    private:
        int maxBinSize;
        int spaceFilled;
        std::vector<int> itemList;
};

class SolutionRepresentation{
    public:
        SolutionRepresentation(){}
        SolutionRepresentation(const std::vector<Bin>& binList){
            for(auto& bin : binList){//for each bin
                std::set<std::pair<int, int>> binRepresentation = std::set<std::pair<int, int>>();
                for(auto& item:bin.getItemList()){//insert in the bin m
                    binRepresentation.insert(std::make_pair(item, bin.getNbrOccItem(item)));
                }
              
                addRepresentationToSolutionRepresentation(binRepresentation);
               
                
            }

        }
        void addRepresentationToSolutionRepresentation(const std::set<std::pair<int, int>> binRepresentation){
            bool binFound = false;
            
            for(auto it = solutionRepresentation.begin(); it != solutionRepresentation.end(); ++it){
                auto binOccPair = *it;

                if(binRepresentation == binOccPair.first){
                    binOccPair.second+=1;
                    binFound=true;
                    break;
                }
                
                
            }
            if(!binFound){
                    
                    this->solutionRepresentation.insert(std::make_pair(binRepresentation,1));
            }
        }
        void printSolutionRepresentation(){
            for(auto it = solutionRepresentation.begin(); it != solutionRepresentation.end(); ++it){
                auto binOccPair = *it;
                std::cout<<"bin"<<"\n";
                for(auto jt = binOccPair.first.begin();jt!=binOccPair.first.end();jt++){
                    auto itemOccPair=*jt;
                    std::cout<<"item :"<<itemOccPair.first<<" occurence:"<<itemOccPair.second<<"\n";
                }
                std::cout<<"occurence of the bin :"<<binOccPair.second<<"\n";

            }
        }
        std::set<std::pair<std::set<std::pair<int, int>>,int>>getSolutionRepresentation()const{
                return solutionRepresentation;
            }
    private:
        std::set //ensemble de
            <std::pair //tuple of 
                <
                    std::set<std::pair<int, int>>//bin representation
                    ,
                    int //occurence
                >
            >solutionRepresentation;
};


class HeuristicBinPacking
{
    public:
        HeuristicBinPacking(){
            this->maxBinSize = 0;
            this->binList = std::vector<Bin>();
            this->itemList = std::vector<int>();
        }
        HeuristicBinPacking(int maxBinSize) {
            this->maxBinSize = maxBinSize;
            this->binList = std::vector<Bin>();
            this->itemList = std::vector<int>();

        }
        HeuristicBinPacking(int maxBinSize, const std::vector<int>& items)
        : maxBinSize(maxBinSize), binList(std::vector<Bin>()), itemList(items) {
            
        }
        void addBinToList(const Bin& bin){
            this->binList.push_back(bin);
        }
        int getTotalNumberOfBins(){
            return binList.size();
        }
        void printResult(){
            for(auto& bin:binList){
                bin.printBin();
                std::cout <<"\n";
            }
        }
        void firstFit(){
            bool foundBin;
            for(auto& item : itemList){
                foundBin = false;
                for(auto& bin : binList){
                    if(bin.itemFitsInBin(item)){
                        foundBin = true;
                        bin.additemToBin(item);
                        break;
                    }
                }
                if(!foundBin){
                    this->addBinToList(Bin(this->maxBinSize));
                    this->binList[binList.size()-1].additemToBin(item);
                }
                   
            }
        }
        void bestFit(){
            bool foundBin;
            int indexOfbestFit;
            int minSpaceLeft;
            for(auto& item : itemList){
                minSpaceLeft=this->maxBinSize;
                foundBin = false;
                indexOfbestFit=-1;
                for(int i=0;i<binList.size();i++){
                    if(binList[i].itemFitsInBin(item))
                    {
                        foundBin=true;
                        if(minSpaceLeft >= binList[i].spaceLeftIfFitted(item)) {
                            minSpaceLeft =binList[i].spaceLeftIfFitted(item);
                            indexOfbestFit = i;
                        }
                    }
                    
                }

                if(!foundBin){
                    this->addBinToList(Bin(this->maxBinSize));
                    this->binList[binList.size()-1].additemToBin(item);
                }
                else{
                    
                    this->binList[indexOfbestFit].additemToBin(item);
                }
                
            }
        } 
        std::vector<Bin> getBinList(){
            return this->binList;
        }
        std::vector<int>getItemList(){
            return itemList;
        }
    private:
        int maxBinSize;
        std::vector<Bin>binList;
        std::vector<int>itemList;
};


class TabuSearchBinPacking{
    public:
        TabuSearchBinPacking(){
            this->maxBinSize=0;
            this->tabuList = nullptr;
        }
        TabuSearchBinPacking(int maxBinSize, int lenTabuList){
            this->maxBinSize =0;
            if (lenTabuList > 0) {
                this->tabuList = new std::pair<HeuristicBinPacking,SolutionRepresentation>[lenTabuList];
            } else {
                this->tabuList = nullptr;
            }

        }
        TabuSearchBinPacking(int maxBinSize, int lenTabuList, const std::vector<int>& items){
            this->tabuList = new std::pair<HeuristicBinPacking,SolutionRepresentation>[lenTabuList];
            
            this->itemList=items;
            this->lenTabuList = lenTabuList;
            this->maxBinSize =maxBinSize;
        }
        ~TabuSearchBinPacking(){

            if(this->tabuList==nullptr)
                delete[] this->tabuList;
        }



        int tabuSearch(int maxIterationNumber,int maxNbrNeighbour){
                int rejected_solutions =0;
                int indexTabuList = 0;
                std::vector<int> initialItemList=this->itemList;
                HeuristicBinPacking hbp=HeuristicBinPacking(this->maxBinSize,this->itemList);
                hbp.bestFit();
                int minCost = hbp.getTotalNumberOfBins();//premiere cout de la solution
                //pour un nombre d'iteration défini
                for(int iterator = 0; iterator<maxIterationNumber;iterator++){
                    std::cout<<iterator<<"\n";
                    
                
                    //create voisinage de l'ensemble de départ
                    std::vector<HeuristicBinPacking> neighbourSolutionVector= std::vector<HeuristicBinPacking>();
                    
                    //intensification 
                   
                    for(int i = 0 ;i < maxNbrNeighbour;i++){
                        // Create a random number generator shuffle it
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        // Generate two random indices
                        std::uniform_int_distribution<> dist(0, this->itemList.size() - 1);
                        int index1 = dist(gen);
                        int index2 = dist(gen);
                        std::swap(initialItemList[index1], initialItemList[index2]);

                        neighbourSolutionVector.push_back(HeuristicBinPacking(this->maxBinSize,initialItemList));

                    }
                    
                    //evaluation du voisinage
                    for(int i=0;i<maxNbrNeighbour;i++){
                        neighbourSolutionVector[i].bestFit();
                        
                        SolutionRepresentation solution = SolutionRepresentation(neighbourSolutionVector[i].getBinList());
                        
                        if(!isInTabuList(solution)){
                            
                            
                            this->tabuList[indexTabuList]=std::make_pair(neighbourSolutionVector[i],solution);
                            
                            indexTabuList = (indexTabuList + 1) % this->lenTabuList;
                        }
                        else{
                            rejected_solutions++;
                        }

                    }
                    
                    //choose the best solution from the neighbour for the next iteration
                    int sizeNSV=neighbourSolutionVector.size();
                    for(int i=0;i<sizeNSV;i++){
                        if(minCost <= neighbourSolutionVector[i].getTotalNumberOfBins()){
                            initialItemList=neighbourSolutionVector[i].getItemList();
                        }
                    }
                    
                }
                HeuristicBinPacking finalSolution = HeuristicBinPacking(this->maxBinSize,initialItemList);
                finalSolution.bestFit();
                
                std::cout<<"cost of the solution"<<finalSolution.getTotalNumberOfBins()<<"\n";
                return rejected_solutions;       
        }

        bool isInTabuList(const SolutionRepresentation& solution) {
            for (int i = 0; i < lenTabuList; i++) {
                
                if (tabuList[i].second.getSolutionRepresentation() == solution.getSolutionRepresentation()) {
                    return true;
                }
            }   
            return false;
        }
    private:
        std::vector<int> itemList;
        std::pair<HeuristicBinPacking,SolutionRepresentation>* tabuList;
        int lenTabuList;
        int maxBinSize;
};






int main(){

   std::ifstream file("benchMark4heuristics.txt");
    if (!file) {
        std::cerr << "Failed to open file: benchmark.txt" << std::endl;
        return 1; // Return an error code or handle the error case appropriately
    }

    int maxBinSize;
    int lenTabuList;
    std::vector<int> items;

    // Read the values from the file
    file >> maxBinSize >> lenTabuList;
    int item;
    while (file >> item) {
        items.push_back(item);
    }

    int nbIteration =10;
    
    clock_t start_time, end_time;
    {
    TabuSearchBinPacking tabuSearch(maxBinSize, lenTabuList, items);
    start_time = clock();
    std::cout<<tabuSearch.tabuSearch(nbIteration,10);
    end_time = clock(); // Get end time
    double execution_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", execution_time);
    }

    {
    TabuSearchBinPacking tabuSearch(maxBinSize, lenTabuList, items);
    start_time = clock();
    std::cout<<tabuSearch.tabuSearch(nbIteration,100);
    end_time = clock(); // Get end time
    double execution_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", execution_time);
    }    
    {
    TabuSearchBinPacking tabuSearch(maxBinSize, lenTabuList, items);
    start_time = clock();
    std::cout<<tabuSearch.tabuSearch(nbIteration,500);
    end_time = clock(); // Get end time
    double execution_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", execution_time);
    }

    {
    TabuSearchBinPacking tabuSearch(maxBinSize, lenTabuList, items);
    start_time = clock();
    std::cout<<tabuSearch.tabuSearch(nbIteration,1000);
    end_time = clock(); // Get end time
    double execution_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", execution_time);
    }


    {
    TabuSearchBinPacking tabuSearch(maxBinSize, lenTabuList, items);
    start_time = clock();
    std::cout<<tabuSearch.tabuSearch(nbIteration,5000);
    end_time = clock(); // Get end time
    double execution_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", execution_time);
    }
}