#include "stdafx.h"

/* Heuristics: return the value of heuristic.
 * Novelty: create the tables and compute the states with novelty.
 */
namespace ls
{
   Heuristics::Heuristics(const Instance &instance, const TypeHeuristic type_heuristic, const double LIMIT_TIME) :
       heuristic(type_heuristic),
       mm(instance),
       mpdb2(instance, 2, LIMIT_TIME),
	   mpdb3(instance, 3, LIMIT_TIME),
       mpdb4(instance, 4, LIMIT_TIME),
       novelty(instance, 1, type_heuristic),
	   distance_(instance)
   {
      mpdb2.Initiate(instance, LIMIT_TIME);
      mpdb3.Initiate(instance, LIMIT_TIME);
      mpdb4.Initiate(instance, LIMIT_TIME);

      // PDB1
      square_cost_ = std::vector<std::vector<int>>(instance.Size(), std::vector<int>(instance.Size(), INF));
      for (int stone = 0; stone <  instance.Size(); stone++)
      {
    	  for (int man = 0; man < instance.Size() ; man++)
    	  {
    		  if (instance.FreeSquaresOn(stone) == true && instance.FreeSquaresOn(man) == true && man != stone)
    		  {
    			  for (int goal = 0; goal < instance.NumberObjects(); goal++)
    			  {
    				  square_cost_[stone][man] = std::min((int)square_cost_[stone][man], (int)distance_.GetDistance(stone, instance.GoalSquares(goal), man));
    				  //std::cout << " square_cost " << square_cost_[stone][man] << std::endl;
    			  }
    		  }
    	  }
      }
  }

   // Return the value objection function for the heuristic passed

