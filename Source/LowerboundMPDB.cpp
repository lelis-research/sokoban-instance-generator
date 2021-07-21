#include "stdafx.h"

/* LowerboundMPDB: Calculate the lower bound for a state considering the problem as a independent of the domain. */

namespace ls
{
LowerboundMPDB::LowerboundMPDB(const Instance &instance, const int size, const int LIMIT_TIME)
    : size_(size), times_called_(0), distance_(instance), utils_(instance), build_(false)
{
    mpdb_ = new MPDB(instance, size_, LIMIT_TIME);
}

int LowerboundMPDB::GetLowerbound(const std::vector<int> &state, const Instance &instance, bool random)
{
    times_called_++;

    if (random)
    {
    	return GetLowerboundHeuristicRandomized(instance, state);
    }
    else if (size_ == 2)
    {
    	return GetLowerboundNormalized(instance, state);
    }
    else
    {
    	return GetLowerboundGreedyRandomized(instance, state);
    }

}

LowerboundMPDB::~LowerboundMPDB() {
    if (size_ == 2) {
        delete[] edges_;
        delete[] weights_;
    }
    delete mpdb_;
    if (complement_ == true)
    {
        delete mpdb_complement_;
    }
}

const int LowerboundMPDB::GetLowerboundNormalized(const Instance &instance, const std::vector<int> &state)
{
    unsigned short number_nodes = instance.NumberObjects();
    unsigned short number_edges = number_nodes * (number_nodes - 1)/2;

    if (instance.NumberObjects() % 2 == 1)
    {
        number_edges = number_edges + number_nodes;
        number_nodes = number_nodes + 1;
    }

    PerfectMatching* matching = new PerfectMatching(number_nodes, number_edges);
    struct PerfectMatching::Options options;
    options.verbose = false;
    matching->options = options;

    unsigned short counter = 0;
    for (unsigned short i = 0; i < instance.NumberObjects(); i++)
    {
        for (unsigned short j = i + 1; j < instance.NumberObjects(); j++)
        {
            edges_[counter * 2] = i;
            edges_[counter * 2 + 1] = j;
            if (normalized_pdb_[state[i]][state[j]][state[instance.NumberObjects()]] >= INF)
            {
                delete matching;
                return INF;
            }
            else
            {
                /*std::cout << normalized_pdb_[state[i]][state[j]][state[instance.NumberObjects()]] << std::endl;

                std::vector<int> lstate(3);
                std::cout << state[i] << " " << state[j] << " " << state[instance.NumberObjects()] << std::endl;
                lstate[0] = state[i];
                lstate[1] = state[j];
                lstate[2] = state[instance.NumberObjects()];
                Show::State(lstate, instance);
                getchar();*/

                weights_[counter] = -1 * normalized_pdb_[state[i]][state[j]][state[instance.NumberObjects()]];
                matching->AddEdge(edges_[counter * 2],edges_[counter * 2 + 1], weights_[counter]);
            }
            counter++;
        }
    }

    if ((instance.NumberObjects()) % 2 == 1)
    {
        for (unsigned short i = 0; i < instance.NumberObjects(); i++)
        {
            edges_[counter * 2] = i;
            edges_[counter * 2 + 1] = number_nodes -1;
            weights_[counter] = -1 * square_cost_[state[i]][state[instance.NumberObjects()]];
            matching->AddEdge(edges_[counter * 2],edges_[counter * 2 + 1], weights_[counter]);
            counter++;
        }
    }

    matching->Solve();

    unsigned short lower_bound = -1 * (unsigned short)ComputePerfectMatchingCost(number_nodes, number_edges, edges_, weights_, matching);

    delete matching;

    return lower_bound;
}

const int LowerboundMPDB::GetLowerboundGreedyRandomized(const Instance &instance, const std::vector<int> &state)
{
    short lower_bound = 0;
    for (unsigned long long i = 0; i < combinations_.size(); i++)
    {
        lower_bound = 0;

        for (unsigned long long j = 0; j < combinations_[i].size(); j++)
        {
            lower_bound += square_cost_[state[combinations_[i][j]]][state[state.size() - 1]];
            closed_state_[j] = instance.SquaresToFree(state[combinations_[i][j]]);
            stones_on_[state[combinations_[i][j]]] = true;
        }

        utils_.Reachable(stones_on_, state[state.size() - 1], reachable_, instance);
        closed_state_[closed_state_.size() - 1] = instance.SquaresToFree(utils_.NormalizeManSquare(reachable_,instance));

        for (unsigned long long j = 0; j < combinations_[i].size(); j++)
        {
            stones_on_[state[combinations_[i][j]]] = false;
        }

        int lower_bound_pdb = mpdb_->GetValue(closed_state_);

        if (lower_bound_pdb == INF)
        {
            return INF;
        }
        cost_combinations_[i].first = lower_bound_pdb - lower_bound; //Only the increment.
        cost_combinations_[i].second = i;
    }

    std::sort(cost_combinations_.begin(), cost_combinations_.end(), std::greater<std::pair<int,int>>());

    int n_tries = instance.NumberObjects();

    int tries = n_tries + 1;
    float random_percentage = 0.0;

    int best_lower_bound = 0;
    while (tries > 0)
    {
        tries--;
        used_combination_.assign(used_combination_.size(), false);
        used_stone_.assign(used_stone_.size(), false);
        lower_bound = 0;

        int selected_combination = (int)rand() %  (unsigned long long)((cost_combinations_.size() - 1) * random_percentage  + 1);
        lower_bound += (int)cost_combinations_[selected_combination].first;

        for (unsigned long long j = 0; j < combinations_[cost_combinations_[selected_combination].second].size(); j++)
        {
            used_stone_[combinations_[cost_combinations_[selected_combination].second][j]] = true;
            for (unsigned long long w = 0; w < stone_in_combination_[combinations_[cost_combinations_[selected_combination].second][j]].size(); w++)
            {
                used_combination_[stone_in_combination_[combinations_[cost_combinations_[selected_combination].second][j]][w]] = true;
            }
        }

        for (unsigned long long i = 0; i < cost_combinations_.size(); i++)
        {
            if (used_combination_[cost_combinations_[i].second] == false)
            {

                lower_bound += (int)cost_combinations_[i].first;

                for (unsigned long long j = 0; j < combinations_[cost_combinations_[i].second].size(); j++)
                {
                    used_stone_[combinations_[cost_combinations_[i].second][j]] = true;
                    for (unsigned long long w = 0; w < stone_in_combination_[combinations_[cost_combinations_[i].second][j]].size(); w++)
                    {
                        used_combination_[stone_in_combination_[combinations_[cost_combinations_[i].second][j]][w]] = true;
                    }
                }
            }
        }

        for (int i = 0; i < used_stone_.size(); i++)
        {
            lower_bound += square_cost_[state[i]][state[state.size() - 1]];
        }

        //Complement penalty.
        if (complement_)
        {
            int stone_counter = 0;
            int base_value = 0;
            for (unsigned long long i = 0; i < used_stone_.size(); i++)
            {
                if (used_stone_[i] == false)
                {
                    closed_complement_state_[stone_counter] = instance.SquaresToFree(state[i]);
                    base_value += square_cost_[state[i]][state[state.size() - 1]];
                    stone_counter++;
                    stones_on_[state[i]] = true;
                }
            }

            utils_.Reachable(stones_on_, state[state.size() - 1], reachable_,instance);
            closed_complement_state_[closed_complement_state_.size() - 1] = instance.SquaresToFree(utils_.NormalizeManSquare(reachable_, instance));

            for (unsigned long long i = 0; i < used_stone_.size(); i++)
            {
                if (used_stone_[i] == false)
                {
                    stones_on_[state[i]] = false;
                }
            }

            lower_bound += mpdb_complement_->GetValue(closed_complement_state_) - base_value;
        }
        random_percentage += (float)1.0 /(float)n_tries;
        best_lower_bound = std::max((int)best_lower_bound, (int)lower_bound);
    }

    return best_lower_bound;
}

const int LowerboundMPDB::GetLowerboundHeuristicRandomized(const Instance &instance, const std::vector<int> &state )
  {
     int best_lower_bound = 0;
     std::copy(state.begin(), state.end(), random_state_.begin());

     int tries = instance.NumberObjects();
     //std::random_shuffle(random_state_.begin(), random_state_.end() - 1); //shuffles the partition states

     while (tries > 0)
     {
        tries--;

        int random_lower_bound = 0;
        int segments = 0;

        while (segments + size_ <= instance.NumberObjects())
        {
           std::sort(random_state_.begin() + segments, random_state_.begin() + segments + size_);

           int j = 0;
           for (int i = segments; i < segments + size_; i++)
           {
              closed_state_[j] = instance.SquaresToFree(random_state_[i]);
              stones_on_[random_state_[i]] = true;
              j++;
           }

           utils_.Reachable(stones_on_, random_state_[random_state_.size() - 1], reachable_, instance);
           closed_state_[closed_state_.size() - 1] = instance.SquaresToFree(utils_.NormalizeManSquare(reachable_, instance));

           for (int j = 0; j < size_; j++)
           {
              stones_on_[random_state_[segments + j]] = false;
           }

           random_lower_bound += mpdb_->GetValue(closed_state_);
           if (random_lower_bound > INF)
           {
              return INF;
           }
           segments = segments + size_;
        }

        if (complement_)
        {
           std::sort(random_state_.begin() + segments, random_state_.end() - 1);
           int j = 0;
           for (int i = segments; i < random_state_.size() - 1; i++)
           {
              closed_complement_state_[j] = instance.SquaresToFree(random_state_[i]);
              stones_on_[random_state_[i]] = true;
              j++;
           }

           utils_.Reachable(stones_on_, random_state_[random_state_.size() - 1], reachable_, instance);
           closed_complement_state_[closed_complement_state_.size() - 1] = instance.SquaresToFree(utils_.NormalizeManSquare(reachable_, instance));

           for (int i = segments; i < random_state_.size() - 1; i++)
           {
              stones_on_[random_state_[i]] = false;
           }
           random_lower_bound += mpdb_complement_->GetValue(closed_complement_state_);
        }
        else
        {
           if (segments < instance.NumberObjects())
           {
              random_lower_bound += square_cost_[random_state_[segments]][random_state_[random_state_.size() - 1]];
           }
        }
        best_lower_bound = std::max(best_lower_bound, random_lower_bound);
        std::random_shuffle(random_state_.begin(), random_state_.end() - 1);
     }
     return best_lower_bound;
  }


bool LowerboundMPDB::Initiate(const Instance &instance, const int LIMIT_TIME)
{
    if (size_ > instance.NumberObjects())
    {
        return false;
    }

    build_ = false;

    if (size_ == 2) {
        computation_lowerbound = ls::TypeComputationLowerbound::TypePerfect;
    }
    else {
        computation_lowerbound = ls::TypeComputationLowerbound::TypeGreedyRandom;
    }

    if (mpdb_->Initiate(instance) == true)
    {
        if (mpdb_->Solve(instance, LIMIT_TIME) == true)
        {
            build_ = true;
        }
    }

    if (build_ == true)
    {
        if (size_ == 2)
        {
            InitiateNormalized(instance);
            InitiateNonNormalized(instance);
        }
        else
        {
            InitiateNonNormalized(instance);
        }
    }

    ComputeSquareCost(instance);

    //Complement MPDB.
    complement_ = false;
    if (instance.NumberObjects() % size_ > 1)
    {
        mpdb_complement_ = new MPDB(instance, instance.NumberObjects() % size_, LIMIT_TIME);
        if (mpdb_complement_->Initiate(instance) == true)
        {
            if (mpdb_complement_->Solve(instance, LIMIT_TIME) == true){
                complement_ = true;
                closed_complement_state_ =  std::vector<unsigned char>(instance.NumberObjects() % size_ + 1);
            }
        }
        if (complement_ == false)
        {
            delete mpdb_complement_;
        }
    }

    closed_state_.assign(closed_state_.size(), instance.SquaresToFree(instance.GetEntrance()));

    return build_;
}

void LowerboundMPDB::ComputeSquareCost(const Instance &instance)
{
    square_cost_ = std::vector<std::vector<unsigned short>>(instance.Size(), std::vector<unsigned short>(instance.Size(), INF));
    for (unsigned short stone = 0; stone <  instance.Size(); stone++)
    {
        for (unsigned short man = 0; man < instance.Size() ; man++)
        {
            if (instance.FreeSquaresOn(stone) == true && instance.FreeSquaresOn(man) == true && man != stone)
            {
                for (unsigned short goal = 0; goal < instance.NumberObjects(); goal++)
                {
                    square_cost_[stone][man] = std::min((unsigned short)square_cost_[stone][man], (unsigned short)distance_.GetDistance(stone, instance.GoalSquares(goal), man));
                }
            }
        }
    }
}

void LowerboundMPDB::InitiateNonNormalized(const Instance &instance)
{
    utils_.GetCombinations(size_, combinations_, stone_in_combination_, instance);
    cost_combinations_ = std::vector<std::pair<long, long>>(combinations_.size());
    used_combination_ = std::vector<bool>(combinations_.size(), false);
    used_stone_ = std::vector<bool>(instance.NumberObjects(), false);
    closed_state_ = std::vector<unsigned char>(size_ + 1, -1);
    random_state_ = std::vector<int>(instance.NumberObjects() + 1);
    stones_on_ = std::vector<bool>(instance.Size(), false);
    reachable_ = std::vector<bool>(instance.Size(), false);
}

void LowerboundMPDB::InitiateNormalized(const Instance &instance)
{
    normalized_pdb_ = mpdb_->Normalize(instance);

    int number_nodes = instance.NumberObjects();
    int number_edges = number_nodes * (number_nodes - 1) / 2;

    if (number_nodes % 2 == 1)
    {
        number_edges = number_nodes + number_edges;
        number_nodes = number_nodes + 1;
    }

    edges_ = new int[2 * number_edges];
    weights_ = new int[number_edges];
}
}
