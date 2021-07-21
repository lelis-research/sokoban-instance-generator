#include "stdafx.h"

/* Read the instance and stores the position of all elements contained in it.  Do the decomposition of instance too.
     	# - wall
		@ - man
		$ - stone
		. - goal square
		* - stone on goal
		+ - man on goal
*/

namespace ls
{
   Instance::Instance()
      : free_squares_on_(std::vector<bool>()), goal_squares_on_(std::vector<bool>()) , 
      wall_squares_on_(std::vector<bool>()), free_to_squares_(std::vector<int>()), goal_squares_(std::vector<int>()),
      height_(0), width_(0), size_(0), size_state_(0), entrance_(0), number_objects_(0), stack_(0), 
      squares_to_free_(std::vector<unsigned char>()), dead_squares_on_(std::vector<bool>()), goal_zone_(std::vector<bool>()),
      reverse_man_(std::vector<int>()), reverse_move_(std::vector<int>()),
      man_(std::vector<int>()), move_(std::vector<int>()), offset_(std::vector<int>()),
      number_free_to_squares_(-1)
   {}

      
   const std::vector<int> Instance::ProcessInstance(const std::vector<std::string> &instance)
   {

      //  ------- PreProcess Instance  -------
      //Get Dimensions.
      int start = 0;
      if (instance[0][0] == '!')
      {
         height_ = static_cast<int>(instance.size()) - 1;
         start = 1;
      }
      else 
      {
         height_ = static_cast<int>(instance.size());
      }

      width_ = 0;
      for (int i = start; i < instance.size(); i++) 
      {
         for (int j = (int)instance[i].size() - 1; j >= 0; j--)
         {
            if (instance[i][j] == '#')
            {
               width_ = std::max(width_, j + 1);
               break;
            }
         }         
      }
      
      //Get objects.
      free_squares_on_ = std::vector<bool>(height_ * width_, false);
      goal_squares_on_ = std::vector<bool>(height_ * width_, false);
      wall_squares_on_ = std::vector<bool>(height_ * width_, false);
      std::vector<bool> stones_on = std::vector<bool>(height_ * width_, false);

      size_ = height_ * width_;
      int man_position = 0;
      int count = 0;
      for (int i = 0; i < height_; i++)
      {
         for (int j = 0; j < instance[i + start].size(); j++)
         {
            int linear_position = i * width_ + j;
            if (instance[i + start][j] == '.') //Goal
            {
               goal_squares_on_[linear_position] = true;
            } else if (instance[i + start][j] == '$') //Stone
            {
               stones_on[linear_position] = true;
               count++;
            } else if (instance[i + start][j] == '@')
            {
               man_position = linear_position;
            } else if (instance[i + start][j] == '+')
            {
               goal_squares_on_[linear_position] = true;
               man_position = linear_position;
            } else if (instance[i + start][j] == '*')
            {
               goal_squares_on_[linear_position] = true;
               stones_on[linear_position] = true;
               count++;
            } 
            else if (instance[i + start][j] == '#')
            {
               wall_squares_on_[linear_position] = true;
            }
         }
      }
      
      //  ------- End PreProcess Instance -------

      free_to_squares_ = std::vector<int>(size_, -1);
      squares_to_free_ = std::vector<unsigned char>(size_, -1);
      dead_squares_on_ = std::vector<bool>(size_, false);
      goal_zone_ = std::vector<bool>(size_, false);

      std::vector<int> state;
      for (int i = 0; i < stones_on.size(); i++) {
         if (stones_on[i] == true)
         {
            state.push_back(i);
         }
         if (goal_squares_on_[i] == true)
         {
            goal_squares_.push_back(i);
            number_objects_++;
         }
      }
      size_state_ = state.size();
      state.push_back(man_position);


      //  ------- Domain -------
      // Stores the direction of the movement of man and stone, and the reverse movement of both.
      offset_ = std::vector<int>(4, 0);
      offset_[0] = 1; offset_[1] = -1; offset_[2] = Instance::Width(); offset_[3] = -Instance::Width();
      
      reverse_move_ = std::vector<int>(4, 0);
      reverse_man_ = std::vector<int>(4, 0);

      reverse_move_[0] = 1;   reverse_move_[1] = -1;   reverse_move_[2] = width_;   reverse_move_[3] = -width_;
      reverse_man_[0] = 2 * 1;  reverse_man_[1] = 2 * -1;  reverse_man_[2] = 2 * width_; reverse_man_[3] = 2 * -width_;
      
      move_ = std::vector<int>(4, 0);
      man_ = std::vector<int>(4, 0);

      move_[0] = 1; move_[1] = -1; move_[2] = Instance::Width(); move_[3] = -Instance::Width();
      man_[0] = -1; man_[1] = 1; man_[2] = -Instance::Width(); man_[3] = Instance::Width();

      Reachable();

      SquaresAndFree();
      
      //Instance Decomposition
      GenerateInstanceDecomposition();
      entrance_ = Instance::GetEntrance();
      goal_zone_ = Instance::GetGoalZone();
      //std::cout << "Entrance end instance: " << entrance_ << std::endl;

      Distances distances(*this);
      dead_squares_on_ = distances.DeadSquaresOn(*this);

      return state;
   }
   
