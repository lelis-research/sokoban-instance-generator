#pragma once

namespace ls
{
   class Reader
   {
   public:
      Reader(const std::string &file);
      const std::vector<std::string> Instace(const int &instance) const {return instaces_[instance];}
      const int Size() const { return (int)instaces_.size();}
   private:
      void ReadFile(const std::string &file);
      std::vector<std::vector<std::string> > instaces_;
   };
}
