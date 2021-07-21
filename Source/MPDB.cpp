#include "stdafx.h"

namespace ls
{
MPDB::MPDB(const Instance &instance, const int size, const int LIMIT_TIME) : utils_(instance),
    expanded_nodes_(0), size_(size), cost_(0), closed_state_(size + 1)
{
    father_ = std::vector<int>(size, 0);
    father_reachable_ = std::vector<bool>(instance.Size(), false);
    child_ = std::vector<int>(size + 1, 0);
    child_reachable_ = std::vector<bool>(instance.Size(), false);
}

const bool MPDB::Initiate(const Instance &instance)
{
    std::vector<std::vector<long>> combinations;
    std::vector<std::vector<long>> t;
    utils_.GetCombinations(size_, combinations, t, instance);

    std::vector<int> state;
    for (unsigned short i = 0; i < combinations.size(); i++)
    {
        state.clear();
        stones_on_.assign(instance.Size(), false);

        //All initial placements for stones.
        for (unsigned short j = 0; j < combinations[i].size(); j++)
        {
            state.push_back(instance.GoalSquares(combinations[i][j]));
            stones_on_[instance.GoalSquares(combinations[i][j])] = true;
        }

        //All reachable zones for man.
        std::vector<int> reachable_zones(instance.Size(), -1);
        utils_.ReachableZones(reachable_zones, stones_on_, instance);
        unsigned short man = 0;

        for (unsigned short j = 0; j < instance.Size(); j++)
        {
            if (reachable_zones[j] == man)
            {
                state.push_back(j);
                for (unsigned short k = 0; k < state.size(); k++)
                {
                    closed_state_[k] = (unsigned char)instance.SquaresToFree(state[k]);
                }

                open_set_.push(std::pair<int, std::vector<int>>(cost_, state));
                closed_set_[closed_state_] = 0;
                state.pop_back();
                man++;
            }
        }
    }

    return true;
}

const bool MPDB::Solve(const Instance &instance, const int LIMIT_TIME)
{
    stones_on_.assign(instance.Size(), false);

    while (open_set_.size() > 0)
    {
        expanded_nodes_++;

        /*if (time.GetDuration() > LIMIT_TIME || getCurrentRSS() > getMemorySize() * MEMORY_FACTOR)
          {
             std::cout << "IPDB: " << GetValue() << " " << size_ << " " << closed_set_.size() << " " << time.GetDuration() << " " << std::endl;
             return false;
          }*/

        std::pair<int, std::vector<int>> node = open_set_.front();
        open_set_.pop();
        father_ = node.second;
        cost_ = node.first;

        //Show::State(father_, instance);
        //std::cout << "cost " << cost_ << std::endl;
        //getchar();

        GenerateSuccessors(instance);

        //std::cout << "open_set_:" << open_set_.size() << std::endl;
    }

    //std::cout << " pdb_nodes " << expanded_nodes_;
    //std::cout << " size " << size_ << " nodes " << expanded_nodes_ << std::endl;
    //getchar();
    stones_on_.assign(stones_on_.size(), false);

    return true;
}

//Generate the successors and put them on closed list
void MPDB::GenerateSuccessors(const Instance &instance)
{
    for (int i = 0; i < father_.size() - 1; i++)
    {
        stones_on_[father_[i]] = true;
    }

    utils_.Reachable(stones_on_, father_[father_.size() - 1], father_reachable_, instance);

    int stone;
    int man;
    for (int i = 0; i < father_.size() - 1; i++)
    {
        for (int j = 0; j < instance.SizeMovesMan(); j++)
        {
            stone = father_[i] + instance.ReverseMove(j);
            man = father_[i] + instance.ReverseMan(j);

            if (stone >= instance.Size() || man >= instance.Size()) continue;

            bool can_move = utils_.CanReverseMove(stone, man, father_reachable_, instance);

            if (can_move)
            {
                std::copy(father_.begin(), father_.end(), child_.begin());

                child_[i] = stone;
                stone_before_ = father_[i];
                stone_after_ = child_[i];

                stones_on_[stone_before_] = false;
                stones_on_[stone_after_] = true;

                utils_.Reachable(stones_on_, man, child_reachable_, instance);

                child_[child_.size() - 1] = utils_.NormalizeManSquare(child_reachable_, instance);

                stones_on_[stone_before_] = true;
                stones_on_[stone_after_] = false;

                Improve(instance);
            }
        }
    }

    for (int i = 0; i < father_.size() - 1; i++)
    {
        stones_on_[father_[i]] = false;
    }
}

void MPDB::Improve(const Instance &instance)
{
    std::sort(child_.begin(), child_.end() - 1);
    for (unsigned short k = 0; k < child_.size(); k++)
    {
        closed_state_[k] = instance.SquaresToFree(child_[k]);
    }

    if (closed_set_.find(closed_state_) == closed_set_.end())
    {
        open_set_.push(std::pair<int, std::vector<int>>(cost_+1, child_));
        closed_set_[closed_state_] = cost_+1;
    }
}

MPDB::~MPDB()
{
    while (!open_set_.empty()) open_set_.pop();
    closed_set_.clear();
}

const int MPDB::GetValue( const std::vector<unsigned char> &state)
{
    if (closed_set_.find(state) == closed_set_.end())
    {
        return INF;
    }
    return closed_set_[state];
}

std::vector<std::vector<std::vector<int>>> MPDB::Normalize(const Instance &instance)
{
    std::vector<std::vector<std::vector<int>>> pdb_ = std::vector<std::vector<std::vector<int>>>(instance.Size(), std::vector<std::vector<int>>(instance.Size(), std::vector<int>(instance.Size(), INF)));
    stones_on_.assign(instance.Size(), false);
    father_reachable_.assign(instance.Size(), false);
    for (auto j = closed_set_.begin(); j != closed_set_.end(); ++j)
    {
        int stone_one = instance.FreeToSquares(j->first[0]);
        int stone_two = instance.FreeToSquares(j->first[1]);
        int man = instance.FreeToSquares(j->first[2]);

        /*std::vector<int> lstate(3);
          std::cout << stone_one << " " << stone_two << " " << man << std::endl;
          lstate[0] = stone_one;
          lstate[1] = stone_two;
          lstate[2] = man;
          Show::State(lstate, instance);
          getchar();*/

        if (pdb_[stone_one][stone_two][man] > j->second || pdb_[stone_two][stone_one][man] > j->second)
        {
            for (int i = 0; i < j->first.size() - 1; i++)
            {
                stones_on_[instance.FreeToSquares(j->first[i])] = true;
            }
            utils_.Reachable(stones_on_, man, father_reachable_, instance);

            pdb_[stone_one][stone_two][man] = std::min(pdb_[stone_one][stone_two][man], j->second);
            pdb_[stone_two][stone_one][man] = std::min(pdb_[stone_two][stone_one][man], j->second);
            for (size_t i = 0; i < instance.Size(); i++)
            {
                if (father_reachable_[i])
                {
                    pdb_[stone_one][stone_two][i] = std::min(pdb_[stone_one][stone_two][i], j->second);
                    pdb_[stone_two][stone_one][i] = std::min(pdb_[stone_two][stone_one][i], j->second);
                }
            }

            for (size_t i = 0; i < j->first.size() - 1; i++)
            {
                stones_on_[instance.FreeToSquares(j->first[i])] = false;
            }
        }
    }
    return pdb_;
}
}
