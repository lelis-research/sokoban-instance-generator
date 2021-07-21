#pragma once

namespace ls
{
   class Show {
   public:
      static void State(const std::vector<int> &state, const Instance &instance);
      static void State(const std::vector<int> &state, const Instance &instance, std::ofstream &out_instance);
      static void Decomposition(const Instance &instance, const int entrance, std::vector<bool> goal_zone, std::ofstream &out);
      static void FillOrder(const Instance &instance, const std::vector<int> order_on); //, std::ostream& out);
   };
}

