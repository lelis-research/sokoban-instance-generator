#pragma once

namespace ls
{
   class Instance {
   public:
      Instance();
      const std::vector<int> ProcessInstance(const std::vector<std::string> &instance);

      void Reachable();
      
      // Instance

      inline const bool FreeSquaresOn(const int &i) const { return free_squares_on_[i]; }
      inline const bool GoalSquaresOn(const int &i) const { return goal_squares_on_[i]; }
      inline const bool WallSquaresOn(const int &i) const { return wall_squares_on_[i]; }
      inline const bool DeadSquaresOn(const int &i) const { return dead_squares_on_[i]; }

      inline const int GoalSquares(const int &i) const { return goal_squares_[i]; }

      inline const int FreeToSquares(const int &i) const { return free_to_squares_[i]; }
      inline const unsigned char SquaresToFree(const int &i) const { return squares_to_free_[i]; }
      inline const int NumberObjects() const { return number_objects_; }
      inline const int NumberFreeSquares() const { return number_free_to_squares_; }

      inline const int Height() const { return height_; }
      inline const int Width() const { return width_; }
      inline const int Size() const { return size_; }

      inline const int GetEntrance() const {return entrance_;}

      inline const bool IsGoalZone(const int &i) const{return goal_zone_[i];}
      inline const bool IsGoalZoneOrEntrance(const int &i) const{return goal_zone_[i] || entrance_==i;}

      // Decomposition
      inline std::vector<bool> GetGoalZone() const {return goal_zone_;}
      void GenerateInstanceDecomposition();   

      // Domain

      inline const int ReverseMan(const int &i) const { return reverse_man_[i]; }
      inline const int ReverseMove(const int &i) const { return reverse_move_[i]; }
      inline const int SizeMovesMan() const { return 4; }
      inline const int SizeState() const { return size_state_; }
      inline const int Man(const int &i) const { return man_[i]; }
      inline const int Move(const int &i) const { return move_[i]; }

      inline const int Offset(const int &i) const { return offset_[i]; }
   private:
      void SquaresAndFree();
      
      std::vector<bool> free_squares_on_;      
      std::vector<bool> goal_squares_on_;      
      std::vector<bool> wall_squares_on_; 
      std::vector<bool> dead_squares_on_;
      std::vector<int> goal_squares_;
      
      std::vector<int> free_to_squares_;
      std::vector<unsigned char> squares_to_free_;

      std::vector<bool> goal_zone_;
      
      int entrance_;
      int number_objects_;
      int number_free_to_squares_;

      int width_;
      int height_;
      int size_;
      int size_state_;

      //Domain 

      std::vector<int> reverse_man_;
      std::vector<int> reverse_move_;
      std::vector<int> man_;
      std::vector<int> move_;

      std::vector<int> offset_;

      const int ReachableZones( const int &block);
      std::vector<int> stack_;

   };
}
