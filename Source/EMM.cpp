#include "stdafx.h"

/* EMM: heuristic that calculate the minmatching. */

namespace ls
{
	EMM::EMM(const Instance &instance) : distances_(instance)
	{
            int number_nodes = 2 * instance.SizeState();
            int number_edges = instance.SizeState() * instance.SizeState();

            edges_ = new int[number_edges * 2];
            weights_ = new int[number_edges];
	}

	const int EMM::GetLowerbound(const std::vector<int> &state,	const Instance &instance)
	{
		int number_objects = instance.SizeState();
		int number_nodes = 2 * number_objects;

		int number_edges = instance.SizeState() * instance.SizeState();
		PerfectMatching* matching = new PerfectMatching(number_nodes, number_edges);

		struct PerfectMatching::Options options;
		options.verbose = false;
		matching->options = options;

		int counter = 0;
		for (int i = 0; i < number_objects; i++) {
			for (int j = 0; j < number_objects; j++) {
				int man = state[number_objects];
				edges_[counter * 2] = i;
				edges_[counter * 2 + 1] = j + number_objects;
				weights_[counter] = distances_.GetDistance(state[i],
						instance.GoalSquares(j), man);
				matching->AddEdge(edges_[counter * 2], edges_[counter * 2 + 1],
						weights_[counter]);
				counter++;
			}
		}

		matching->Solve();
		int heuristic = ComputePerfectMatchingCost(number_nodes, number_edges,
				edges_, weights_, matching);
		delete matching;

		//	times_called_++;
		//	if (heuristic >= INF) {
		//		times_pruned_++;
		//		goal_lowerbound = INF;
		//	} else {
		//		goal_lowerbound = (int) heurisitc + linear_conflicts_.GetLinearConflicts(state);
		//	}
		//	return goal_lowerbound;
		return heuristic;
	}

	// bool EMM::Initiate(const Instance &instance)
	// {
	//    linear_conflicts_.PreComputeLinearConflicts(instance);
	//    return true;
	// }

	EMM::~EMM() {
		delete[] edges_;
		delete[] weights_;
	}

}
