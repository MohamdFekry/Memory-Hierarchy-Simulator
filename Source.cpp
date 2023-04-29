#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;


vector<string> indecis;
vector<string> offsets;
vector<int> Vbits;
vector<string> tags;

int accesses = 0;
int misses = 0;
int hits = 0;
float HitRatio, MissRatio;

int off_exist;
int for_print = 0;

void printAll(float AMAT)
{
    for_print++;
    cout << "Changes after address number " << for_print << " to be called:\n\n";
    if (off_exist == 1)
    {
        cout << "Index:\t\tOffset:\t\tV:\tTag:\t\tData:\n";

        for (size_t i = 0; i < indecis.size(); i++)
        {
            cout << indecis[i] << "\t\t" << offsets[i] << "\t\t" << 1 << "\t" << tags[i] << "\t\t";
            cout << "Mem[" << tags[i] << indecis[i] << offsets[i] << "]\n";
        }
    }

    else if (off_exist == 0)
    {
        cout << "Index:\tV:\tTag:\tData:\n";

        for (size_t i = 0; i < indecis.size(); i++)
        {
            cout << indecis[i] << "\t" << 1 << "\t" << tags[i] << "\t";
            cout << "Mem[" << tags[i] << indecis[i] << "]\n";
        }
    }

    HitRatio = float(float(float(hits) / float(accesses)) * 100);
    MissRatio = float(float(float(misses) / float(accesses)) * 100);
    cout << "\nOverall number of accesses: " << accesses;
    cout << "\nOverall number of hits: " << hits << ",\tHit ratio: " << HitRatio << "%";
    cout << "\nOverall number of misses: " << misses << ",\tMiss ratio: " << MissRatio << "%";

    cout << "\nThe current Average Memory Access Time (AMAT): " << AMAT << endl << endl << endl;
}

int main()
{
    float AMAT = 0;
    int s, l, cyc;
    cout << "Please, specify the following:\n\n";

    cout << "Please, answer with 1 for yes, or 0 for no.\nDo you want to include an offset? ";
    cin >> off_exist;
    while (off_exist != 0 && off_exist != 1)
    {
        cout << "\nplease enter a valid answer: 0 or 1.\n";
        cout << "Do you want to include an offset? ";
        cin >> off_exist;
    }

    if (off_exist == 1)
    {
        cout << "\nThe total cache size : "; cin >> s;
        "The cache line size: "; cin >> l;
        "The number of cycles needed to access the cache: "; cin >> cyc;

        cout << "\nPlease note that the following is the print of The Meaningful cache lines that have values.\n";
        cout << "The rest of the cache is empty by default so all of their valid bits are zero,\n";
        cout << "and their tags are not defined, and so it doesn't need to be printed.\n\n\n";

        int num_lines = s / l;
        int index_size = log2(num_lines);
        int disp_size = log2(l);
        int tag_size = 32 - disp_size - index_size;

        string word;
        ifstream address("Addresses.txt", ios::binary);
        while (address >> word)
        {
            accesses++;
            string temp_tag, temp_ind, temp_dis;

            temp_tag = word.substr(0, tag_size);
            word.erase(0, tag_size);

            temp_ind = word.substr(0, index_size);
            word.erase(0, index_size);

            temp_dis = word;

            int iterate = -1;
            for (size_t i = 0; i < indecis.size(); i++)
            {
                if (temp_ind == indecis[i])
                {
                    iterate = i;
                    break;
                }
            }

            if (iterate == -1)
            {
                misses++;
                indecis.push_back(temp_ind);
                offsets.push_back(temp_dis);
                tags.push_back(temp_tag);
            }

            else if (iterate != -1)
            {
                if (temp_tag == tags[iterate] && temp_dis == offsets[iterate])
                {
                    hits++;
                }

                else if (temp_tag != tags[iterate] && temp_dis == offsets[iterate])
                {
                    misses++;
                    tags[iterate] = temp_tag;
                }
            }

            // AMAT =  Hit time + Miss rate × Miss penalty; where miss penality is specified to be 100 cycles
            AMAT = float(cyc * hits) + float((float((float(misses) / float(accesses))) * 100));

            printAll(AMAT);
        }

        address.close();
    }

    else if (off_exist == 0)
    {
        cout << "\nThe total cache size : "; cin >> s;
        cout << "The number of cycles needed to access the cache: "; cin >> cyc;

        cout << "\nPlease note that the following is the print of The Meaningful cache lines that have values.\n";
        cout << "The rest of the cache is empty by default so all of their valid bits are zero,\n";
        cout << "and their tags are not defined, and so it doesn't need to be printed.\n\n\n";

        int num_lines = s;
        int index_size = log2(num_lines);
        int tag_size = 5 - index_size;

        string word;
        ifstream address("Addresses.txt", ios::binary);
        while (address >> word)
        {
            accesses++;
            string temp_tag, temp_ind;

            temp_tag = word.substr(0, tag_size);
            word.erase(0, tag_size);

            temp_ind = word;

            int iterate = -1;
            for (size_t i = 0; i < indecis.size(); i++)
            {
                if (temp_ind == indecis[i])
                {
                    iterate = i;
                    break;
                }
            }

            if (iterate == -1)
            {
                misses++;
                indecis.push_back(temp_ind);
                tags.push_back(temp_tag);
            }

            else if (iterate != -1)
            {
                if (temp_tag == tags[iterate])
                {
                    hits++;
                }

                else if (temp_tag != tags[iterate])
                {
                    misses++;
                    tags[iterate] = temp_tag;
                }
            }

            // AMAT =  Hit time + Miss rate × Miss penalty; where miss penality is specified to be 100 cycles
            AMAT = float(cyc * hits) + float((float((float(misses) / float(accesses))) * 100));

            printAll(AMAT);
        }

        address.close();
    }
}