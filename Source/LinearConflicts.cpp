#include "stdafx.h"

/* LinearConflicts: Calculate the linear conflicts. */

namespace ls
{
   LinearConflicts::LinearConflicts(const Instance &instance) : utils_(instance),distances_(instance)
   {  
  
   }

   const int LinearConflicts::GetLinearConflicts(const std::vector<int> &state, const Instance &instance)
   {
      int penalty = 0;
      member_.assign(instance.NumberObjects(), false);
      
      for (int i = 0; i < instance.NumberObjects(); i++)
      {
         for (int j = i + 1; j < instance.NumberObjects(); j++)
         {
            int stone_a = state[i];
            int stone_b = state[j];
            int man = state[instance.NumberObjects()];

            if (member_[i] == false && !instance.GoalSquaresOn(stone_a))
            {
               if (member_[j] == false && !instance.GoalSquaresOn(stone_b))
               {
                  if (linear_conflicts_[stone_a][stone_b][man])
                  {
                     penalty += 2;
                     member_[i] = true;
                     member_[j] = true;
                  }
               }
            }
         }
      }
      return penalty;  
   }

   const int LinearConflicts::GetGoalZoneAndGlobalLinearConflicts( const std::vector<int> &state, const Instance &instance)
   {
      int penalty = 0;

      member_.assign(instance.NumberObjects(), false);

      for (int i = 0; i < instance.NumberObjects(); i++)
      {
         for (int j = i + 1; j < instance.NumberObjects(); j++)
         {
            int stone_a = state[i];
            int stone_b = state[j];
            int man = state[instance.NumberObjects()];

            if (instance.IsGoalZoneOrEntrance(stone_a) || instance.IsGoalZoneOrEntrance(stone_b))
            {
               if (member_[i] == false && !instance.GoalSquaresOn(stone_a))
               {
                  if (member_[j] == false && !instance.GoalSquaresOn(stone_b))
                  {
                     if (linear_conflicts_[stone_a][stone_b][man])
                     {
                        penalty += 2;
                        member_[i] = true;
                        member_[j] = true;
                     }
                  }
               }
            }
         }
      }
      return penalty;  
   }

