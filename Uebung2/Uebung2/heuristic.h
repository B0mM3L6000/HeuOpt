#ifndef HEURISTIC_H_
#define HEURISTIC_H_

#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include "timeinfo.h"

using namespace std;

class heuristic
{
	string s_instance_name;
	vector<vector<unsigned> > vv_distance;
	vector<pair<unsigned, pair<unsigned, unsigned> > > v_consecutive_opponents;
	unsigned u_number_teams;
	unsigned u_number_rounds;

	vector<pair<unsigned, bool> > v_schedule;

	c_TimeInfo timer;


	// neighborhoods:
	// Function "Test*neighborhoods*" does NOT change the current solution!
	// Input: possible Teams/Rounds
	// Output: delta_cost, num_violation = change in costs if move would be made, number of violations (which cannot be repaired) 0 = nothing is violated >0 number of violations
	// The Function "*neighborhoods*" (without test) changes the current solution
	// Input: Teams/Rounds to change
	pair<int, int> TestSwapHA(unsigned team_i, unsigned team_j);
	void SwapHA(unsigned team_i, unsigned team_j);
	pair<int, int> TestSwapRds(unsigned round_k, unsigned round_l);
	void SwapRds(unsigned round_k, unsigned round_l);
	pair<int, int> TestSwapTms(unsigned team_i, unsigned team_j);
	void SwapTms(unsigned team_i, unsigned team_j);
	pair<int, int> TestPrtSwapRds(unsigned team_i, unsigned round_k, unsigned round_l);
	void PrtSwapRds(unsigned team_i, unsigned round_k, unsigned round_l);
	pair<int, int> TestPrtSwapTms(unsigned team_i, unsigned team_j, unsigned round_k);
	void PrtSwapTms(unsigned team_i, unsigned team_j, unsigned round_k);
	
public:
	//constructors
	heuristic(unsigned u_teamcount, unsigned u_rounds, vector<vector<unsigned> >& v_distance);
	heuristic(string filename);

	void Solution_Uebung0();

	// construct a startingsolution
	void Easy_Startingsolution();
	void Advanced_Startingsolution();

	// getter
	// Input <current team, number of round>
	// Output <opponent, home/away> 
	// if bool = true -> current team play at home
	// if bool = false -> current team play away
	pair<unsigned, bool> Get_Match(unsigned team, unsigned round) {return v_schedule.at((u_number_rounds*(team))+(round));}

	// Input <Team1, Team2>
	// Distance between Team1 and Team2
	unsigned Get_Distance(unsigned team1, unsigned team2) {return vv_distance[team1][team2];}
	
	// Input <Team1, Team2>
	// Output: Number of consecutive opponents of Team2 after Team1
	unsigned Get_Num_Cons_Opp(unsigned team1, unsigned team2) {return v_consecutive_opponents[u_number_teams*(team1)+(team2)].first;}
	
	// setter
	// Input < current team, number of round, opponent of current team, home/away>
	// if bool = true -> current team play at home
	// if bool = false -> current team play away
	void Set_Match(unsigned team, unsigned round, unsigned opponent, bool home){v_schedule.at((u_number_rounds*(team))+(round)) = make_pair(opponent, home);}

	// Input <Team1, Team2, Number of times team1 and team2 are conscutive opponents>
	// team3-team1, team3-team2 -> number = 1
	// additional team4-team1, team4-team2 -> number = 2
	// Set Number of consecutive opponents of Team2 after Team1
	void Set_Num_Cons_Opp(unsigned team1, unsigned team2, unsigned number) {v_consecutive_opponents[u_number_teams*(team1)+(team2)] = make_pair(number, make_pair(team1, team2));}

	// Feasibility check for the schedule
	bool Check_Constraints();

	// IO
	void Read_Schedule( string file );
	void Print_Schedule(bool zero_based);
	void Print_Distance();
	void Print_Consecutives();
	
	// calculates the distance of a given schedule
	unsigned Calculate_Distance();


	// Local Search
	// controls local search for each neighborhood
	// Input k = number of improved solutions (k = 1; 1 < k < 99; k >= 99)
	// d.h. k = 1 take first improvement
	// 1 < k < 99 take best of k improvements
	// k >= 99 take best improvement
	// Output int gain
	int Move_SwapHA(unsigned k);
	int Move_SwapRds(unsigned k);
	int Move_SwapTms(unsigned k);
	int Move_PrtSwapRds(unsigned k);
	int Move_PrtSwapTms(unsigned k);

	//martins hilfsfunktionen:

	//welches runden sind die paarungen?
	pair<int, int> Which_rounds(unsigned team_i, unsigned team_j);
	//distanzen vor swap:
	int distance_vorher(unsigned team_i, unsigned round);
	//distanzen nach swap HA:
	int distance_nachher_vorne(unsigned team_i, unsigned round);
	int distance_nachher_hinten(unsigned team_i, unsigned round);
	//distanzen nach swap RNDS:
	int distance_nachher_vorne2(unsigned team_i, unsigned round, unsigned swappedround);
	int distance_nachher_hinten2(unsigned team_i, unsigned round, unsigned swappedround);
	int distance_nachher_mitte(unsigned team, unsigned round, unsigned swappedround);
	//homeawaytest bei swapHA
	bool swapHA_homeaway_test(unsigned team, unsigned round1, unsigned round2);
	//norepeater check:
	bool norepeater(unsigned team, unsigned round1, unsigned round2);

};

#endif /* HEURISTIC_H_ */
