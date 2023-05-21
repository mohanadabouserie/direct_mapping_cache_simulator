/*
For test 1, input those values in the terminal to understand my point of the test case:
CacheSize = 16 bytes
lineSize = 4 bytes
accessCycles (any value)

For test 2, input those values in the terminal to understand my point of the test case:
CacheSize = 1024 bytes
lineSize = 32 bytes
accessCycles (any value)
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct CacheLine 
{
    bool valid;
    unsigned int tag;
    vector<unsigned int> offsets;
};

class CacheSimulator 
{
private:
    vector<CacheLine> cache;
    unsigned int cacheSize;
    unsigned int lineSize;
    unsigned int accessCycles;
    unsigned int accesses;
    unsigned int hits;
    unsigned int misses;
    unsigned int numLines;
    unsigned int indexBits;
    unsigned int offsetBits;
    unsigned int tagBits;

public:
    CacheSimulator(unsigned int cache_Size, unsigned int line_Size, unsigned int access_Cycles) 
    {
        cacheSize = cache_Size;
        lineSize = line_Size;
        accessCycles = access_Cycles;
        accesses = 0;
        hits = 0;
        misses = 0;
        numLines = cacheSize / lineSize;
        indexBits = log2(numLines);
        offsetBits = log2(lineSize);
        tagBits = 32 - indexBits - offsetBits;

        cache.resize(numLines);
        for (auto& line : cache) 
        {
            line.valid = false;
            line.tag = 0;
            line.offsets.resize(lineSize, 0);
        }
    }

    unsigned int getIndex(unsigned int address) 
    {
        return (address >> offsetBits) & ((1 << indexBits) - 1);
    }

    unsigned int getTag(unsigned int address) 
    {
        return (address >> (indexBits + offsetBits));
    }

    unsigned int getOffset(unsigned int address) 
    {
        return address & ((1 << offsetBits) - 1);
    }

    void accessMemory(unsigned int address) 
    {
        accesses++;
        unsigned int index = getIndex(address);
        unsigned int tag = getTag(address);
        unsigned int offset = getOffset(address);
        auto it = find(cache[index].offsets.begin(), cache[index].offsets.end(), offset);
        if (cache[index].valid && cache[index].tag == tag && it != cache[index].offsets.end()) 
        {
            hits++;
        }
        else 
        {
            misses++;
            if(cache[index].valid && cache[index].tag != tag)
            {
                cache[index].offsets.clear();
            }
            cache[index].valid = true;
            cache[index].tag = tag;
            cache[index].offsets.push_back(offset);
        }
    }

    void simulate(const string& filename) 
    {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) 
        {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        unsigned int address;
        while (inputFile >> address) 
        {
            accessMemory(address);

            cout << "Cache State (After Access " << accesses << "):\n";
            for (unsigned int i = 0; i < numLines; i++) 
            {
                cout << "Line " << i << ": Valid = " << cache[i].valid << " Tag = " << cache[i].tag <<  endl;
            }

            double hitRatio = double(hits) / accesses;
            double missRatio = double(misses) / accesses;
            double AMAT = accessCycles + (missRatio * 100);

            cout << "Total number of accesses: " << accesses << endl;
            cout << "Hit ratio: " << hitRatio << endl;
            cout << "Miss ratio: " << missRatio << endl;
            cout << "Average Memory Access Time (AMAT): " << AMAT << " cycles" << endl;
            cout << endl;
        }

        inputFile.close();
    }
};

void run_code()
{
    unsigned int cacheSize, lineSize;
    int accessCycles;

    cout << "Enter the size of the cache in bytes: ";
    cin >> cacheSize;
    while(cacheSize % 2 != 0)
    {
        cout << "Error! Cache size must be a power of 2" << endl;
        cout << "Enter the size of the cache in bytes: ";
        cin >> cacheSize;
    }
    cout << endl;
    cout << "Enter the line size in bytes: ";
    cin >> lineSize ;
    while(lineSize % 2 != 0)
    {
        cout << "Error! Line size must be a power of 2" << endl;
        cout << "Enter the line size in bytes: ";
        cin >> lineSize;
    }
    cout << endl;
    cout << "Enter the number of access cycles needed to access the cache (An integer from 1 to 10): ";
    cin >> accessCycles;
    while(accessCycles < 1 || accessCycles > 10)
    {
        cout << "Error! Number of access cycles must be between 1 and 10" << endl;
        cout << "Enter the number of access cycles needed to access the cache (An integer from 1 to 10): ";
        cin >> accessCycles;
    }
    cout << endl;

    string filename = "addresses.txt";

    CacheSimulator cache_Simulator(cacheSize, lineSize, accessCycles);
    cache_Simulator.simulate(filename);
}

int main() 
{
    run_code();
    return 0;
}