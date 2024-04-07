#pragma once
#include <vector>
#include <unordered_map>
#include "../graphics/renderable2d.h"
#include <unordered_set>

using namespace sparky;
using namespace graphics;

class DynamicArray
{
private:
    std::vector<Renderable2D*> nums;
    std::unordered_map<Renderable2D*, std::unordered_set<int> > mp;
    //std::vector <Renderable2D*> arr;
    //std::unordered_map <Renderable2D*, int> Map;

public:
    DynamicArray() {};
    void add(Renderable2D* x)
    {
        /*if(Map.find(x) != Map.end())
            return;

        int index = arr.size();
        arr.push_back(x);

        Map.insert(std::pair<Renderable2D*,int>(x, index));*/

        nums.push_back(x);
        int index = nums.size() - 1;
        mp[x].insert(index);
    }

    void remove(Renderable2D* x)
    {
        /*if(Map.find(x) == Map.end())
            return;

        int index = Map.at(x);
        Map.erase(x);

        int last = arr.size() - 1;
        std::swap(arr[index], arr[last]);
        arr.pop_back();

        Map.at(arr[index]) = index;*/

        if (!mp.count(x))
            return;
        int index = *(mp[x].begin());

        int lastIndex = nums.size() - 1;

        mp[x].erase(index);

        if (index != lastIndex) {
            mp[nums[lastIndex]].erase(lastIndex);

            mp[nums[lastIndex]].insert(index);

            std::swap(nums[index], nums[lastIndex]);
        }
        nums.pop_back();

        if (mp[x].size() == 0)
            mp.erase(x);
    }

    int search(Renderable2D* x)
    {
        /*if(Map.find(x) != Map.end())
            return Map.at(x);
        return -1;*/
        return 0;
    }

    int getSize()
    {
        //return arr.size();
        return nums.size();
    }

    std::vector<Renderable2D*> getArr()
    {
        return nums;
    }
};