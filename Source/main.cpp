#include "stdafx.h"

using namespace std;
using namespace ls;

ls::TypeHeuristic HeuristicType(string heuristic)
{
	ls::TypeHeuristic type_heuristic;

	// A -- Simple heuristic

	if (heuristic == "pdb1")
		return type_heuristic = ls::TypeHeuristic::pdb1;
	else if (heuristic == "pdb2")
		return type_heuristic = ls::TypeHeuristic::pdb2;
	else if (heuristic == "pdb3r")
		return type_heuristic = ls::TypeHeuristic::pdb3r;
	else if (heuristic == "pdb4r")
		return type_heuristic = ls::TypeHeuristic::pdb4r;

	// B -- Heuristic + Conflicts

	else if (heuristic == "pdb2_2oc") //[PDB2, PDB2-PDB1]
		return type_heuristic = ls::TypeHeuristic::pdb2_oc2;
	else if (heuristic == "pdb3r_3ocr") //[PDB3r, PDB3r-PDB2]
		return type_heuristic = ls::TypeHeuristic::pdb3r_oc3r;
	else if (heuristic == "pdb4r_4ocr") //[PDB4r, PDB4r-PDB3r]
		return type_heuristic = ls::TypeHeuristic::pdb4r_oc4r;

	// C -- Conflicts + Heuristic

	else if (heuristic == "2oc_pdb2") //[PDB2-PDB1,PDB2]
		return type_heuristic = ls::TypeHeuristic::oc2_pdb2;
	else if (heuristic == "3ocr_pdb3r") //[PDB3r-PDB2,PDB3r]
		return type_heuristic = ls::TypeHeuristic::oc3r_pdb3r;
	else if (heuristic == "4ocr_pdb4r") //[PDB4r-PDB3r,PDB4r]
		return type_heuristic = ls::TypeHeuristic::oc4r_pdb4r;

	// D -- Novelty + Heuristic

	else if (heuristic == "wpdb1_pdb1") //[w(PDB1),PDB1]
		return type_heuristic = TypeHeuristic::wpdb1_pdb1;
	else if (heuristic == "wpdb2_pdb2") //[w(PDB2),PDB2]
		return type_heuristic = ls::TypeHeuristic::wpdb2_pdb2;
	else if (heuristic == "wpdb3r_pdb3r") //[w(PDB3r),PDB3r]
		return type_heuristic = ls::TypeHeuristic::wpdb3r_pdb3r;
	else if (heuristic == "wpdb4r_pdb4r") //[w(PDB4r),PDB4r]
		return type_heuristic = ls::TypeHeuristic::wpdb4r_pdb4r;

	// E -- Novelty + Heuristic + Conflicts

	else if (heuristic == "wpdb2_pdb2_2oc") // [w(PDB2),PDB2,PDB2-PDB1]
			return type_heuristic = ls::TypeHeuristic::wpdb2_pdb2_oc2;
	else if (heuristic == "wpdb3r_pdb3r_3ocr") // [w(PDB3r),PDB3r,PDB3r-PDB2]
		return type_heuristic = ls::TypeHeuristic::wpdb3r_pdb3r_oc3r;
	else if (heuristic == "wpdb4r_pdb4r_4ocr") // [w(PDB4r),PDB4r,PDB4r-PDB3r]
		return type_heuristic = ls::TypeHeuristic::wpdb4r_pdb4r_oc4r;

	// F -- Novelty + Conflicts + Heuristic

	else if (heuristic == "wpbd2_2oc_pdb2") // [w(PDB2),PDB2-PDB1,PDB2]
		return type_heuristic = ls::TypeHeuristic::wpdb2_oc2_pdb2;
	else if (heuristic == "wpbd3r_3ocr_pdb3r") // [w(PDB3r),PDB3r-PDB2,PDB3r]
		return type_heuristic = ls::TypeHeuristic::wpdb3r_oc3r_pdb3r;
	else if (heuristic == "wpdb4r_4ocr_pdb4r") // [w(PDB4r),PDB4r-PDB3r,PDB4r]
		return type_heuristic = ls::TypeHeuristic::wpdb4r_oc4r_pdb4r;
}

