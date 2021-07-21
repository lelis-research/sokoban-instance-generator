#pragma once

namespace ls
{
   class GenerateInstance
   {
   private:
      const double LIMIT_TIME_;
      const TypeHeuristic type_heuristic_;

   public:
      GenerateInstance(const Instance &instance, const double LIMIT_TIME, TypeHeuristic type_heuristic);

      std::vector<int> RunHS(const std::string file, const std::vector<int> &root, const Instance &instance, TypeHeuristic type_heuristic,
    		  TypeTest type_test, std::ofstream &out_distance, std::ofstream &out_instance);

   };

}
