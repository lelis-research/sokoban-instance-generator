#pragma once

namespace ls
{
	enum TypeHeuristic
	{
		// A -- Simple heuristic

		pdb1,
		pdb2,
		pdb3r,
		pdb4r,

		// B -- Heuristic + Conflicts

		pdb2_oc2, //[PDB2, PDB2-PDB1]
		pdb3r_oc3r, //[PDB3r, PDB3r-PDB2]
		pdb4r_oc4r, //[PDB4r, PDB4r-PDB3r]

		// C -- Conflicts + Heuristic

		oc2_pdb2, //[PDB2-PDB1,PDB2]
		oc3r_pdb3r, //[PDB3r-PDB2,PDB3r]
		oc4r_pdb4r, //[PDB4r-PDB3r,PDB4r]

		// D -- Novelty + Heuristic

		wpdb1_pdb1, // [w(PDB1),PDB1]
		wpdb2_pdb2, // [w(PDB2),PDB2]
		wpdb3r_pdb3r, // [w(PDB3r),PDB3r]
		wpdb4r_pdb4r, // [w(PDB4r),PDB4r]

		// E -- Novelty + Heuristic + Conflicts

		wpdb2_pdb2_oc2, // [w(PDB2),PDB2,PDB2-PDB1]
		wpdb3r_pdb3r_oc3r, // [w(PDB3r),PDB3r,PDB3r-PDB2]
		wpdb4r_pdb4r_oc4r, // [w(PDB4r),PDB4r,PDB4r-PDB3r]

		// F -- Novelty + Conflicts + Heuristic

		wpdb2_oc2_pdb2, // [w(PDB2),PDB2-PDB1,PDB2]
		wpdb3r_oc3r_pdb3r, // [w(PDB3r),PDB3r-PDB2,PDB3r]
		wpdb4r_oc4r_pdb4r, // [w(PDB4r),PDB4r-PDB3r,PDB4r]
	};

	enum TypeTest
	{
		GBFS
	};

	class Novelty
	{
		public:
			Novelty(const Instance &instance, const int novelty_size, ls::TypeHeuristic type_heuristic);

			int GetNovelty_2H(const std::vector<int> &state, const Instance &instance, int h_value);
			int GetNovelty_2H_new(const std::vector<int> &state, const Instance &instance, int h_value);
		private:
			std::vector<std::vector<std::vector<std::vector<bool>>>> novelty_two_h;
			std::vector<std::vector<std::vector<bool>>> novelty_one_h;
	};

	class Heuristics
	{
		public:
			Heuristics(const Instance &instance, const TypeHeuristic type_heuristic, const double LIMIT_TIME);
			void GetValueObjectiveFunction(const std::vector<int> &state, const Instance &instance, int &h_value, int &objective_function);

			const int GetMM(const std::vector<int> &state, const Instance &instance) { return mm.GetLowerbound(state, instance); }
			const int GetPDB1(const std::vector<int> &state, const Instance &instance);
			const int GetPDB2(const std::vector<int> &state, const Instance &instance) { return mpdb2.GetLowerbound(state, instance); }
			const int GetPDB3(const std::vector<int> &state, const Instance &instance) { return mpdb3.GetLowerbound(state, instance); }
			const int GetPDB4(const std::vector<int> &state, const Instance &instance) { return mpdb4.GetLowerbound(state, instance); }

			const int GetPDB3_Random(const std::vector<int> &state, const Instance &instance) { return mpdb3.GetLowerbound(state, instance, true); }
			const int GetPDB4_Random(const std::vector<int> &state, const Instance &instance) { return mpdb4.GetLowerbound(state, instance, true); }

			//std::vector<bool> DeadSquaresOn(const Instance &instance) {return mm.DeadSquaresOn(instance);}

		private:
			LowerboundMPDB mpdb2;
			LowerboundMPDB mpdb3;
			LowerboundMPDB mpdb4;
			EMM mm;
			Novelty novelty;
			TypeHeuristic heuristic;

			std::vector<std::vector<int>> square_cost_; //Closest goal.
			Distances distance_;

	};
}

