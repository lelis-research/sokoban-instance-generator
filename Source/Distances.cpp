#include "stdafx.h"

namespace ls
{
   Distances::Distances(const Instance &instance) : utils_(instance)
   {
      distances_ = std::vector<std::vector<std::vector<int> >>(instance.Size(), std::vector<std::vector<int> >(instance.Size(), std::vector<int>(instance.Size(), DEAD_SQUARE)));
      BackoutDistances(instance);

      for (int i = 0; i < instance.Size(); i++)
      {
         for (int j = 0; j < instance.Size(); j++)
         {
            if (instance.FreeSquaresOn(i) && instance.FreeSquaresOn(j))
            {
               distances_[i][i][j] = 0;
            }
         }
      }
   }

   void Distances::BackoutDistances(const Instance &instance)
   {
      std::tuple<int, int, int> node;
      std::vector<bool> reachable_squares = std::vector<bool>(instance.Size(), false);

      for (int i = 0; i < instance.Size(); i++)
      {
         if (instance.FreeSquaresOn(i))
         {
            for (int j = 0; j < 4; j++)
            {
               int man_initial = i + instance.Move(j);
               if (man_initial < instance.Size())
               {
                  if (instance.FreeSquaresOn(man_initial))
                  {
                     std::vector<std::vector<int> > closed_set(instance.Size(), std::vector<int>(instance.Size(), DEAD_SQUARE));
                     std::get<0>(node) = 0;
                     std::get<1>(node) = i;
                     std::get<2>(node) = man_initial;

                     closed_set[std::get<1>(node)][std::get<2>(node)] = 0;

                     std::queue<std::tuple<int, int, int> > openSet;
                     openSet.push(node);
                     while (!openSet.empty())
                     {
                        node = openSet.front();
                        openSet.pop();

                        Successors(node, openSet, closed_set, reachable_squares, instance);
                     }

                     for (int stones = 0; stones < instance.Size(); stones++)
                     {
                        for (int man = 0; man < instance.Size(); man++)
                        {
                           distances_[i][stones][man_initial] = std::min(closed_set[stones][man], distances_[i][stones][man_initial]);
                        }
                     }

                     closed_set.clear();
                     utils_.ReachableWithOneStone(i, man_initial, reachable_squares, instance);

                     for (int man = 0; man < instance.Size(); man++)
                     {
                        if (reachable_squares[man])
                        {
                           for (int square = 0; square < instance.Size(); square++)
                              distances_[i][square][man] = std::min(distances_[i][square][man_initial], distances_[i][square][man]);
                        }
                     }
                  }
               }
            }
         }
      }
   }

   void Distances::Successors(std::tuple<int, int, int> &node, std::queue<std::tuple<int, int, int> > &open_set, std::vector<std::vector<int> > &closed_set, std::vector<bool> &reachable_squares, const Instance &instance)
   {
      std::tuple<int, int, int> node_child;

      utils_.ReachableWithOneStone(std::get<1>(node), std::get<2>(node), reachable_squares, instance);

      for (int j = 0; j < 4; j++)
      {
         int stone = std::get<1>(node) + instance.Move(j);
         int man = std::get<1>(node) + instance.Man(j);

         if (stone < instance.Size() && man < instance.Size())
         {
            if (instance.FreeSquaresOn(stone) == true && reachable_squares[man] == true)
            {
               std::get<0>(node_child) = std::get<0>(node) + 1;
               std::get<1>(node_child) = stone;
               std::get<2>(node_child) = std::get<1>(node);

               if (std::get<0>(node_child) < closed_set[std::get<1>(node_child)][std::get<2>(node_child)])
               {
                  closed_set[std::get<1>(node_child)][std::get<2>(node_child)] = std::get<0>(node_child);
                  open_set.push(node_child);
               }
            }
         }
      }
   }

   std::vector<bool> Distances::DeadSquaresOn(const Instance &instance)
   {
      std::vector<bool> dead_squares_on = std::vector<bool>(instance.Size(), true);
      for (int i = 0; i < instance.Size(); i++)
      {
         dead_squares_on[i] = true;
         for (int j = 0; j < 4; j++)
         {
            int man = i + instance.Offset(j);
            if (man < instance.Size())
            {
               if (instance.FreeSquaresOn(i) == true && instance.FreeSquaresOn(man) == true)
               {
                  for (int k = 0; k < instance.NumberObjects(); k++)
                  {
                     if (distances_[i][instance.GoalSquares(k)][man] != DEAD_SQUARE)
                     {
                        dead_squares_on[i] = false;
                        break;
                     }
                  }
                  if (dead_squares_on[i] == false) break;
               }
            }
         }
      }
      return dead_squares_on;
   }
}
