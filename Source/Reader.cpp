#include "stdafx.h"

/* Reads the input file from start to finish. */

namespace ls
{
   using namespace std;
   Reader::Reader(const std::string &file) {
		 #if defined(__linux__) || defined(__lnx__)
            ReadFile("Instances/" + file + ".map");
         #else
            ReadFile("../Instances/" + file + ".map");
         #endif
   }

   void Reader::ReadFile(const std::string &file)
   {
      ifstream reader;
      reader.open(file.c_str());
      vector<string> instance;

      while(!reader.eof())
      {
         instance.clear();
         string line;
         line.clear();
         getline(reader,line);
         while (line.compare("") != 0 && !reader.eof() && (int)line[0] != 13)  
         {
            instance.push_back(line);
            line.clear();
            getline(reader,line);
         };
         if (reader.eof())
         {
            instance.push_back(line);            
         }
         if (instance.size() > 1)
         {
            instaces_.push_back(instance);
         }         
      }  
      reader.close();
   }
}
