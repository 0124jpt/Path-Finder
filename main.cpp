#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <unordered_set>

class Graph {
private:
    int numNodes;
public:
    Graph(int num):numNodes(num){nodes = new Node[numNodes];}

    class edge{
    public:
        edge(int n, double w=0):dst(n),weight(w){}
        int dst;
        double weight;
        bool operator==(edge other){
            return dst == other.dst;
        }
    };
    class Node {
    public:
        std::vector<edge> edges;
    };

    Node *nodes;

    Graph(const Graph& other){
        numNodes = other.numNodes;
        std::memcpy(nodes,other.nodes,numNodes*sizeof(other.nodes[0]));
    } // copy constructor
   Graph& operator=(const Graph& other){
        if(this == &other) return *this;
        numNodes = other.numNodes;
        std::memcpy(nodes,other.nodes,numNodes*sizeof(other.nodes[0]));
        return *this;
    } // assignment operator


    void addEdge(int v1, int v2, double w=0){
        addEdgeImpl(v1,v2,w);
        addEdgeImpl(v2,v1,w);
    }
    std::vector<edge> getAdjacentTo(int node){
        return nodes[node].edges;
    }
    void addEdgeImpl(int v1, int v2, double w=0){
        std::vector<Graph::edge> &edges = nodes[v1].edges;
        if(std::find(edges.begin(),edges.end(), edge(v2)) == edges.end()){
            nodes[v1].edges.push_back(edge(v2,w));
        }
    }
    void addAdjacentTo(int node, std::vector<edge> adj){
        for(edge e: adj)
            addEdge(node,e.dst,e.weight);
    }

};

