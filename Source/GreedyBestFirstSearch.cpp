#include "stdafx.h"

/* GreedySearch: do  a reverse greedy best first search on the state space, considering the states on the priority queue and closed set.
 * The algorithm stops when the limit of memory or the limit or time is reached.*/

namespace ls
{
   GreedyBestFirstSearch::GreedyBestFirstSearch(const Instance &instance) :
      utils_(instance),
      stones_on_(std::vector<bool>(instance.Size(), false)),
      reachable_(std::vector<bool>(instance.Size(), false)),
      child_(std::vector<int>(instance.SizeState() + 1, 0)),
      child_reachable_(std::vector<bool>(instance.Size(), false))
   {}

   std::vector<int> GreedyBestFirstSearch::Run(const std::vector<int> & root, const Instance & instance, GetTime time, std::ofstream & out_distances, std::ofstream &out_instance,
		   const double LIMIT_TIME, Heuristics &heuristics)
   {

	  boost::unordered_set<std::vector<int> > closed_set;
      std::priority_queue< std::pair<int, std::vector<int>> > pq_open_set;

      std::vector<int> father = root;
      std::vector<int> best_state = root;

      int father_value = 0;
      int best_value = 0;
      int child_value = 0;
      int objective_value = 0;

      int search_distance_ = 0;
      int expanded_nodes = 0;
      int step_debug_ = 0;


      // Get the heuristic value for the father state
      heuristics.GetValueObjectiveFunction(father, instance, child_value, best_value);

      // Put on the priority queue and the closed set the father state and its heuristic value
      pq_open_set.push(std::pair<int, std::vector<int>>(father_value, father));
      closed_set.insert(father);


      // Run the algorithm while the open priority queue is not empty and the limit time is not reached
      while (pq_open_set.empty() == false && time.GetDuration() < LIMIT_TIME)
      {
         father = pq_open_set.top().second;
         search_distance_ = pq_open_set.top().first;

         // Remove the state on open queue.
         pq_open_set.pop();

         expanded_nodes++;
         step_debug_++;

         // Verify if the memory reached the limit of the memory (8GB)
         if (getCurrentRSS() > 8589934592) //if (getCurrentRSS() > 4294967296)
         {
            break;
            step_debug_ = 0;
         }

         // Generate the successors
         for (int i = 0; i < father.size() - 1; i++)
         {
            stones_on_[father[i]] = true;
         }

         // Verify if the state generated from the father state is the reachable
         utils_.Reachable(stones_on_, father[father.size() - 1], reachable_, instance);

         int stone, man;
         for (int i = 0; i < father.size() - 1; i++)
         {
            for (int j = 0; j < instance.SizeMovesMan(); j++)
            {
               stone = father[i] + instance.ReverseMove(j);
               man = father[i] + instance.ReverseMan(j);

               if (stone >= instance.Size() || stone < 0 || man >= instance.Size() || man < 0) continue;

               bool can_move = utils_.CanReverseMove(stone, man, reachable_, instance);

               if (can_move)
               {
                  std::copy(father.begin(), father.end(), child_.begin());

                  child_[i] = stone;
                  int stone_before = father[i];
                  int stone_after = child_[i];

                  stones_on_[stone_before] = false;
                  stones_on_[stone_after] = true;

                  utils_.Reachable(stones_on_, man, child_reachable_, instance);

                  child_[child_.size() - 1] = utils_.NormalizeManSquare(child_reachable_, instance);

                  stones_on_[stone_before] = true;
                  stones_on_[stone_after] = false;

                  std::sort(child_.begin(), child_.end() - 1);

                  if (closed_set.find(child_) == closed_set.end())
                  {
                     // Get the heuristic value for the child state
                	 heuristics.GetValueObjectiveFunction(child_, instance, child_value, objective_value);

                	 // Put on the priority queue and the closed set the child state and its heuristic value
                     pq_open_set.push(std::pair<int, std::vector<int>>(child_value, child_));
                     closed_set.insert(child_);

                     // Choose the state with the best value between the last best_value and the last child evaluated
                     best_value = std::max(objective_value, best_value);

                     // Refresh the best_state with the child, if its heuristic value is the best value than last best_value
                     if (best_value == objective_value)
                     {
                        best_state = child_;
                     }
                  }
               }
            }
         }

         // Clean the vector stones_on_ for the next iteration
         for (int i = 0; i < father.size() - 1; i++)
         {
            stones_on_[father[i]] = false;
         }
      }

      // Print the values of the expanded nodes and the current memory
      std::cout << " expanded " << expanded_nodes << " memory " << getCurrentRSS();
      out_distances << " expanded " << expanded_nodes << " memory " << getCurrentRSS();

      // Returns the best initial state founded by the GBFS algorithm
      return best_state;
   }
}
