#pragma once

namespace ls
{
   class MPDB
   {
   public:
      MPDB(const Instance &instance, const int size, const int LIMIT_TIME);
      ~MPDB();
      virtual const bool Solve(const Instance &instance, const int LIMIT_TIME);
      virtual const bool Initiate(const Instance &instance);

      const int GetValue(const std::vector<unsigned char> &state);
      std::vector<std::vector<std::vector<int>>> Normalize(const Instance &instance);
   private:
      //Main.
      virtual void Improve(const Instance &instance);

      //Initial states.
      void InitialStates();

      //Utils
      std::vector<unsigned char> closed_state_;
      ushort cost_;
      Utils utils_;

      // PDB
      int stone_before_;
      int stone_after_;
      std::vector<bool> stones_on_;
      void GenerateSuccessors(const Instance &instance);

      std::vector<bool> father_reachable_;
      std::vector<int> father_;
      std::vector<bool> child_reachable_;
      std::vector<int> child_;

      //Lowerbound
      std::queue<std::pair<int,std::vector<int>>> open_set_;
      boost::unordered_map<std::vector<unsigned char>,int> closed_set_;

      //Info
      unsigned long long expanded_nodes_;
      unsigned long long size_;
   };
}
