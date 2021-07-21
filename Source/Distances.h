#pragma once

namespace ls
{
   class Distances {
   public:
      Distances(const Instance &instance);
      const int GetDistance(const int stone, const int goal, const int man) const { return distances_[stone][goal][man]; }
      std::vector<std::vector<std::vector<int>>> GetDistance() {return distances_;}
     std::vector<bool> DeadSquaresOn(const Instance &instancez);
   
   private:
      void BackoutDistances(const Instance &instance);
      void Successors(std::tuple<int, int, int> &node, 
         std::queue<std::tuple<int, int, int> > &open_set,
         std::vector<std::vector<int> > &closed_set, 
         std::vector<bool> &reachable_squares, const Instance &instance);

      std::vector<std::vector<std::vector<int> > > distances_;
      Utils utils_;
   };
}
