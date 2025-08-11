#include "Maze.h"


// jason le
//constructer
Maze::Maze(mcpp::Coordinate basePoint, int xlen, int zlen, std::vector<std::vector<char>> mazeStructure , bool mode){
    this->basePoint=basePoint;
    if(xlen%2==0){
        this->xlen=xlen+1;
    } else {
        this->xlen=xlen;
    }
    if(zlen%2==0){
        this->zlen=zlen+1;
    } else {
        this->zlen=zlen;
    }    
    this->mazeStructure = mazeStructure;
    TESTING_MODE = mode;
}


//deconstructer
Maze::~Maze()
{    
    mazeStructure.clear();
    origLayout.clear(); 
    origLayoutBlock.clear();
    addedBlocks.clear();
    terrainheight.clear();
    oldPos.clear();
}


int Maze::getLength(){
    return xlen;
}


int Maze::getWidth(){
    return zlen;
}


mcpp::Coordinate Maze::getBasePoint(){
    return basePoint;
}


int Maze::getFloorHeight(){
    return basePoint.y-1;
}


void Maze::Level(){ 
    mcpp::Coordinate loc;
    std::vector<int> innerOrig;
    mcpp::BlockType blockType;
    int blockHeight;
    int i, j;
    for(i = basePoint.x-1; i < basePoint.x + xlen+1;++i){
        for(j=basePoint.z-1;j<basePoint.z + zlen+1;++j){
            blockHeight = mc.getHeight(i, j);
            while(blockHeight-basePoint.y>=0){
                innerOrig.clear();
                loc.x = i;
                loc.y = blockHeight;
                loc.z = j;
                innerOrig.push_back(i);
                innerOrig.push_back(blockHeight);
                innerOrig.push_back(j);
                blockType = mc.getBlock(loc);
                origLayout.push_back(innerOrig);
                origLayoutBlock.push_back(blockType);
                mc.setBlock(loc, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                blockHeight=mc.getHeight(i,j);
            }
            while(basePoint.y-blockHeight-1>0){                
                loc.x = i;
                loc.y = blockHeight+1;
                loc.z = j;
                mc.setBlock(loc, mcpp::Blocks::DIRT);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                addedBlocks.push_back(loc);
                blockHeight = mc.getHeight(i,j);
            }
        }
    }
    ylen = blockHeight;
}


void Maze::Remake(){
    unsigned int i;
    mcpp::BlockType block;
    mcpp::Coordinate loc;
    for(i=0;i<addedBlocks.size();++i){
        loc = addedBlocks.at(i);
        mc.setBlock(loc, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    for(i=0;i<origLayout.size();++i){
        loc.x=origLayout.at(i).at(0);
        loc.y=origLayout.at(i).at(1);
        loc.z=origLayout.at(i).at(2);
        block = origLayoutBlock.at(i);
        mc.setBlock(loc, block);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    addedBlocks.clear();
    origLayout.clear();
    origLayoutBlock.clear();
    terrainheight.clear();
    oldPos.clear();
}


void Maze::mazeMake(){
    int i=-1, j =-1;
    mcpp::Coordinate loc;
    for(auto outer: mazeStructure){
        ++i;
        j=-1;
        for(auto inner: outer){
            ++j;
            if (inner ==Wall){
                loc.x=i+basePoint.x;
                loc.z=j+basePoint.z;
                loc.y=mc.getHeight(loc.x, loc.z)+1;
                mc.setBlock(loc, mcpp::Blocks::ACACIA_WOOD_PLANK);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                addedBlocks.push_back(loc);
                loc.y=mc.getHeight(loc.x, loc.z)+1;
                mc.setBlock(loc, mcpp::Blocks::ACACIA_WOOD_PLANK);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                addedBlocks.push_back(loc);
                loc.y=mc.getHeight(loc.x, loc.z)+1;
                mc.setBlock(loc, mcpp::Blocks::ACACIA_WOOD_PLANK);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                addedBlocks.push_back(loc);
            }
        }
    }
}


std::vector<int> Maze::getExit(){
    int i;
    bool exitFound = false;
    std::vector<int> arrayPos;
    for(i=0;i<xlen;++i){
        if(mazeStructure.at(0).at(i)==openSpace){
            arrayPos.push_back(0);
            arrayPos.push_back(i);
            exitFound = true;
        }
        if(mazeStructure.at(zlen-1).at(i)==openSpace){
            arrayPos.push_back(zlen-1);
            arrayPos.push_back(i);
            exitFound = true;
        }            
    }
    for(i=0;i<zlen;++i){
        if(mazeStructure.at(i).at(0)==openSpace){
            arrayPos.push_back(i);
            arrayPos.push_back(0);
            exitFound = true;
        }
        if(mazeStructure.at(i).at(xlen-1)==openSpace){
            arrayPos.push_back(i);
            arrayPos.push_back(xlen-1);
            exitFound = true;
        }
    }
    if(!exitFound){
        arrayPos.push_back(-1);
        arrayPos.push_back(-1);
    }
    return arrayPos;
}


void Maze::readTerrain(){
    int i, j, height;
    std::vector<int> exitPos = getExit();
    for(i=0;i<xlen;++i){
        std::vector<int> innerTerrain;
        for(j=0;j<zlen;++j){
            if(mazeStructure.at(i).at(j)=='.'){
                height=mc.getHeight(basePoint.x+i,basePoint.z+j)-basePoint.y;
                innerTerrain.push_back(height);
                } else{
                    innerTerrain.push_back(terrainWall);
                }
            }
        terrainheight.push_back(innerTerrain);
    }
}


//floodFIll
void Maze::editMaze(int x, int z, int oldX, int oldZ){
    bool isValid = true;
    std::vector<int> currPos;
    
    currPos.push_back(x);
    currPos.push_back(z);
    for(int i=0;i<oldPos.size();++i){
        if(oldPos.at(i)==currPos){
            isValid=false;
        }
    }
    if(x<0 || x>=xlen || z<0 || z>=zlen || (x==oldX && z==oldZ)){
        isValid=false;
    }
    if(isValid){
        if(terrainheight.at(x).at(z)==terrainWall){
            isValid=false;
        }
    }
    if(isValid){
        if(oldX==-1 && oldZ==-1){
            mazeWithTerrain.at(x).at(z)=openSpace;
            oldPos.push_back(currPos);
            editMaze(x+1, z, x, z);
            editMaze(x, z+1, x, z);
            editMaze(x-1, z, x, z);
            editMaze(x, z-1, x, z);
        } else{
            int heightDiff = terrainheight.at(oldX).at(oldZ)-terrainheight.at(x).at(z);
            if((heightDiff>=-1)&&(heightDiff<=1)){
                mazeWithTerrain.at(x).at(z)=openSpace;
                oldPos.push_back(currPos);
                editMaze(x+1, z, x, z);
                editMaze(x, z+1, x, z);
                editMaze(x-1, z, x, z);
                editMaze(x, z-1, x, z);
            } else{
                terrainheight.at(oldX).at(oldZ)=terrainWall;
                mazeWithTerrain.at(oldX).at(oldZ)=Wall;
            }
        }
    }
}


void Maze::makeMazeEmpty(){
    for(int i=0;i<this->xlen;++i){
        std::vector<char> innerEmpty;
        for(int j=0;j<this->zlen;++j){
            innerEmpty.push_back(Wall);
        }
        mazeWithTerrain.push_back(innerEmpty);
    }
}


void Maze::exitIdentifier(){
    std::vector<int> exitPos = getExit();
    mcpp::Coordinate loc;
    loc.x=basePoint.x+exitPos.at(0);
    loc.z=basePoint.z+exitPos.at(1);
    loc.y=mc.getHeight(loc.x, loc.z)+1;
    mc.setBlock(loc, mcpp::Blocks::BLUE_CARPET);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    addedBlocks.push_back(loc);
}

void Maze::mazeMakeWithTerrain(){
    int i=-1, j =-1;
    mcpp::Coordinate loc;
    for(auto outer: mazeWithTerrain){
        ++i;
        j=-1;
        for(auto inner: outer){
            ++j;
            if (inner ==Wall){
                loc.x=i+basePoint.x;
                loc.z=j+basePoint.z;
                loc.y=mc.getHeight(loc.x, loc.z)+1;
                mc.setBlock(loc, mcpp::Blocks::ACACIA_WOOD_PLANK);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                addedBlocks.push_back(loc);
                loc.y=mc.getHeight(loc.x, loc.z)+1;
                mc.setBlock(loc, mcpp::Blocks::ACACIA_WOOD_PLANK);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                addedBlocks.push_back(loc);
                loc.y=mc.getHeight(loc.x, loc.z)+1;
                mc.setBlock(loc, mcpp::Blocks::ACACIA_WOOD_PLANK);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                addedBlocks.push_back(loc);
            }
        }
    }
}

void:: Maze::clearOldPosAndMaze(){
    oldPos.clear();
    mazeWithTerrain.clear();
}