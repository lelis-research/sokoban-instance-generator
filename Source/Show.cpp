#include "stdafx.h"

/* Show: Print the instances generated. */

namespace ls
{
	//Print on the file the state
   void Show::State(const std::vector<int> &state, const Instance &instance, std::ofstream &out_instance) {
      std::vector<bool> stone_on(instance.Size(), false);

      for (int i = 0; i < state.size() - 1; i++)
         stone_on[state[i]] = true;

      for (int i = 0; i < instance.Height(); i++)
      {
         for (int j = 0; j < instance.Width(); j++)
         {
            int k = i * instance.Width() + j;
            if (state[state.size() - 1] == k && instance.GoalSquaresOn(k))
            {
               //std::cout << '+';
               out_instance << '+';
            }
            else if (state[state.size() - 1] == k)
            {
               //std::cout << '@';
               out_instance << '@';
            }
            else if (stone_on[k] && instance.GoalSquaresOn(k))
            {
               //std::cout << '*';
               out_instance << '*';
            }
            else if (instance.WallSquaresOn(k))
            {
               //std::cout << '#';
               out_instance << '#';
            }
            else if (stone_on[k])
            {
               //std::cout << '$';
               out_instance << '$';
            }
            else if (instance.GoalSquaresOn(k))
            {
               //std::cout << '.';
               out_instance << '.';
            }
            else
            {
               //std::cout << ' ';
               out_instance << ' ';
            }
         }
         if (i + 1 != instance.Height())
            //std::cout << std::endl;
            out_instance << std::endl;
      }
      //std::cout << std::endl << std::endl;
      out_instance << std::endl;
   }

   //Print on the console the state
   void Show::State(const std::vector<int> &state, const Instance &instance)
   {
      std::vector<bool> stone_on(instance.Size(), false);

      for (int i = 0; i < state.size() - 1; i++)
         stone_on[state[i]] = true;

      for (int i = 0; i < instance.Height(); i++)
      {
         for (int j = 0; j < instance.Width(); j++)
         {
            int k = i * instance.Width() + j;
            if (state[state.size() - 1] == k && instance.GoalSquaresOn(k))
            {
               std::cout << '+';
            }
            else if (state[state.size() - 1] == k)
            {
               std::cout << '@';
            }
            else if (stone_on[k] && instance.GoalSquaresOn(k))
            {
               std::cout << '*';
            }
            else if (instance.WallSquaresOn(k))
            {
               std::cout << '#';
            }
            else if (stone_on[k])
            {
               std::cout << '$';
            }
            else if (instance.GoalSquaresOn(k))
            {
               std::cout << '.';
            }
            else
            {
               std::cout << ' ';
            }
         }
         if (i + 1 != instance.Height())
            std::cout << std::endl;
      }
      std::cout << std::endl << std::endl;
   }

   //Print the instance decomposed
   void Show::Decomposition(const Instance &instance, const int entrance, std::vector<bool> goal_zone, std::ofstream &out)
   {
      for (int i = 0; i < instance.Height(); i++)
      {
         for (int j = 0; j < instance.Width(); j++)
         {
            int v = i * instance.Width() + j;
            if (entrance == v)
            {
               std::cout << 'E';
               out << 'E';
            }    
            else if (instance.GoalSquaresOn(v)) 
            {
               std::cout << '.';
               out << '.';
            }
            else if (goal_zone[v] == true)
            {           
               std::cout << 'I';            
               out << 'I';
            }
            else if (instance.WallSquaresOn(v)) 
            {
               std::cout << '#';
               out << '#';
            }
            else if (instance.FreeSquaresOn(v) && instance.DeadSquaresOn(v) == false)
            {
               std::cout << 'O';
               out << 'O';
            }           
            else 
            {
               std::cout << ' ';
               out << ' ';
            }
         }
         if (i + 1 != instance.Height())
            std::cout << std::endl;
            out << std::endl;
      }
      std::cout << std::endl << std::endl;
      out << std::endl << std::endl;
      
   }

   void Show::FillOrder(const Instance &instance, const std::vector<int> order_on/*, std::ostream& out /*= std::cout*/ )
      {
         for (int i = 0; i < instance.Height(); i++)
         {
            for (int j = 0; j < instance.Width(); j++)
            {
               int v = i * instance.Width() + j;

               if (order_on[v] != INF)
               {
                  if (order_on[v] < 10)
                  {
                	  std::cout << order_on[v] ;
                  } else
                  {
                	  std::cout << (char)('A' - 10 + order_on[v]);
                  }
               }
               else if (instance.WallSquaresOn(v))
               {
            	   std::cout << '#';
               }
               else
               {
            	   std::cout << ' ';
               }
            }
            if (i + 1 != instance.Height())
               std::cout << std::endl;
         }
         std::cout << std::endl << std::endl;
      }

}
