#include "stdafx.h"

/* GenerateInstance: Generate the instance according to the algorithm and heuristics passed by parameter.*/

namespace ls
{
   GenerateInstance::GenerateInstance(const Instance &instance, const double LIMIT_TIME, TypeHeuristic type_heuristic)
      : type_heuristic_(type_heuristic), LIMIT_TIME_(LIMIT_TIME)
   {}


   std::vector<int> GenerateInstance::RunHS(const std::string file, const std::vector<int> &root, const Instance &instance, TypeHeuristic type_heuristic, TypeTest type_test,
		   std::ofstream &out_distance, std::ofstream &out_instance)
   {
	   // Creates a variable of type Heuristics
	   Heuristics heuristics_(instance, type_heuristic_, LIMIT_TIME_);
	   std::vector<int> best_state;

	   // Starting the timer
	   GetTime time;
	   time.StartTimer();

	   // Call the GBFS algorithm that will be the reverse search from the initial states through the space-states search
	   ls::GreedyBestFirstSearch hs(instance);
	   best_state = hs.Run(root, instance, time, out_distance, out_instance, LIMIT_TIME_, heuristics_);


	   // Print on the screen the value of MM, PDB1, IPDB2, IPDB3, IPDB4, MPDB2, MPDB3, MPDB4 of the last state found.
	   std::cout << " mm " << heuristics_.GetMM(best_state, instance)
					<< " pdb1 " << heuristics_.GetPDB1(best_state, instance)
					<< " mpdb2 " << heuristics_.GetPDB2(best_state, instance)
					<< " mpdb3 " << heuristics_.GetPDB3_Random(best_state, instance)
					<< " mpdb4 " << heuristics_.GetPDB4_Random(best_state, instance)
					<< " time " << time.GetDuration()
					<< std::endl;

	   // Print on the files the value of MM, PDB1, IPDB2, IPDB3, IPDB4, MPDB2, MPDB3, MPDB4 of the last state found.
	   out_distance << " mm " << heuristics_.GetMM(best_state, instance)
          			<< " pdb1 " << heuristics_.GetPDB1(best_state, instance)
					<< " mpdb2 " << heuristics_.GetPDB2(best_state, instance)
					<< " mpdb3 " << heuristics_.GetPDB3_Random(best_state, instance)
					<< " mpdb4 " << heuristics_.GetPDB4_Random(best_state, instance)
					<< " time " << time.GetDuration()
					<< std::endl;


	   //Print the instance generated.
	   Show::State(best_state, instance, out_instance);

	   // Returns the best initial state founded by the GBFS algorithm
	   return best_state;

   }

}
