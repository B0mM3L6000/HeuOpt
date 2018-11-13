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

	//tests von martin (zugriffe auf variablen in klasse)
	unsigned rundenanzahl() { return u_number_rounds; }
	unsigned teamanzahl() { return u_number_teams; }
};

#endif /* HEURISTIC_H_ */
