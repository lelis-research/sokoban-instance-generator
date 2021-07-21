#pragma once

namespace ls
{
   struct State
   {
      std::vector<bool> reachable_;
      std::vector<int> state_;
   };

   enum Direction
   {
      Horizontal,
      Vertical
   };

   class Utils {
   public:
      Utils(const Instance &instance);
      const static int NormalizeManSquare(const std::vector<bool> &reachable_squares, const Instance &instance);
      void Reachable(const std::vector<bool> &stones_on, const int &man, std::vector<bool> &reachable_squares, const Instance &instance);
      void ReachableWithOneStone(const int &stone, const int &man, std::vector<bool> &reachable_squares, const Instance &instance);
      void ReachableZones(std::vector<int> &reachable_zones, std::vector<bool> &stones_on, const Instance &instance);
      const bool CanAnyReverseMove(const int stone, const std::vector<bool> &reachable_squares, const Instance &instance);
      const bool CanReverseMove(const int stone, const int man, const std::vector<bool> &reachable_squares, const Instance &instance);
      const bool CanMove(const int stone, const int man, const std::vector<bool> &stones_on, const std::vector<bool> &reachable_squares, const Instance &instance);
      void GetCombinations(const long size, std::vector<std::vector<long> > &combinations, std::vector<std::vector<long> > &stone_in_combination, const Instance &instance);
   private:
      void GenerateCombinations(const long int count, const long int m_combination, const long int n_combination, std::vector<long int> combination, std::vector<std::vector<long int> > &combinations, std::vector<std::vector<long int> > &stone_in_combination);
      std::vector<int> stack_;
   };

#ifndef __linux__
   class GetTime
   {
   public:
      GetTime() : duration_()
      {}
      inline void StartTimer() {duration_ = std::chrono::high_resolution_clock::now();}
      inline const double GetDuration() const {
         return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - duration_).count();
      }
   private:
      std::chrono::high_resolution_clock::time_point duration_;
   };
#else
   class GetTime
   {
   public:
      GetTime() : duration_()
      {}
      inline void StartTimer() { duration_ = clock(); }
      inline const double GetDuration() const {
         return double(clock() - duration_)/CLOCKS_PER_SEC;
      }
   private:
      clock_t duration_;
   };
#endif
}
