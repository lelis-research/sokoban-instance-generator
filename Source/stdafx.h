#pragma once

//Options
#define MEMORY_FACTOR 0.9
#define INF 10000
#define DEAD_SQUARE 10000
#define DEBUG_STEP_NUMBER_NODES 100000
#define LIMIT_EXPANDED_NODES 2000000
#define DEBUG false
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS

#include <set>
#include <map>
#include <list>
#ifndef __lnx__
#include <omp.h>
#endif
#include <array>
#include <tuple>
#include <cmath>
#include <queue>
#include <vector>
#include <bitset>
#include <string>
#include <fstream>
#include <cstring>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <functional>  
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <stdio.h>
#include <utility> 
#include <algorithm>
#include <chrono>
#include <random>

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/heap/priority_queue.hpp>
#include <boost/heap/binomial_heap.hpp>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/pairing_heap.hpp>
#include <boost/heap/skew_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>
#include <boost/lexical_cast.hpp>

#ifndef __linux__
#include <chrono>
#endif

#include "PerfectMatching.h"

#if defined(__linux__) || defined(__lnx__)
#include <sys/stat.h>
#else
#include <direct.h>
#endif

#include "MemoryUtils.h"

#include "Reader.h"
#include "Instance.h"
#include "Utils.h"
#include "Show.h"
#include "Distances.h"
#include "LinearConflicts.h"
//#include "IPDB.h"
#include "MPDB.h"
//#include "LowerboundPDB.h"
//#include "LowerboundIPDB.h"
#include "LowerboundMPDB.h"
//#include "Neighborhood.h"
#include "EMM.h"
#include "Heuristics.h"
#include "FillOrder.h"


//#include "RandomWalk.h"
//#include "BestImprovement.h"
//#include "BreadthFirstSearch.h"
#include "GreedyBestFirstSearch.h"
//#include "BETA.h"
//#include "AStar.h"
#include "GenerateInstance.h"
