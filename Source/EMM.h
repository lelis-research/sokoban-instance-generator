#pragma once

namespace ls
{
   class EMM
   {
   public:
      EMM(const Instance &instance);
      ~EMM();

      const int GetLowerbound(const std::vector<int> &state, const Instance &instance);
      //std::vector<bool> DeadSquaresOn(const Instance &instance) {return distances_.DeadSquaresOn(instance);}

      //void GetLowerbound(const std::vector<int> &state, const int stone_before, const int stone_after, int &maze_lowerbound, int &goal_lowerbound)
   private:
      Distances distances_;
      int *edges_;
      int *weights_;

      //bool Initiate(const Instance &instance);

      //Used for the matching computation.
      //LinearConflicts linear_conflicts_;

   };
}
