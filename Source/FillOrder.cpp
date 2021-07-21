#include "stdafx.h"

namespace ls
{
   FillOrder::FillOrder(const Instance &instance) : utils_(instance)
   {
      GenerateWeightedMapFillOrder(instance);
   }

   void FillOrder::GenerateWeightedMapFillOrder(const Instance &instance)
   {
      fill_order_weighted_ = std::vector<int>(instance.NumberObjects(), 0);
      fill_order_goals = std::vector<int>(instance.Size(), INF);
      
      std::vector<bool> reachablePositions(instance.Size(), false);;
      std::vector<bool> stones_on(instance.Size(), false);
      std::vector<bool> stones_on_weighted(instance.Size(), false);
      for (int i = 0; i <  instance.Size(); i++)
      {
         if (instance.GoalSquaresOn(i))
         {
            stones_on[i] = true;
            stones_on_weighted[i] = true;
         }
         	
      }
      
      int goals_counter = 0;

      while (goals_counter < instance.NumberObjects())
      {
         for (int man = 0; man < instance.Size(); man++)
         {
            for (int i = 0; i < instance.NumberObjects(); i++)
            {
               if (instance.FreeSquaresOn(man) == true && stones_on[man] == false)
               {
            	  utils_.Reachable(stones_on, man, reachablePositions, instance);
            	  if (utils_.CanAnyReverseMove(instance.GoalSquares(i), reachablePositions, instance)
            	                       && stones_on_weighted[instance.GoalSquares(i)])
                  {
                     stones_on_weighted[instance.GoalSquares(i)] = false;
                     fill_order_goals[instance.GoalSquares(i)] = goals_counter;
                     goals_counter++;
                  }
               }
            }
         }
         stones_on = stones_on_weighted;
      }

      for (int i = 0; i < instance.NumberObjects(); i++)
      {
         fill_order_weighted_[i] = (int)pow(2, i);
      }

      //Show::FillOrder(instance, fill_order_goals);
      //system("PAUSE");
   }

   const int FillOrder::GetWeightFillOrder(int current_stone) const
   {
      if (fill_order_goals[current_stone] == INF)
      {
         return 0;
      }
      return fill_order_weighted_[fill_order_goals[current_stone]];
   }

}
