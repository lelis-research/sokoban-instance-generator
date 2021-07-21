#pragma once

namespace ls 
{
   class Instance;

   class LinearConflicts
   {
   public:
      LinearConflicts(const Instance &instance);
      const int GetLinearConflicts(const std::vector<int> &state, const Instance &instance);
      const int GetGoalZoneAndGlobalLinearConflicts( const std::vector<int> &state, const Instance &instance);      
      void PreComputeLinearConflicts(const Instance &instance);
   private:  
      const bool MoveWithoutDistanceIncrease(const int stone, const int man, Direction direction, const std::vector<bool> &reachable, const Instance &instance);

      std::vector<std::vector<std::vector<bool>>> linear_conflicts_;
      std::vector<bool> member_;
      Utils utils_;
      Distances distances_;
   };
}
