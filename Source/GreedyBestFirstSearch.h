#pragma once

namespace ls
{
	class GreedyBestFirstSearch
	{
	public:
		GreedyBestFirstSearch(const Instance &instance);
		std::vector<int> Run(const std::vector<int> & root, const Instance & instance, GetTime time, std::ofstream & out_distances, std::ofstream &out_instance,
			const double LIMIT_TIME, Heuristics &heuristics);
	private:
		Utils utils_;

		std::vector<bool> stones_on_;
		std::vector<bool> reachable_;
		std::vector<int> child_;
		std::vector<bool> child_reachable_;
	};
}
