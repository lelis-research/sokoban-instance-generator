#pragma once

namespace ls {
enum TypeLowerbound
{
	TypeIPDB,
	TypeEMM
};

enum TypeComputationLowerbound
{
	TypeGreedy,
	TypeGreedyRandom,
	TypeHeuristicRandom,
	TypePerfect
};

class LowerboundMPDB {
public:
    LowerboundMPDB(const Instance &instance, const int size, const int LIMIT_TIME);
    virtual ~LowerboundMPDB();

    int GetLowerbound(const std::vector<int> &state, const Instance &instance, bool random = false);
    bool Initiate(const Instance &instance, const int LIMIT_TIME);
protected:
    const int GetLowerboundGreedyRandomized(const Instance &instance, const std::vector<int> &state);
    const int GetLowerboundNormalized(const Instance &instance, const std::vector<int> &state);

    const int GetLowerboundHeuristicRandomized(const Instance &instance, const std::vector<int> &state);

    //Used for the general matching computation.
    MPDB *mpdb_;
    MPDB *mpdb_complement_;
    Distances distance_;

    void ComputeSquareCost(const Instance &instance);
    std::vector<std::vector<unsigned short>> square_cost_; //Closest goal.

    //Statistics.
    long times_called_;

    //PDB
    void InitiateNonNormalized(const Instance &instance);
    void InitiateNormalized(const Instance &instance);
    std::vector<std::vector<long>> combinations_;
    std::vector<std::vector<long>> stone_in_combination_;
    std::vector<std::pair<long, long>> cost_combinations_;

    std::vector<bool> used_combination_;
    std::vector<bool> used_stone_;
    std::vector<bool> stones_on_;
    std::vector<bool> reachable_;

    std::vector<unsigned char> closed_state_;
    std::vector<unsigned char> closed_complement_state_;
    std::vector<int> random_state_;


    int* edges_;
    int* weights_;
    std::vector<std::vector<std::vector<int> > > normalized_pdb_;

    bool build_;
    bool complement_;
    int size_;

    TypeComputationLowerbound computation_lowerbound;
    Utils utils_;
};
}