class maze{
private:
    std::vector<std::vector<char>> mazeVector;
    int numNodes = 0;
    int startLoc = 0;
    int endLoc = 0;
    void dfs_search(std::set<int> &visitedSet, int currNode){
        if(visitedSet.find(currNode) == visitedSet.end()){
            visitedSet.insert(currNode);
            for(auto i: g.getAdjacentTo(currNode))
                dfs_search(visitedSet, i.dst);
        }
    }
    std::pair<std::map<int,int>,std::set<int>> DijkstraShortestPath(){
        std::map<int,int> distanceMap;
        std::map<int,int> predMap;
        std::set<int> nodeSet;
        std::list<int> unvisitedQueue;
        dfs_search(nodeSet,startLoc);

        for(auto node: nodeSet){
            predMap.insert({node, 0});
            distanceMap.insert({node, 999999});
            unvisitedQueue.push_back(node);
        }

        distanceMap[startLoc] = 0; // start distance = 0;
        int currentVertex = startLoc;
        while(!unvisitedQueue.empty()){
            // visit the node with the minimum distance from the start
            int minNode = *unvisitedQueue.begin();
            for(auto i:unvisitedQueue)
                if(distanceMap[minNode] > distanceMap[i]) minNode = i;
            currentVertex = minNode;
            unvisitedQueue.remove(currentVertex);


            for(auto i:g.getAdjacentTo(currentVertex)){
                double edgeWeight = i.weight;
                double altPath = distanceMap[currentVertex] + edgeWeight;

                if(altPath < distanceMap[i.dst]){
                    distanceMap[i.dst] = altPath;
                    predMap[i.dst] = currentVertex;
                }
            }
        }
        return {predMap,nodeSet};

    }
public:
    Graph &g;
    maze(std::vector<std::vector<char>> vect) : mazeVector(vect), g(*(new Graph(0))){};
    void trim(){
        int firstRowSize = mazeVector[0].size();
        std::vector<std::vector<char>> trimmedMaze;
        for(auto & mazeElement : mazeVector){
            std::vector<char> tempVect;
            for(int j=0; j<firstRowSize; j++){
                tempVect.push_back(mazeElement[j]);
            }
            trimmedMaze.push_back(tempVect);
        }
        mazeVector = trimmedMaze;
    }
    void print(){
        for(auto & mazeRow : mazeVector) {
            for (char mazeElement : mazeRow){
                auto mazeEdges = g.getAdjacentTo(startLoc);
                std::cout << mazeElement;
            }
            std::cout << std::endl;
        }
    }
    void convertToGraph() { // return maze graph
        g = *(new Graph((int) (mazeVector.size() * mazeVector[0].size())));
        int width = mazeVector[0].size();
        for (int i = 0; i < mazeVector.size(); i++) {
            for (int j = 0; j < mazeVector[i].size(); j++) {
                int loc = i * width + j;
                if (mazeVector[i][j] == 's' && !startLoc) {
                    startLoc = loc;
                    if (j < mazeVector[0].size() && mazeVector[i][j + 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc + 1, 1}});
                    } // space to the right
                    if (j > 0 && mazeVector[i][j - 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc - 1, 1}});
                    } // space to the left
                    if (i > 0 && mazeVector[i - 1][j] == ' ') {
                        g.addAdjacentTo(loc, {{loc - width, 1}});
                    } // space above
                    if (i < mazeVector.size() && mazeVector[i + 1][j] == ' ') {
                        g.addAdjacentTo(loc, {{loc + width, 1}});
                    } // space below
                    if (i > 0 && j > 0 && mazeVector[i - 1][j - 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc - width - 1, 1.41}});
                    } // space top left
                    if (i > 0 && j < mazeVector[0].size() && mazeVector[i - 1][j + 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc - width + 1, 1.41}});
                    } // space top right
                    if (i < mazeVector.size() && j > 0 && mazeVector[i + 1][j - 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc + width - 1, 1.41}});
                    } // space bottom left
                    if (i < mazeVector.size() && j < mazeVector[i].size() && mazeVector[i + 1][j + 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc + width + 1, 1.41}});
                    } // space bottom right
                    numNodes++;
                }
                if (mazeVector[i][j] == ' ') {
                    if (j < mazeVector[0].size() && mazeVector[i][j + 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc + 1, 1}});
                    } // space to the right
                    if (j > 0 && mazeVector[i][j - 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc - 1, 1}});
                    } // space to the left
                    if (i > 0 && mazeVector[i - 1][j] == ' ') {
                        g.addAdjacentTo(loc, {{loc - width, 1}});
                    } // space above
                    if (i < mazeVector.size() && mazeVector[i + 1][j] == ' ') {
                        g.addAdjacentTo(loc, {{loc + width, 1}});
                    } // space below
                    if (i > 0 && j > 0 && mazeVector[i - 1][j - 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc - width - 1, 1.41}});
                    } // space top left
                    if (i > 0 && j < mazeVector[0].size() && mazeVector[i - 1][j + 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc - width + 1, 1.41}});
                    } // space top right
                    if (i < mazeVector.size() && j > 0 && mazeVector[i + 1][j - 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc + width - 1, 1.41}});
                    } // space bottom left
                    if (i < mazeVector.size() && j < mazeVector[i].size() && mazeVector[i + 1][j + 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc + width + 1, 1.41}});
                    } // space bottom right
                    numNodes++;
                }
                if (mazeVector[i][j] == 'e') {
                    endLoc = loc;
                    if (j < mazeVector[0].size() && mazeVector[i][j + 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc + 1, 1}});
                    } // space to the right
                    if (j > 0 && mazeVector[i][j - 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc - 1, 1}});
                    } // space to the left
                    if (i > 0 && mazeVector[i - 1][j] == ' ') {
                        g.addAdjacentTo(loc, {{loc - width, 1}});
                    } // space above
                    if (i < mazeVector.size() && mazeVector[i + 1][j] == ' ') {
                        g.addAdjacentTo(loc, {{loc + width, 1}});
                    } // space below
                    if (i > 0 && j > 0 && mazeVector[i - 1][j - 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc - width - 1, 1.41}});
                    } // space top left
                    if (i > 0 && j < mazeVector[0].size() && mazeVector[i - 1][j + 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc - width + 1, 1.41}});
                    } // space top right
                    if (i < mazeVector.size() && j > 0 && mazeVector[i + 1][j - 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc + width - 1, 1.41}});
                    } // space bottom left
                    if (i < mazeVector.size() && j < mazeVector[i].size() && mazeVector[i + 1][j + 1] == ' ') {
                        g.addAdjacentTo(loc, {{loc + width + 1, 1.41}});
                    } // space bottom right
                    numNodes++;
                }
            }
        }
    }
    void outputShortestPath(){
        auto test = DijkstraShortestPath();
        int testLoc = endLoc;
        std::set<int> nodeSet = test.second;
        std::map<int,int> predMap = test.first;
        std::unordered_set<int> pathHash;
        while(testLoc != startLoc){
            pathHash.insert(predMap[testLoc]);
            testLoc = predMap[testLoc];
        }
        std::cout << std::endl;
        for(int i=0; i<mazeVector.size(); i++){
            for (int j=0; j<mazeVector[0].size(); j++){
                if((pathHash.find(i*mazeVector[0].size()+j) != pathHash.end()) && mazeVector[i][j] != 's')
                    std::cout << "*";
                else
                    std::cout << mazeVector[i][j];

            }
            std::cout << std::endl;
        }
    }
    bool getInput{

    };
};

int main(int argc, char **argv) {
    // command line argument check
    if(argc>1){
        std::cout << "Jackson Thomas jpthoma6@ncsu.edu" << std::endl;
        return 0;
    }

    // input
    std::vector<std::vector<char>> inputMaze;
    std::string temp;
    while(true){
        std::getline(std::cin, temp);
        if(temp.empty()) break;
        std::vector<char> tempVect;
        for(char & vectChar : temp) {
            tempVect.push_back(vectChar);
        }
        inputMaze.push_back(tempVect);
    }
    maze userVect(inputMaze); // create a maze from the input
    if(inputMaze.empty()) return 0; // check if nothing is entered
    if(inputMaze.size() == 1){
        userVect.print();
        return 0;
    }  // check if one character is entered

    // output
    userVect.trim();
    userVect.convertToGraph();
    userVect.outputShortestPath();

    return 0;
}

