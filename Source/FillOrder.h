#pragma once

namespace ls
{
   class FillOrder
   {
   public:
      FillOrder(const Instance &instance);

      const int GetWeightFillOrder(int current_stone) const;
      const int GetWeightFillOrder(std::vector<int> &state);
   private:
      void GenerateWeightedMapFillOrder(const Instance &instance);
      std::vector<int> fill_order_weighted_;
      std::vector<int> fill_order_goals;
      std::vector<int> goal_order;
      std::vector<int> stones_on;
      Utils utils_;
   };
}
