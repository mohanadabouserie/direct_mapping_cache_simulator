#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

struct CacheLine 
{
    bool valid;
    unsigned int tag;
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

    void initializeCache() 
    {
        numLines = cacheSize / lineSize;
        indexBits = log2(numLines);
        offsetBits = log2(lineSize);
        tagBits = 32 - indexBits - offsetBits;

        cache.resize(numLines);
        for (auto& line : cache) 
        {
            line.valid = false;
            line.tag = 0;
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

public:
    CacheSimulator(unsigned int cacheSize, unsigned int lineSize, unsigned int accessCycles) : cacheSize(cacheSize), lineSize(lineSize), accessCycles(accessCycles), accesses(0), hits(0), misses(0) 
    {
        initializeCache();
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
            ++accesses;
            unsigned int index = getIndex(address);
            unsigned int tag = getTag(address);

            if (cache[index].valid && cache[index].tag == tag) 
            {
                ++hits;
            } 
            else 
            {
                ++misses;
                cache[index].valid = true;
                cache[index].tag = tag;
            }

            // Output cache state after each access
            cout << "Cache State (After Access " << accesses << "):\n";
            for (unsigned int i = 0; i < numLines; ++i) 
            {
                cout << "Line " << i << ": Valid = " << cache[i].valid << " Tag = " << cache[i].tag << endl;
            }

            double hitRatio = double(hits) / accesses;
            double missRatio = double(misses) / accesses;
            double amat = accessCycles + (missRatio * 100);

            cout << "Total number of accesses: " << accesses << endl;
            cout << "Hit ratio: " << fixed << setprecision(2) << hitRatio << endl;
            cout << "Miss ratio: " << fixed << setprecision(2) << missRatio << endl;
            cout << "Average Memory Access Time (AMAT): " << fixed << setprecision(2) << amat << " cycles" << endl;
            cout << endl;
        }

        inputFile.close();
    }
};

void run_code()
{
    unsigned int cacheSize, lineSize, accessCycles;

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

    string filename = "addresses.txt";

    CacheSimulator cacheSimulator(cacheSize, lineSize, accessCycles);
    cacheSimulator.simulate(filename);
}

int main() 
{
    run_code();
    return 0;
}