   void LinearConflicts::PreComputeLinearConflicts(const Instance &instance)
   {
	  member_ = std::vector<bool>(instance.NumberObjects());
      
      linear_conflicts_ = std::vector<std::vector<std::vector<bool>>>(instance.Size(), std::vector<std::vector<bool>>(instance.Size(), std::vector<bool>(instance.Size(), false)));
      std::vector<bool> reachable = std::vector<bool>(instance.Size(), false);

      for (int stone_a = 0; stone_a < instance.Size(); stone_a++)
      {
         if (instance.FreeSquaresOn(stone_a))
         {
            int stone_b = stone_a + instance.Offset(2);

            if (stone_b < instance.Size())
            {
               if (instance.FreeSquaresOn(stone_b))
               {
                  std::vector<bool> man_squares(instance.Size(), false);
                  
                  for (int man = 0; man < instance.Size(); man++)
                  {
                     if (instance.FreeSquaresOn(man) && man_squares[man] == false)
                     {
                        std::vector<bool> stones_on(instance.Size(), false);
                        stones_on[stone_a] = true;
                        stones_on[stone_b] = true;

                        utils_.Reachable(stones_on, man, reachable, instance);

                        if (MoveWithoutDistanceIncrease(stone_a, man, Direction::Horizontal, reachable, instance) == false && 
                           MoveWithoutDistanceIncrease(stone_b, man, Direction::Horizontal, reachable, instance) == false)
                        {
                           linear_conflicts_[stone_a][stone_b][man] = true;
                           linear_conflicts_[stone_b][stone_a][man] = true;
                           
                           std::vector<bool> new_stones_on(instance.Size(), false);
                           
                           new_stones_on[stone_a] = true;
                           new_stones_on[stone_b] = true;
                           
                           man_squares[man] = true;
                           
                           for (int i = 0; i < instance.Size(); i++)
                           {
                              if (reachable[i])
                              {
                                 linear_conflicts_[stone_a][stone_b][i] = true;
                                 linear_conflicts_[stone_b][stone_a][i] = true;
                                 man_squares[i] = true;

                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }

      for (int stone_a = 0; stone_a < instance.Size(); stone_a++)
      {
         if (instance.FreeSquaresOn(stone_a))
         {
            int stone_b = stone_a + instance.Offset(0);
            if (stone_b < instance.Size())
            {
               if (instance.FreeSquaresOn(stone_b))
               {
                  std::vector<bool> man_squares(instance.Size(), false);
                  
                  for (int man = 0; man < instance.Size(); man++)
                  {
                     if (instance.FreeSquaresOn(man) && man_squares[man] == false)
                     {
                        std::vector<bool> stones_on(instance.Size(), false);

                        stones_on[stone_a] = true;
                        stones_on[stone_b] = true;

                        utils_.Reachable(stones_on, man, reachable, instance);

                        if (MoveWithoutDistanceIncrease(stone_a, man, Direction::Vertical, reachable, instance) == false && 
                           MoveWithoutDistanceIncrease(stone_b, man, Direction::Vertical, reachable, instance) == false)
                        {
                           linear_conflicts_[stone_a][stone_b][man] = true;
                           linear_conflicts_[stone_b][stone_a][man] = true;
                           man_squares[man] = true;

                           for (int i = 0; i < instance.Size(); i++)
                           {
                              if (reachable[i])
                              {
                                 linear_conflicts_[stone_a][stone_b][i] = true;
                                 linear_conflicts_[stone_b][stone_a][i] = true;
                                 man_squares[i] = true;
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   const bool LinearConflicts::MoveWithoutDistanceIncrease(const int stone, const int man, Direction direction, const std::vector<bool> &reachable, const Instance &instance)
   {
      if (direction == Direction::Vertical)
      {
         int required_man = stone - instance.Offset(2);
         int new_stone = stone + instance.Offset(2);

         if (new_stone < instance.Size() && required_man >= 0)
         {
            if (instance.FreeSquaresOn(new_stone) && reachable[required_man])
            {
               for (int i = 0; i < instance.NumberObjects(); i++)
               {
		 if (distances_.GetDistance(new_stone, instance.GoalSquares(i), stone) < distances_.GetDistance(stone, instance.GoalSquares(i), man))
                     return true;
               }
            }
         }

         required_man = stone + instance.Offset(2);
         new_stone = stone - instance.Offset(2);
         
         if (required_man < instance.Size() && new_stone >= 0)
         {
            if (instance.FreeSquaresOn(new_stone) && reachable[required_man])
            {
               for (int i = 0; i < instance.NumberObjects(); i++)
               {
                  if (distances_.GetDistance(new_stone, instance.GoalSquares(i), stone) < distances_.GetDistance(stone, instance.GoalSquares(i), man))
                     return true;
               }
            }
         }
         
         return false;

      } 
      else 
      {
         int required_man = stone - instance.Offset(0);
         int new_stone = stone + instance.Offset(0);
         
         if (new_stone < instance.Size() && required_man >= 0)
         {
            if (instance.FreeSquaresOn(new_stone) && reachable[required_man])
            {
               for (int i = 0; i < instance.NumberObjects(); i++)
               {
                  if (distances_.GetDistance(new_stone, instance.GoalSquares(i), stone) < distances_.GetDistance(stone, instance.GoalSquares(i), man))
                     return true;
               }
            }
         }

         required_man = stone + instance.Offset(0);
         new_stone = stone - instance.Offset(0);

         if (required_man < instance.Size() && new_stone >= 0)
         {
            if(instance.FreeSquaresOn(new_stone) && reachable[required_man])
            {
               for (int i = 0; i < instance.NumberObjects(); i++)
               {
                  if (distances_.GetDistance(new_stone, instance.GoalSquares(i), stone) < distances_.GetDistance(stone, instance.GoalSquares(i), man))
                     return true;
               }
            }
         }
         return false;
      }
   }
}