void CaseRunInstanceMethod(char* argv[])
{
   //Read files.
   std::string file = argv[1];
   ls::Reader reader(file);

   //Read algorithm
   ls::TypeTest type_test = ls::TypeTest::GBFS;

   // Read heuristic
   std::string heuristic = argv[2];
   ls::TypeHeuristic type_heuristic = HeuristicType(heuristic);

   //Read time
   std::string TIME = argv[3];
   const int LIMIT_TIME = std::stoi(TIME);

   std::string run_time = argv[4];

   // -------------- Printing the results on the files -----------------------------------------------
   // Creation of the files and folders to store the results
   std::string base_folder = "Results/" + run_time + "/";
   std::string heuristic_folder = base_folder + "GBFS-" + heuristic + "-" + TIME;
   mkdir(base_folder.c_str(), 0777);
   mkdir(heuristic_folder.c_str(), 0777);

   std::string folder_results_distances = heuristic_folder + "/Distances/";
   std::string folder_results_instances = heuristic_folder + "/Instances/";
   mkdir(folder_results_distances.c_str(), 0777);
   mkdir(folder_results_instances.c_str(), 0777);

   std::ofstream out_distances(folder_results_distances + file + "-GBFS-" + heuristic + ".txt");
   std::ofstream out_instances(folder_results_instances + file + ".map");

   //Put the number of instance on the file of the result
   out_instances << "!" << file[9] << file[10] << std::endl;
   out_distances << file[9] << file[10] << " GBFS " << heuristic << " ";

   //out_all_distances << "# MEMORY EXPANDED MM PDB1 IPDB2 IPDB3 IPDB4 MPDB2 MPDB3 MPDB4 TIME" << std::endl;
   // ----------------------------------------------------------------------------------------------------

   srand(time(NULL));

   //Run tests.
   for (int i = 0; i < reader.Size(); i++)
   {
      // Pre-Processing the instance.
      ls::Instance instance;
      std::vector<std::string> instace_file(reader.Instace(i));
      std::vector<int> root = instance.ProcessInstance(instace_file);

      //Generate the instance
      ls::GenerateInstance test_case(instance, LIMIT_TIME, type_heuristic);
      std::vector<int> results = test_case.RunHS(file, root, instance, type_heuristic, type_test, out_distances, out_instances);

   }

   out_distances.close();
   out_instances.close();

}


void CaseRunAll()
{
   //Read files.
   std::string file = "GoalxSokoban";
   ls::Reader reader("GoalxSokoban");

   //Read heuristic test.
   std::string algorithm = "GBFS";
   ls::TypeTest type_test = ls::TypeTest::GBFS;

   //Read time.
   std::string TIME = "10";
   const int LIMIT_TIME = std::stoi(TIME);

   // Read heuristic.
   ls::TypeHeuristic type_heuristic;
   std::string heuristic = "4c_pdb4";

   type_heuristic = HeuristicType(heuristic);

   //Read Set Instance used.
   std::string set_instance = "Local";

   const bool pdb4_random = true;

   std::string run_time = "01";

   // Creation of the files and folders to store the results
   std::string base_folder = "Results/" + run_time + "/";
   std::string heuristic_folder = base_folder + "GBFS-" + heuristic + "-" + TIME;
   mkdir(base_folder.c_str(), 0777);
   mkdir(heuristic_folder.c_str(), 0777);

   std::string folder_results_distances = heuristic_folder + "/Distances/";
   std::string folder_results_instances = heuristic_folder + "/Instances/";
   mkdir(folder_results_distances.c_str(), 0777);
   mkdir(folder_results_instances.c_str(), 0777);

   std::ofstream out_distances(folder_results_distances + file + "-GBFS-" + heuristic + ".txt");
   std::ofstream out_instances(folder_results_instances + file + ".map");

   //Put the number of instance on the file of the result
   out_instances << "!" << file[9] << file[10] << std::endl;
   out_distances << file[9] << file[10];

   srand(time(NULL));

   //Run tests.
   for (int i = 0; i < 90; i++)
   {
	   // Pre-processing the instance.
	   ls::Instance instance;
	   std::vector<std::string> instace_file(reader.Instace(i));
	   std::vector<int> root = instance.ProcessInstance(instace_file);

	   //Generate the instance.
	   ls::GenerateInstance test_case(instance, LIMIT_TIME, type_heuristic);
	   std::vector<int> results = test_case.RunHS(file, root, instance, type_heuristic, type_test, out_distances, out_instances);
   }

   // Close the files of the results
   out_distances.close();
   out_instances.close();
}

void HeuristicInitialState()
{
   ls::Reader reader("xSokoban");
   for (int i = 0; i < reader.Size(); i++)
   //for (int i = 36; i <= 36; i++)
   {
      std::cout << i + 1;

      //Pre-Processing the instance
      ls::Instance instance;
      std::vector<std::string> instace_file(reader.Instace(i));

      std::vector<int> root = instance.ProcessInstance(instace_file);

      /*MPDB* mpdb = new MPDB(instance, 4, 60);
      mpdb->Initiate(instance);
      mpdb->Solve(instance, 60);
      delete mpdb;*/

      LowerboundMPDB* mpdb = new LowerboundMPDB(instance, 4, 60);
      mpdb->Initiate(instance, 60);
      //Show::State(root, instance);

      std::cout << " " << mpdb->GetLowerbound(root, instance) << std::endl;
      delete mpdb;
   }
}


int main(int argc, char* argv[])
{
   if (argc > 1)
   {
      CaseRunInstanceMethod(argv);
   }
   else
   {
      //HeuristicInitialState();
      CaseRunAll();
   }
}
