#include "stdafx.h"

/* Utils: It has some features as verify if a state is reachable or not; if can do the movement only with stone, or with stone and man;
 * if a state is reachable with one stone; if a zone is reachable;
 * besides making combinations between the stones to be used in the calculation of conflicts.
 * */

namespace ls
{

   Utils::Utils(const Instance &instance) : stack_(std::vector<int>(instance.Size()))
   {}

   //Normalize the position of man
   const int Utils::NormalizeManSquare(const std::vector<bool> &reachable_squares, const Instance &instance)
   {
      for (int i = 0; i < instance.Size(); i++)
      {
         if (reachable_squares[i])
            return (i);
      }
      return -1;
   }

   //Verify if the state is reachable
   void Utils::Reachable(const std::vector<bool> &stones_on, const int &man, std::vector<bool> &reachable_squares, const Instance &instance)
   {
      reachable_squares.assign(instance.Size(), false);
      int top = 0;

      stack_[top] = man;
      top++;

      while (top != 0)
      {
         top--;
         int square = stack_[top];

         reachable_squares[square] = true;
         for (size_t i = 0; i < 4; i++)
         {
            if (instance.WallSquaresOn(square + instance.ReverseMove(i)) == false &&
               stones_on[square + instance.ReverseMove(i)] == false &&
               reachable_squares[square + instance.ReverseMove(i)] == false)
            {
               stack_[top] = square + instance.ReverseMove(i);
               top++;
            }
         }
      }
   }

   //Verify if is reachable with only one stone
   void Utils::ReachableWithOneStone(const int &stone, const int &man, std::vector<bool> &reachable_squares, const Instance &instance)
   {
      reachable_squares.assign(instance.Size(), false);
      int top = 0;

      stack_[top] = man;
      top++;

      while (top != 0)
      {
         top--;
         int square = stack_[top];

         reachable_squares[square] = true;
         for (size_t i = 0; i < 4; i++)
         {
            if (instance.WallSquaresOn(square + instance.Move(i)) == false &&
               square + instance.Move(i) != stone &&
               reachable_squares[square + instance.Move(i)] == false)
            {
               stack_[top] = square + instance.Move(i);
               top++;
            }
         }
      }
   }

   //Verify if the zone is reachable
   void Utils::ReachableZones( std::vector<int> &reachable_zones, std::vector<bool> &stones_on, const Instance &instance)
   {
      reachable_zones.assign(instance.Size(), -1);

      int top = 0;

      int man = 0;

      for (int i = 0; i < instance.Size(); i++)
      {
         if (reachable_zones[i] == -1 && instance.FreeSquaresOn(i) == true && stones_on[i] == false)
         { 
            stack_[top] = i;
            top++;

            while (top != 0)
            {
               top--;
               int square = stack_[top];

               reachable_zones[square] = man;
               for (size_t i = 0; i < 4; i++)
               {
                  if (instance.WallSquaresOn(square + instance.Offset(i)) == false && 
                     stones_on[square + instance.Offset(i)] == false && 
                     reachable_zones[square + instance.Offset(i)] == -1) 
                  {
                     stack_[top] = square + instance.Offset(i);
                     top++;
                  }
               }
            }
           
            man++;
         }
      }
   }

   //Verify if can do any movement reverse
   const bool Utils::CanAnyReverseMove(const int stone, const std::vector<bool> &reachable_squares, const Instance &instance)
   {
   	   for (int i = 0; i < 4; i++)
   	   {
   		   if (CanReverseMove(stone + instance.Offset(i), stone + 2 * instance.Offset(i), reachable_squares, instance))
   		   {
   			   return true;
   		   }
   	   }

   	   return false;
   }

   const bool Utils::CanReverseMove(const int stone, const int man, const std::vector<bool> &reachable_squares, const Instance &instance)
   {
      if (man < instance.Size() && stone < instance.Size() && man >= 0 && stone >= 0)
      {
         if (reachable_squares[man] && reachable_squares[stone])
         {
            return true;
         }
      }
      return false;
   }


   const bool Utils::CanMove(const int stone, const int man, const std::vector<bool> &stones_on, const std::vector<bool> &reachable_squares, const Instance &instance)
   {
      if (instance.WallSquaresOn(stone) == false && stones_on[stone] == false && reachable_squares[man] == true && instance.DeadSquaresOn(stone) == false) return true;
      return false;
   }



   //Do the combinations to be used to calculate the conflicts
   void Utils::GetCombinations(const long size, std::vector<std::vector<long> > &combinations, std::vector<std::vector<long> > &stone_in_combination, const Instance &instance)
     {
        stone_in_combination = std::vector<std::vector<long>>(instance.NumberObjects(),std::vector<long>());
        GenerateCombinations(0, size, instance.NumberObjects(), std::vector<long>(), combinations, stone_in_combination);
     }

   void Utils::GenerateCombinations(const long count, const long m_combination, const long n_combination, std::vector<long> combination, std::vector<std::vector<long> > &combinations, std::vector<std::vector<long>  > &stone_in_combination)
   {
		if (m_combination == 0) {
			combinations.push_back(combination);
			for (long i = 0; i < combination.size(); i++) {
				stone_in_combination[combination[i]].push_back(combinations.size() - 1);
			}
		} else {
			for (long i = count; i <= n_combination - m_combination; i++) {
				combination.push_back(i);
				GenerateCombinations(i + 1, m_combination - 1, n_combination, combination, combinations, stone_in_combination);
				combination.pop_back();
			}
		}
	}
}