   void Heuristics::GetValueObjectiveFunction(const std::vector<int> &state, const Instance &instance, int &h_value, int &objective_function)
   {

		// A -- Simple heuristic

	   if (heuristic == TypeHeuristic::pdb1)
	   {
		   h_value = GetPDB1(state,instance);
		   objective_function = h_value;
	   }
	   else if (heuristic == TypeHeuristic::pdb2)
	   {
		   h_value = GetPDB2(state, instance);
		   objective_function = h_value;
	   }
	   else if (heuristic == TypeHeuristic::pdb3r)
	   {
		   h_value = GetPDB3_Random(state, instance);
		   objective_function = h_value;
	   }
	   else if (heuristic == TypeHeuristic::pdb4r)
	   {
		   h_value = GetPDB4_Random(state, instance);
		   objective_function = h_value;
	   }

		// B -- Heuristic + Conflicts

	   else if (heuristic == TypeHeuristic::pdb2_oc2) //[PDB2, PDB2-PDB1]
	   {
		   int pdb1_value = GetPDB1(state, instance);
		   int pdb2_value = GetPDB2(state, instance);
		   objective_function = 1000 * pdb2_value + (pdb2_value - pdb1_value);
		   h_value = objective_function;
	   }
	   else if (heuristic == TypeHeuristic::pdb3r_oc3r) //[PDB3r, PDB3r-PDB2]
	   {
		   int pdb2_value = GetPDB2(state, instance);
		   int pdb3_value = GetPDB3_Random(state, instance);
		   objective_function = 1000 * pdb3_value + (pdb3_value - pdb2_value);
		   h_value = objective_function;
	   }
	   else if (heuristic == TypeHeuristic::pdb4r_oc4r) //[PDB4r, PDB4r-PDB3r]
	   {
		   int pdb3_value = GetPDB3_Random(state, instance);
		   int pdb4_value = GetPDB4_Random(state, instance);
		   objective_function = 1000 * pdb4_value + (pdb4_value - pdb3_value);
		   h_value = objective_function;
	   }

	   // C -- Conflicts + Heuristic

	   else if (heuristic == TypeHeuristic::oc2_pdb2) //[PDB2-PDB1,PDB2]
	   {
		   int pdb1_value = GetPDB1(state, instance);
		   int pdb2_value = GetPDB2(state, instance);
		   objective_function = 1000 * (pdb2_value - pdb1_value) + pdb2_value;
		   h_value = objective_function;
	   }
	   else if (heuristic == TypeHeuristic::oc3r_pdb3r) //[PDB3r-PDB2,PDB3r]
	   {
		   int pdb2_value = GetPDB2(state, instance);
		   int pdb3_value = GetPDB3_Random(state, instance);
		   objective_function = 1000 * (pdb3_value - pdb2_value) + pdb3_value;
		   h_value = objective_function;
	   }
	   else if (heuristic == TypeHeuristic::oc4r_pdb4r) //[PDB4r-PDB3r,PDB4r]
	   {
		   int pdb3_value = GetPDB3_Random(state, instance);
		   int pdb4_value = GetPDB4_Random(state, instance);
		   objective_function = 1000 * (pdb4_value - pdb3_value) + pdb4_value;
		   h_value = objective_function;
	   }

		// D -- Novelty + Heuristic

		else if (heuristic == TypeHeuristic::wpdb1_pdb1) // [w(PDB1),PDB1]
		{
			objective_function = GetPDB1(state, instance);
			h_value = (3 - novelty.GetNovelty_2H(state, instance, objective_function)) * 1000 + objective_function;
		}

		else if (heuristic == TypeHeuristic::wpdb2_pdb2) // [w(PDB2),PDB2]
		{
			objective_function = GetPDB2(state, instance);
			h_value = (3 - novelty.GetNovelty_2H(state, instance, objective_function)) * 1000 + objective_function;
		}
		else if (heuristic == TypeHeuristic::wpdb3r_pdb3r) // [w(PDB3r),PDB3r]
		{
			objective_function = GetPDB3_Random(state, instance);
			h_value = (3 - novelty.GetNovelty_2H(state, instance, objective_function)) * 1000 + objective_function;
		}

		else if (heuristic == TypeHeuristic::wpdb4r_pdb4r) // [w(PDB4r),PDB4r]
		{
			objective_function = GetPDB4_Random(state, instance);
			h_value = (3 - novelty.GetNovelty_2H(state, instance, objective_function)) * 1000 + objective_function;
		}

		// E -- Novelty + Heuristic + Conflicts

	      else if (heuristic == TypeHeuristic::wpdb2_pdb2_oc2) // [w(PDB2),PDB2,PDB2-PDB1]
	      {
	    	  int pdb1_value = GetPDB1(state, instance);
	    	  int pdb2_value = GetPDB2(state, instance);
	    	  objective_function = 10000 * pdb2_value + (pdb2_value - pdb1_value);
	    	  h_value = (3 - novelty.GetNovelty_2H(state, instance, pdb2_value)) * 100000 + pdb2_value * 1000 + (pdb2_value - pdb1_value);
	      }
	      else if (heuristic == TypeHeuristic::wpdb3r_pdb3r_oc3r) // [w(PDB3r),PDB3r,PDB3r-PDB2]
	      {
	    	  int pdb2_value = GetPDB2(state, instance);
	    	  int pdb3_value = GetPDB3_Random(state, instance);
	    	  objective_function = 10000 * pdb3_value + (pdb3_value - pdb2_value);
	    	  h_value = (3 - novelty.GetNovelty_2H(state, instance, pdb3_value)) * 100000 + pdb3_value * 1000 + (pdb3_value - pdb2_value);
	      }
	      else if (heuristic == TypeHeuristic::wpdb4r_pdb4r_oc4r) // [w(PDB4r),PDB4r,PDB4r-PDB3r]
	      {
	    	  int pdb3_value = GetPDB3_Random(state, instance);
	    	  int pdb4_value = GetPDB4_Random(state, instance);
	    	  objective_function = 10000 * pdb4_value + (pdb4_value - pdb3_value);
	    	  h_value = (3 - novelty.GetNovelty_2H(state, instance, pdb4_value)) * 100000 + pdb4_value * 1000 + (pdb4_value - pdb3_value);
	      }

		// F -- Novelty + Conflicts + Heuristic

		      else if (heuristic == TypeHeuristic::wpdb2_oc2_pdb2) // [w(PDB2),PDB2-PDB1,PDB2] */
	      {
	    	  int pdb1_value = GetPDB1(state, instance);
	    	  int pdb2_value = GetPDB2(state, instance);
	    	  objective_function = 10000 * (pdb2_value - pdb1_value) + pdb2_value;
	    	  h_value = (3 - novelty.GetNovelty_2H(state, instance, pdb2_value)) * 100000 + (pdb2_value - pdb1_value) * 1000 + pdb2_value;
	      }

	      else if (heuristic == TypeHeuristic::wpdb3r_oc3r_pdb3r) // [w(PDB3r),PDB3r-PDB2,PDB3r]
	      {
	    	  int pdb2_value = GetPDB2(state, instance);
	    	  int pdb3_value = GetPDB3_Random(state, instance);
	    	  objective_function = 10000 * (pdb3_value - pdb2_value) + pdb3_value;
	    	  h_value = (3 - novelty.GetNovelty_2H(state, instance, pdb3_value)) * 100000 + (pdb3_value - pdb2_value) * 1000 + pdb3_value;
	      }
	      else if (heuristic == TypeHeuristic::wpdb4r_oc4r_pdb4r) // [w(PDB4r),PDB4r-PDB3r,PDB4r]
	      {
	    	  int pdb3_value = GetPDB3_Random(state, instance);
	    	  int pdb4_value = GetPDB4_Random(state, instance);
	    	  objective_function = 10000 * (pdb4_value - pdb3_value) + pdb4_value;
	    	  h_value = (3 - novelty.GetNovelty_2H(state, instance, pdb4_value)) * 100000 + (pdb4_value - pdb3_value) * 1000 + pdb4_value;
	      }

   }