   void Instance::Reachable()
   {
      free_squares_on_.assign(size_, false);
      int top = 0;

      std::vector<int> stack(size_);
      stack[top] = goal_squares_[0];
      top++;

      while (top != 0)
      {
         top--;
         int square = stack[top];

         free_squares_on_[square] = true;
         for (size_t i = 0; i < 4; i++)
         {
            if (wall_squares_on_[square + move_[i]] == false &&
               free_squares_on_[square + move_[i]] == false)
            {
               stack[top] = square + move_[i];
               top++;
            }
         }
      }
   }

    void Instance::SquaresAndFree()
    {
       int counter = 0;
       squares_to_free_ = std::vector<unsigned char>(Instance::Size(), -1);
       free_to_squares_ = std::vector<int>(Instance::Size(), -1);
       for (int i = 0; i <  Instance::Size(); i++)
       {
          if (Instance::FreeSquaresOn(i) == true)
          {
             squares_to_free_[i] = counter;
             free_to_squares_[counter] = i;
             counter++;
          }
       }
       number_free_to_squares_ = counter;
    }

   // -------------------- Decomposition --------------------

   const int Instance::ReachableZones( const int &block)
   {
      int reachable_squares = 0;
      goal_zone_.assign(Instance::Size(), false);

      int top = 0;
      for (int goal = 0; goal < Instance::NumberObjects(); goal++)
      {
         if (goal_zone_[Instance::GoalSquares(goal)] ==  false && Instance::GoalSquares(goal) != block)
         {
            stack_[top] = Instance::GoalSquares(goal);
            top++;

            while (top != 0)
            {
               top--;
               int square = stack_[top];
               
               if (goal_zone_[square] == false)
               {
                  goal_zone_[square] = true;
                  reachable_squares++;
               }
               
               for (int i = 0; i < 4; i++)
               {
                  int new_square = square + Instance::Move(i);
                  int man = square + 2 * Instance::Move(i);
                  if (new_square < Instance::Size() && man < Instance::Size() && man >= 0 && new_square>=0 ) 
                  {
                     if (Instance::FreeSquaresOn(man) == true && goal_zone_[new_square] == false &&
                         Instance::FreeSquaresOn(new_square) == true && new_square != block ) 
                     {
                        stack_[top] = new_square;
                        top++;
                     }
                  }
               }
            }
         }
      }
      return reachable_squares;
   }

   void Instance::GenerateInstanceDecomposition()
   {

      stack_ = std::vector<int>(Instance::Size());
      goal_zone_ = std::vector<bool>(Instance::Size());
      entrance_ = INF;
      int number_goal_zone_squares = INF;

      for (int e = 0; e < Instance::Size(); e++)
      {
         if (Instance::FreeSquaresOn(e))
         {         
            int number_local_goal_zone_squares = ReachableZones(e);
            if (number_local_goal_zone_squares < number_goal_zone_squares)
            {
               number_goal_zone_squares = number_local_goal_zone_squares;
               entrance_ = e;
            }
         }
      }

      ReachableZones(entrance_);

      // -------------- Show the Decomposition -------------- 
      /*for (int i = 0; i < Instance::Height(); i++)
      {
         for (int j = 0; j < Instance::Width(); j++)
         {
            int v = i * Instance::Width() + j;
            if (entrance_ == v)
            {
               std::cout << 'E';
            }    
            else if (Instance::GoalSquaresOn(v)) 
            {
               std::cout << '.';
            }
            else if (goal_zone_[v] == true)
            {           
               std::cout << 'I';            
            }
            else if (Instance::WallSquaresOn(v)) 
            {
               std::cout << '#';
            }
            else if (Instance::FreeSquaresOn(v) && Instance::DeadSquaresOn(v) == false) 
            {
               std::cout << 'O';
            }           
            else 
            {
               std::cout << ' ';
            }
         }
         if (i + 1 != Instance::Height())
            std::cout << std::endl;
      }
      std::cout << std::endl << std::endl;*/
      
      stack_.clear();
   }

}
