#include <iostream>
#include <bits/stdc++.h>
using namespace std;



int bin_to_int(string s)
{
    int i = 0;
    int n = 0;
    int l = s.length();
    for (int i = 0; i < s.length(); i++)
    {
        if (s.at(l - 1 - i) == '1')
        {
            n = n + (round)(pow(2, i));
        }
    }
    return n;
}



list<int> updateLru(list<int> v, int ser)
{
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        if (*it == ser)
        {
            v.erase(it);
            break;
        }
    }
    v.push_front(ser);
    return v;
}

int main()
{
    int  cache_size_kb,sets, block_size, ways, index_size, addr_size, tag_size, off_size;

    cout<<"Enter cache size in KB : ";
    cin>>cache_size_kb;
    cout<<"Enter block Size : ";
    cin>>block_size;
    cout<<"Enter no of ways: ";
    cin>>ways;
    cout<<"Enter address size : ";
    cin>>addr_size;


    sets=(cache_size_kb*1024)/(ways*block_size);
    index_size=(int)log2(sets);
    off_size=(int)log2(block_size);
    tag_size=addr_size-index_size-off_size;

    string cacheData[sets][ways];
    string cacheTag[sets][ways];
    char cacheValid[sets][ways];
    char cacheDirty[sets][ways];

    int hit_count, miss_count;
    hit_count = 0;
    miss_count = 0;

    list<int> Lru[sets];

    for (int i = 0; i < sets ; i++)
    {
        for (int j = 0; j < ways ; j++)
        {
            Lru[i].push_front(j);
        }
    }


    string addr;
    char read, write;
    read = '1';
 
    ifstream addrFile("LU.txt");
    while (getline(addrFile, addr))
    {
        string tgst = addr.substr(0, tag_size);
        string blcaddrstr = addr.substr(0, tag_size + index_size);
        int tag = bin_to_int(tgst);
        int block_addr = bin_to_int(blcaddrstr);
        int index = block_addr % sets;
        bool hit = false;
        int hit_line;

        for (int i = 0; i < ways - 1; i++)
        {
            if (cacheValid[index][i] == '1' && cacheTag[index][i] == tgst)
            {
                hit_line = i;
                hit=true;
                break;
            }
        }


        if (hit == true)
        {
            hit_count++;
            list<int> upLr = updateLru(Lru[index], hit_line);
            
            
            if (read == '1')
            {
                string s = cacheData[index][hit_line];
                // manipulate to give data output
            }
            else
            {
                cacheDirty[index][hit_line] = '1';
                // and update updated data
            }
        }
        else
        {
            miss_count++;
            int replace_line = Lru[index].back();
            Lru[index].pop_back();
            Lru[index].push_front(replace_line);
            if (cacheValid[index][replace_line] == '1' && cacheDirty[index][replace_line] == '1')
            {
                // write data to memory
            }

            cacheValid[index][replace_line] = '1';
            cacheDirty[index][replace_line] = '0';
            cacheTag[index][replace_line] = tgst;
            // get data form mem and write in cache
            if (read == '1')
            {
                // get data form mem and write in cache
                // no change in dirty bit
            }
            else
            {
                cacheDirty[index][replace_line] = '1';
                // write updated data in chache mem
            }
        }
    }

    cout<<"Total hits = "<<hit_count<<endl;
    cout<<"Total miss = "<<miss_count<<endl;

    return 0;
}