   // Calculate the canonical PDB heuristic, where there is only a stone plus the man.
   const int Heuristics::GetPDB1(const std::vector<int> &state, const Instance &instance)
   {
	   int lower_bound = 0;
	   for (int i = 0; i < state.size()-1; i++)
	   {
		   lower_bound += square_cost_[state[i]][state[state.size() - 1]];
	   }

	   return lower_bound;
   }


   // Calculate of the Novelty

   Novelty::Novelty(const Instance &instance, const int novelty_size, ls::TypeHeuristic type_heuristic)
   {
      novelty_one_h = std::vector<std::vector<std::vector<bool>>>(700, std::vector<std::vector<bool>>(instance.NumberFreeSquares(), std::vector<bool>(instance.NumberFreeSquares(), false)));
      novelty_two_h = std::vector<std::vector<std::vector<std::vector<bool>>>>(700, std::vector<std::vector<std::vector<bool>>>(instance.NumberFreeSquares(), std::vector<std::vector<bool>>(instance.NumberFreeSquares(), std::vector<bool>(instance.NumberFreeSquares(), false))));
   }

   int Novelty::GetNovelty_2H(const std::vector<int> &state, const Instance &instance, int h_value)
   {

	   bool novelty1 = false;
	   bool novelty2 = false;

	   for (int i = 0; i < state.size() - 1; i++)
	   {
		   if (novelty_one_h[h_value][instance.SquaresToFree(state[i])][instance.SquaresToFree(state[state.size() - 1])] == false)
		   {
			   novelty_one_h[h_value][instance.SquaresToFree(state[i])][instance.SquaresToFree(state[state.size() - 1])] = true;
			   novelty1 = true;
		   }
	   }

	   for (int i = 0; i < state.size() - 1; i++)
	   {
		   for (int j = i + 1; j < state.size() - 1; j++)
		   {
			   if (novelty_two_h[h_value][instance.SquaresToFree(state[i])][instance.SquaresToFree(state[j])][instance.SquaresToFree(state[state.size() - 1])] == false)
			   {
				   novelty_two_h[h_value][instance.SquaresToFree(state[i])][instance.SquaresToFree(state[j])][instance.SquaresToFree(state[state.size() - 1])] = true;
				   novelty2 = true;
			   }
		   }
	   }

	   if (novelty1)
		   return 1;
	   else if (novelty2)
		   return 2;
	   else
		   return 3;
   }

   int Novelty::GetNovelty_2H_new(const std::vector<int> &state, const Instance &instance, int h_value)
   {
	   bool novelty1 = false;
	   bool novelty2 = false;

	   for (int i = 0; i < state.size() - 1; i++)
	   {
		   for (int j = i + 1; j < state.size() - 1; j++)
		   {
			   if (novelty_one_h[h_value][instance.SquaresToFree(state[i])][instance.SquaresToFree(state[j])] == false)
			   {
				   novelty_one_h[h_value][instance.SquaresToFree(state[i])][instance.SquaresToFree(state[j])] = true;
				   novelty1 = true;
			   }
		   }
	   }

	   for (int i = 0; i < state.size() - 1; i++)
	   {
		   for (int j = i + 1; j < state.size() - 1; j++)
		   {
			   for (int k = j + 1; k < state.size() - 1; k++)
			   {
				   if (novelty_two_h[h_value][instance.SquaresToFree(state[i])][instance.SquaresToFree(state[j])][instance.SquaresToFree(state[k])] == false)
				   {
					   novelty_two_h[h_value][instance.SquaresToFree(state[i])][instance.SquaresToFree(state[j])][instance.SquaresToFree(state[k])] = true;
					   novelty2 = true;
				   }
			   }
		   }
	   }

	   if (novelty1)
		   return 1;
	   else if (novelty2)
		   return 2;
	   else
		   return 3;
   }
}
