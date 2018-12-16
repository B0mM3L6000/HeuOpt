#include "heuristic.h"
#include "timeinfo.h"
#include <algorithm>
#include <fstream>

using namespace std;

void heuristic::Solution_Uebung0()
{
	// Solution Rechnerübung 0
	cout << endl;
	Print_Schedule(true);
	cout << endl;

	timer.Start();

	// 1. Ausgabe Gegner von Team 2 in Runde 30
	cout << "1.) Team 2 plays in round 3 against team " << Get_Match(1, 2).first + 1;
	if (Get_Match(1, 2).second)
		cout << " at home." << endl << endl;
	else
		cout << " away." << endl << endl;

	// 2. Unzulässigkeit beheben
	Set_Match(1, 4, 3, false);
	// Test
	cout << endl;
	Print_Schedule(true);
	cout << endl;

	// 3. Berechne und Speichere consecOpp
	// Compute the number of consecutive opponents
	int t1 = 1;
	int t2 = 3;
	int counter = 0;
	for (unsigned n = 1; n < u_number_rounds; n++)
	{
		if (Get_Match(t1, n - 1).first == Get_Match(t2, n).first)
			counter++;
		if (Get_Match(t1, n).first == Get_Match(t2, n - 1).first)
			counter++;
	}
	Set_Num_Cons_Opp(t1, t2, counter);
	cout << "3. a) consecOpp(" << t1 << "," << t2 << ") = " << Get_Num_Cons_Opp(t1, t2) << "." << endl << endl;

	t1 = 3;
	t2 = 2;
	counter = 0;
	for (unsigned n = 1; n < u_number_rounds; n++)
	{
		if (Get_Match(t1, n - 1).first == Get_Match(t2, n).first)
			counter++;
		if (Get_Match(t1, n).first == Get_Match(t2, n - 1).first)
			counter++;
	}
	Set_Num_Cons_Opp(t1, t2, counter);
	cout << "3. b) consecOpp(" << t1 << "," << t2 << ") = " << Get_Num_Cons_Opp(t1, t2) << "." << endl << endl;

	timer.Stop();
	// 4. SolutionFile erstellen
	bool neueDatei = false;
	string filename = "solution_ue0.csv";
	ifstream temp(filename);
	if (!temp) neueDatei = true;
	else temp.close();

	ofstream stream(filename, ios::app);
	locale myloc("");
	stream.imbue(myloc);

	if (neueDatei)
		stream << "Instanzname;Anzahl Teams;consecOpp(1,3);consecOpp(3,2);Dist;Time;" << endl; // ';'= column separator
																							   //Name
	stream << s_instance_name << ";";
	//Num Teams
	stream << u_number_teams << ";";
	//consecOpp(2,4)
	stream << Get_Num_Cons_Opp(1, 3) << ";";
	//consecOpp(4,3)
	stream << Get_Num_Cons_Opp(3, 2) << ";";
	//Dist
	stream << Calculate_Distance() << ";";
	//Time
	stream << timer.Seconds() << ";";
	stream << endl;
}

//method creates a starting solution
void heuristic::Easy_Startingsolution()
{
	//create a starting solution with the polygon method
	vector<int> nodes(u_number_teams, 0);

	// Initialize polygon
	for (unsigned j = 0; j < u_number_teams; j++)
		nodes[j] = j;

	// Create number_teams - 1 rounds
	for (unsigned n = 0; n < u_number_teams - 1; n++)
	{
		// save schedule
		Set_Match(nodes.front(), n, nodes.back(), true);
		Set_Match(nodes.back(), n, nodes.front(), true);

		for (unsigned l = 1; l < (u_number_teams / 2); l++)
		{
			Set_Match(nodes[l], n, nodes[u_number_teams - l - 1], true);
			Set_Match(nodes[u_number_teams - l - 1], n, nodes[l], true);
		}


		// turn polygon
		int rem_node_team2 = nodes[u_number_teams - 2];
		for (int j = (u_number_teams - 2); j > 0; j--)
			nodes[j] = nodes[j - 1];

		nodes[0] = rem_node_team2;
	}

	// mirrored schedule
	for (unsigned t = 0; t < u_number_teams; t++)
	{
		for (unsigned n = 0; n < u_number_rounds / 2; n++)
		{
			int opponent = Get_Match(t, n).first;
			Set_Match(t, n + u_number_rounds / 2, opponent, true);
		}
	}

	// Assign stadiums to games
	vector<int> consecutive(u_number_teams, 1);
	vector<int> consecutive_start_of_schedule(u_number_teams, 1);
	for (unsigned n = 0; n < u_number_rounds / 2; n++)
	{
		vector<bool> set_team(u_number_teams, false);
		if (n == 0)
		{
			// random assignment of home /away in the first round
			for (unsigned t = 0; t < u_number_teams; t++)
			{
				if (!set_team[t])
				{
					bool random = false;
					if (rand() % 2 == 1)
						random = true;
					int opponent = Get_Match(t, n).first;

					set_team[t] = true;
					set_team[opponent] = true;

					Set_Match(t, n, opponent, random);
					Set_Match(opponent, n, t, !random);
				}
			}
		}
		else if (n < u_number_rounds / 2 - 1)
		{

			for (unsigned t = 0; t < u_number_teams; t++)
			{
				// team t (and opponent) are not assigned
				if (!set_team[t])
				{
					int opponent = Get_Match(t, n).first;
					set_team[t] = true;
					set_team[opponent] = true;

					// Case 1
					if (consecutive[t] < consecutive[opponent])
					{
						bool home_opponent = Get_Match(opponent, n - 1).second;
						bool home_t = Get_Match(t, n - 1).second;
						// Case 1a
						if (home_opponent)
						{
							Set_Match(t, n, opponent, true);
							Set_Match(opponent, n, t, false);
							if (home_t)
								consecutive[t]++;
							else
								consecutive[t] = 1;
						}
						// Case 1b
						else
						{
							Set_Match(t, n, opponent, false);
							Set_Match(opponent, n, t, true);
							if (!home_t)
								consecutive[t]++;
							else
								consecutive[t] = 1;
						}
						consecutive[opponent] = 1;
					}
					// Case 2
					else if (consecutive[t] > consecutive[opponent])
					{
						bool home_opponent = Get_Match(opponent, n - 1).second;
						bool home_t = Get_Match(t, n - 1).second;
						// Case 2a
						if (home_t)
						{
							Set_Match(t, n, opponent, false);
							Set_Match(opponent, n, t, true);
							if (home_opponent)
								consecutive[opponent]++;
							else
								consecutive[opponent] = 1;
						}
						// Case 2b
						else
						{
							Set_Match(t, n, opponent, true);
							Set_Match(opponent, n, t, false);
							if (!home_opponent)
								consecutive[opponent]++;
							else
								consecutive[opponent] = 1;
						}
						consecutive[t] = 1;
					}
					// Case 3
					else //consecutive[t] == consecutive[opponent]
					{
						bool home_opponent = Get_Match(opponent, n - 1).second;
						bool home_t = Get_Match(t, n - 1).second;
						// Case 3a
						if (home_t && !home_opponent)
						{
							Set_Match(t, n, opponent, false);
							Set_Match(opponent, n, t, true);

							consecutive[opponent] = 1;
							consecutive[t] = 1;
						}
						// Case 3b
						else if (!home_t && home_opponent)
						{
							Set_Match(t, n, opponent, true);
							Set_Match(opponent, n, t, false);
							consecutive[opponent] = 1;
							consecutive[t] = 1;
						}
						// Case 3c
						else
						{
							bool random = false;
							if (rand() % 2 == 1)
								random = true;

							// both consecutive[t] >= 3 and consecutive[opponent] >= 3  -->   generate new schedule
							if (consecutive[t] >= 3)
							{
								n = 0;
								for (unsigned t = 0; t < u_number_teams; t++)
									consecutive[t] = 1;

								//cout << "Komplett neuen Spielplan erstellen!" << endl;

								break;
							}
							// random assignment
							else
							{
								Set_Match(t, n, opponent, random);
								Set_Match(opponent, n, t, !random);

								if (home_t == random)
									consecutive[t]++;
								else
									consecutive[t] = 1;

								if (home_opponent == !random)
									consecutive[opponent]++;
								else
									consecutive[opponent] = 1;
							}
						}
					}
				}
			}
			// U == 3 ( Max Consecutives)
			// remember first 3 rounds
			if (n == 3)
			{
				for (unsigned t = 0; t < u_number_teams; t++)
				{
					// 0. round == 1. round
					if (Get_Match(t, 0).second == Get_Match(t, 1).second)
					{
						// + 1. round == 2. round
						if (Get_Match(t, 1).second == Get_Match(t, 2).second)
							// 3 times only home or away
							consecutive_start_of_schedule[t] = 3;
						else
							// 2 times only home or away
							consecutive_start_of_schedule[t] = 2;
					}
					else
						// 1 time only home or away
						consecutive_start_of_schedule[t] = 1;
				}
			}
		}
		// last round of first half
		// home/away in the last round of the first half is also assigned randomly
		else
		{
			for (unsigned t = 0; t < u_number_teams; t++)
			{
				if (!set_team[t])
				{
					bool random = false;
					if (rand() % 2 == 1)
						random = true;
					int opponent = Get_Match(t, n).first;

					set_team[t] = true;
					set_team[opponent] = true;

					// both >= 3  -->  new schedule
					if (consecutive[t] >= 3 && consecutive[opponent] >= 3)
					{
						n = 0;
						for (unsigned t = 0; t < u_number_teams; t++)
							consecutive[t] = 1;

						//cout << "Komplett neuen Spielplan erstellen!" << endl;

						break;
					}
					// team t played 3 times home or 3 times away
					// try valid assignment
					else if (consecutive[t] >= 3)
					{
						bool home_t = Get_Match(t, n - 1).second;
						if ((consecutive_start_of_schedule[opponent] + consecutive[opponent] > 3 && Get_Match(opponent, 1).second == home_t)
							|| (consecutive_start_of_schedule[t] + consecutive[t] > 3 && Get_Match(t, 1).second == !home_t))
						{
							// valid assignment not possible  -->  new schedule
							n = 0;
							for (unsigned t = 0; t < u_number_teams; t++)
								consecutive[t] = 1;

							//cout << "Komplett neuen Spielplan erstellen!" << endl;

							break;
						}
						// valid assignment possible
						else
						{
							Set_Match(t, n, opponent, !home_t);
							Set_Match(opponent, n, t, home_t);
						}

					}
					// team opponent played 3 times home or 3 times away
					// try valid assignment
					else if (consecutive[opponent] >= 3)
					{
						bool home_opponent = Get_Match(opponent, n - 1).second;
						if ((consecutive_start_of_schedule[opponent] + consecutive[opponent] > 3 && Get_Match(opponent, 1).second == !home_opponent)
							|| (consecutive_start_of_schedule[t] + consecutive[t] > 3 && Get_Match(t, 1).second == home_opponent))
						{
							// valid assignment not possible  -->  new schedule
							n = 0;
							for (unsigned t = 0; t < u_number_teams; t++)
								consecutive[t] = 1;

							//cout << "Komplett neuen Spielplan erstellen!" << endl;

							break;
						}
						// valid assignment possible
						else
						{
							Set_Match(t, n, opponent, home_opponent);
							Set_Match(opponent, n, t, !home_opponent);
						}
					}
					// also valid for second half?
					else
					{
						if ((consecutive_start_of_schedule[opponent] + consecutive[opponent] >= 3 && Get_Match(opponent, 1).second == random)
							|| (consecutive_start_of_schedule[t] + consecutive[t] >= 3 && Get_Match(t, 1).second == !random))
						{
							// not valid  -->  new schedule
							n = 0;
							for (unsigned t = 0; t < u_number_teams; t++)
								consecutive[t] = 1;

							//cout << "Komplett neuen Spielplan erstellen!" << endl;

							break;
						}
						// valid assignment possible
						else
						{
							Set_Match(t, n, opponent, random);
							Set_Match(opponent, n, t, !random);
						}
					}
				}
			}
		}
	}

	// mirrored schedule for home/away
	for (unsigned n = 0; n < u_number_rounds / 2; n++)
	{
		for (unsigned t = 0; t < u_number_teams; t++)
		{
			Set_Match(t, u_number_rounds / 2 + n, Get_Match(t, n).first, !Get_Match(t, n).second);
		}
	}
}

//method creates a starting solution
void heuristic::Advanced_Startingsolution()
{
	// Use schedule from before
	Easy_Startingsolution();
	cout << "Die 1. berechnete Startloesung kostet: " << Calculate_Distance() << endl;

	// Compute the number of consecutive opponents
	for (unsigned t1 = 0; t1 < u_number_teams; t1++)
	{
		for (unsigned t2 = 0; t2 < u_number_teams; t2++)
		{
			int counter = 0;

			for (unsigned n = 1; n < u_number_rounds; n++)
			{
				if (Get_Match(t1, n - 1).first == Get_Match(t2, n).first)
					counter++;
			}
			Set_Num_Cons_Opp(t1, t2, counter);
		}
	}

	// Assign real teams to abstract teams
	// Sort pairs of abstract teams corresponding to the consecutive matrix
	sort(v_consecutive_opponents.begin(), v_consecutive_opponents.end());

	// Calculate minimal distance to closest neighbor (real teams)
	// <Distanz, <aktuelles Team, nächster Nachbar> >
	vector<pair<int, pair<int, int> > > shortest_distance(u_number_teams);
	for (unsigned t1 = 0; t1 < u_number_teams; t1++)
	{
		unsigned minimum = 9999999;
		for (unsigned t2 = 0; t2 < u_number_teams; t2++)
		{
			if (t1 != t2)
			{
				if (vv_distance[t1][t2] < minimum)
				{
					minimum = vv_distance[t1][t2];
					shortest_distance[t1] = make_pair(minimum, make_pair(t1, t2));
				}
			}
		}
	}

	// sort according to ascending distance
	sort(shortest_distance.begin(), shortest_distance.end());

	// Assignment real to abstract teams
	// safe if real / abstract team is already set
	vector<bool> set_real_teams(u_number_teams, false);
	vector<bool> set_abstract_teams(u_number_teams, false);
	// save assignment real <-> abstract team
	vector<int> get_abstract_of_real_team(u_number_teams, -1);
	vector<int> get_real_of_abstract_team(u_number_teams, -1);

	// go over all  teams
	for (unsigned t1 = 0; t1 < u_number_teams; t1++)
	{
		int real_team1 = shortest_distance[t1].second.first;
		int real_team2 = shortest_distance[t1].second.second;

		// team 1 is already set --> continue
		if(set_real_teams[real_team1])
			continue;

		// team 1 will be assigned
		set_real_teams[real_team1] = true;

		// Case 1) closest neighbor is already set
		// closest neighbor = real_team2
		if (set_real_teams[real_team2])
		{
			// Find first pair (a,b) with a = abstraktesTeam(real_Team2) and b not set
			int abstract_team2 = get_abstract_of_real_team[real_team2];
			for (int iterator = v_consecutive_opponents.size() - 1; iterator > 0; iterator--)//iterate backwards
			{
				unsigned a = v_consecutive_opponents[iterator].second.first;
				unsigned b = v_consecutive_opponents[iterator].second.second;
				if (a == abstract_team2 && !set_abstract_teams[b])
				{
					// Set b = abstraktesTeam(real_Team1) 
					set_abstract_teams[b] = true;
					get_abstract_of_real_team[real_team1] = b;
					get_real_of_abstract_team[b] = real_team1;
					break;
				}
			}
		}
		// Case 2) closest neighbor is not set
		else
		{
			// Find first pair (a,b) with both a and b not set
			for (int iterator = v_consecutive_opponents.size() - 1; iterator > 0; iterator--)//iterate backwards
			{
				unsigned a = v_consecutive_opponents[iterator].second.first;
				unsigned b = v_consecutive_opponents[iterator].second.second;
				if (!set_abstract_teams[a] && !set_abstract_teams[b])
				{
					// assign either (a to real_Team1 and b to real_Team2) or vice versa
					// here: a = abstraktesTeam(real_Team1) and b = abstraktesTeam(real_Team2)
					set_abstract_teams[a] = true;
					get_abstract_of_real_team[real_team1] = a;
					get_real_of_abstract_team[a] = real_team1;
					set_abstract_teams[b] = true;
					get_abstract_of_real_team[real_team2] = b;
					get_real_of_abstract_team[b] = real_team2;
					set_real_teams[real_team2] = true;
					break;
				}
			}
		}
	}

	// Change schedule (realTeam(abstraktTeam) instead of abstraktTeam)
	// Intermediate step: Save schedule in a new schedule (easier to change rows as a last step)
	vector<vector<pair<int, bool> > > new_schedule(u_number_teams);
	for (unsigned team = 0; team < u_number_teams; team++)
	{
		vector<pair<int, bool> > new_schedule_for_current_real_team;
		int new_team_number = get_real_of_abstract_team[team];

		for (unsigned n = 0; n < u_number_rounds; n++)
		{
			int current_opponent = Get_Match(team, n).first;
			int new_opponent_number = get_real_of_abstract_team[current_opponent];
			bool home = Get_Match(team, n).second;

			new_schedule_for_current_real_team.push_back(make_pair(new_opponent_number, home));
		}
		new_schedule[new_team_number] = new_schedule_for_current_real_team;
	}

	// update the schedule
	for (unsigned t = 0; t < u_number_teams; t++)
	{
		for (unsigned n = 0; n < u_number_rounds; n++)
		{
			Set_Match(t, n , new_schedule[t][n].first, new_schedule[t][n].second);
		}
	}

	cout << "Die 2. berechnete Startloesung kostet: " << Calculate_Distance() << endl;
}


bool heuristic::Check_Constraints()
{
	for (unsigned t = 0; t < u_number_teams; t++)
	{
		int counter_consecutives = 0;
		vector<int> counter_opponent(u_number_teams, 0);//each team should play 2 times against all teams
		vector<pair<bool, bool> > counter_home(u_number_teams, make_pair(false, false));//does home / away fit?

		for (unsigned n = 0; n < u_number_rounds; n++)
		{
			// 1. Played 2 times against each team?
			counter_opponent[Get_Match(t, n).first]++;
			// does home / away fit?
			if (Get_Match(t, n).second)
				counter_home[Get_Match(t, n).first].first = true;
			else
				counter_home[Get_Match(t, n).first].second = true;

			if (n >= 1)
			{
				// 2. count home/away (should be <= 3)
				if (Get_Match(t, n - 1).second == Get_Match(t, n).second)
				{
					counter_consecutives++;
					if (counter_consecutives > 3)
					{
						cout << "In Runde " << n << " spielt Team " << t << " mehr als 3 mal home oder away!" << endl;
						return false;
					}
				}
				else
					counter_consecutives = 1;

				// 3. no-repeater
				if (Get_Match(t, n - 1).first == Get_Match(t, n).first)
				{
					cout << "In Runde " << n << " spielt Team " << t << " gegen denselber Gegner wie in der vorherigen Runde" << endl;
					return false;
				}
			}
		}
		for (unsigned team = 0; team < u_number_teams; team++)
		{
			if (t == team)
				continue;
			// 1. Played 2 times against each team?
			if (counter_opponent[team] != 2)
			{
				cout << "Team " << t << " spielt nicht genau zweimal gegen: " << team << "!" << endl;
				return false;
			}
			// does home / away fit?
			if (!counter_home[team].first || !counter_home[team].second)
			{
				cout << "Team " << t << " spielt nicht einmal home und einmal away gegen Team: " << team << "!" << endl;
				return false;
			}
		}
	}

	for (unsigned n = 0; n < u_number_rounds; n++)
	{
		for (unsigned t = 0; t < u_number_teams; t++)
		{
			// 1. Is home/away assignment ok?
			if (Get_Match(t, n).second == Get_Match(Get_Match(t, n).first, n).second)
			{
				cout << "In Runde " << n << " spielt Team " << t << " nicht das passende home oder away!" << endl;
				return false;
			}
			// 2. Do the right opponents play against each other?
			if (t != Get_Match(Get_Match(t, n).first, n).first)
			{
				cout << "In Runde " << n << " spielt der Gegner von Team " << t << "(Gegner= " << Get_Match(t, n).first << ") nicht gegen Team " << t << "!" << endl;
				return false;
			}
		}
	}
	return true;
}

// ---------------------------------------------------------------------------------------------------------->
// ab hier: Hilfsfunktionen, die Sie benutzten können, aber deren Aufbau irrelevant ist
// ---------------------------------------------------------------------------------------------------------->

// constructor for the heuristic
heuristic::heuristic(unsigned u_teamcount, unsigned u_rounds, vector<vector<unsigned> >& v_distance)
	:u_number_teams(u_teamcount),
	u_number_rounds(u_rounds),
	vv_distance(v_distance)
{
	v_schedule.resize(u_number_teams*u_number_rounds);
	v_consecutive_opponents.resize(u_number_teams*u_number_teams);
	c_TimeInfo dummy_time;
	dummy_time.Start();
	srand(atoi(dummy_time.FormattedTime().c_str()));
}

heuristic::heuristic(string file)
	:s_instance_name(file)
{
	ifstream instfile;
	instfile.open(file.c_str(), ios::in);

	if (instfile.is_open())
	{
		//read settings
		instfile >> u_number_teams;

		//resize vector for saving instance
		vv_distance.resize(u_number_teams);
		for(int i = 0; i < (int)u_number_teams; i++)
			vv_distance[i].resize(u_number_teams);

		// Set number of rounds
		u_number_rounds = 2*(u_number_teams-1);

		//read distance matrix
		for (int i=0; i<(int)u_number_teams; i++)
		{
			for (int j =0; j<(int)u_number_teams; j++)
			{
				instfile >> vv_distance[i][j];
			}
		}
	}
	else 
	{
		cout << "Could not read file";
	}
	instfile.close();

	v_schedule.resize(u_number_teams*u_number_rounds);
	v_consecutive_opponents.resize(u_number_teams*u_number_teams);
	c_TimeInfo dummy_time;
	dummy_time.Start();
	srand(atoi(dummy_time.FormattedTime().c_str()));
}


////RECHNERÜBUNG 2///

//hilfsfunktionen:
pair<int, int> heuristic::Which_rounds(unsigned team_i, unsigned team_j) {
	//in welchen runden spielen die Teams gegeneinander?
	unsigned hinrunde = 0;
	unsigned rueckrunde = 0;
	for (unsigned i = 0; i < u_number_rounds / 2; i++) {
		unsigned enemy = Get_Match(team_i, i).first;
		if (enemy == team_j) {
			hinrunde = i;
			rueckrunde = i + (u_number_rounds / 2);
			break;
		}
	}
	//cout << hinrunde << " " << rueckrunde << endl;
	return make_pair(hinrunde, rueckrunde);
}

//distanz funktionen fuer swapHA

//distanz vor swap (für beide seiten nutzbar)
int heuristic::distance_vorher(unsigned team, unsigned round) {
	int distance = 0;
	if (round != 0 && round != u_number_rounds) {
		if (Get_Match(team, round - 1).second == false && Get_Match(team, round).second == false) {
			distance = Get_Distance(Get_Match(team, round - 1).first, Get_Match(team, round).first);
		}
		else if (Get_Match(team, round - 1).second == false && Get_Match(team, round).second == true) {
			distance = Get_Distance(Get_Match(team, round - 1).first, team);
		}
		else if (Get_Match(team, round - 1).second == true && Get_Match(team, round).second == false) {
			distance = Get_Distance(team, Get_Match(team, round).first);
		}
	}
	else if (round == 0) {
		if (Get_Match(team, round).second == false) {
			distance = Get_Distance(team, Get_Match(team, round).first);
		}
	}
	else if (round == u_number_rounds) {
		if (Get_Match(team, round - 1).second == false) {
			distance = Get_Distance(Get_Match(team, round - 1).first, team);
		}
	}
	return distance;

}

//distanz nach swap (links) HA
int heuristic::distance_nachher_vorne(unsigned team, unsigned round) {
	int distance = 0;
	if (round != 0 && round != u_number_rounds) {
		if (Get_Match(team, round - 1).second == false && !Get_Match(team, round).second == false) {
			distance = Get_Distance(Get_Match(team, round - 1).first, Get_Match(team, round).first);
		}
		else if (Get_Match(team, round - 1).second == false && !Get_Match(team, round).second == true) {
			distance = Get_Distance(Get_Match(team, round - 1).first, team);
		}
		else if (Get_Match(team, round - 1).second == true && !Get_Match(team, round).second == false) {
			distance = Get_Distance(team, Get_Match(team, round).first);
		}
	}
	else if (round == 0) {
		if (!Get_Match(team, round).second == false) {
			distance = Get_Distance(team, Get_Match(team, round).first);
		}
	}
	return distance;

}

//distanz nach swap (links) RNDS
int heuristic::distance_nachher_vorne2(unsigned team, unsigned round, unsigned swappedround) {
	int distance = 0;
	if (round != 0 && round != u_number_rounds) {
		if (Get_Match(team, round - 1).second == false && Get_Match(team, swappedround).second == false) {
			distance = Get_Distance(Get_Match(team, round - 1).first, Get_Match(team, swappedround).first);
		}
		else if (Get_Match(team, round - 1).second == false && Get_Match(team, swappedround).second == true) {
			distance = Get_Distance(Get_Match(team, round - 1).first, team);
		}
		else if (Get_Match(team, round - 1).second == true && Get_Match(team, swappedround).second == false) {
			distance = Get_Distance(team, Get_Match(team, swappedround).first);
		}
	}
	else if (round == 0) {
		if (Get_Match(team, swappedround).second == false) {
			distance = Get_Distance(team, Get_Match(team, swappedround).first);
		}
	}
	return distance;

}

//distanz nach swap rechts HA
int heuristic::distance_nachher_hinten(unsigned team, unsigned round) {
	int distance = 0;
	if (round != 0 && round != u_number_rounds) {
		if (!Get_Match(team, round - 1).second == false && Get_Match(team, round).second == false) {
			distance = Get_Distance(Get_Match(team, round - 1).first, Get_Match(team, round).first);
		}
		else if (!Get_Match(team, round - 1).second == false && Get_Match(team, round).second == true) {
			distance = Get_Distance(Get_Match(team, round - 1).first, team);
		}
		else if (!Get_Match(team, round - 1).second == true && Get_Match(team, round).second == false) {
			distance = Get_Distance(team, Get_Match(team, round).first);
		}
	}
	else if (round == u_number_rounds) {
		if (!Get_Match(team, round - 1).second == false) {
			distance = Get_Distance(Get_Match(team, round - 1).first, team);
		}
	}
	return distance;

}

//distanz nach swap rechts RNDS
int heuristic::distance_nachher_hinten2(unsigned team, unsigned round, unsigned swappedround) {
	int distance = 0;
	if (round != 0 && round != u_number_rounds) {
		if (Get_Match(team, swappedround).second == false && Get_Match(team, round).second == false) {
			distance = Get_Distance(Get_Match(team, swappedround).first, Get_Match(team, round).first);
		}
		else if (Get_Match(team, swappedround).second == false && Get_Match(team, round).second == true) {
			distance = Get_Distance(Get_Match(team, swappedround).first, team);
		}
		else if (Get_Match(team, swappedround).second == true && Get_Match(team, round).second == false) {
			distance = Get_Distance(team, Get_Match(team, round).first);
		}
	}
	else if (round == u_number_rounds) {
		if (Get_Match(team, swappedround).second == false) {
			distance = Get_Distance(Get_Match(team, swappedround).first, team);
		}
	}
	return distance;

}

//distanz nach swap mitte RNDS
int heuristic::distance_nachher_mitte(unsigned team, unsigned round, unsigned swappedround) {
	int distance = 0;
	if (round != 0 && round != u_number_rounds) {
		if (Get_Match(team, swappedround).second == false && Get_Match(team, round).second == false) {
			distance = Get_Distance(Get_Match(team, swappedround).first, Get_Match(team, round).first);
		}
		else if (Get_Match(team, swappedround).second == false && Get_Match(team, round).second == true) {
			distance = Get_Distance(Get_Match(team, swappedround).first, team);
		}
		else if (Get_Match(team, swappedround).second == true && Get_Match(team, round).second == false) {
			distance = Get_Distance(team, Get_Match(team, round).first);
		}
	}
	return distance;

}


//test ob home/away max 3mal in folge für swapHA:
bool heuristic::swapHA_homeaway_test(unsigned team, unsigned round1, unsigned round2) {
	int counter;
	bool currentround;
	bool tmp;

	if ((round2 - round1) <= 3) { //edge case dass beide nur zwei oder weniger voneiander weg liegen:
		if (round1 < 3 && round2 > u_number_rounds - 4) { //edge case beide seiten -> ganze reihe
			counter = 1;
			if (round1 == 0) {
				tmp = Get_Match(team, round2).second;
			}
			else {
				tmp = Get_Match(team, 0).second;
			}
			for (int i = 1; i < u_number_rounds; i++) {
				if (round1 == i) {
					currentround = Get_Match(team, round2).second;
				}
				else if (round2 == i) {
					currentround = Get_Match(team, round1).second;
				}
				else {
					currentround = Get_Match(team, i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " << round2 - 2 + i << " bei Team " << team << endl;;
					return false;
				}
			}
		}
		else if (round1 < 3) { //edge case links
			counter = 1;
			if (round1 == 0) {
				tmp = Get_Match(team, round2).second;
			}
			else {
				tmp = Get_Match(team, 0).second;
			}
			for (int i = 1; i < round2 + 4; i++) {
				if (i == round1) {
					currentround = Get_Match(team, round2).second;
				}
				else if (i == round2) {
					currentround = Get_Match(team, round1).second;
				}
				else {
					currentround = Get_Match(team, i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " <<  i << " bei Team " << team << endl;
					return false;

				}
			}
		}

		else if (round2 > u_number_rounds - 4) { //edge case rechts
			counter = 1;
			tmp = Get_Match(team, round1 - 3).second;
			for (int i = 1; i < (u_number_rounds - round2 + 3 +(round2-round1)); i++) {
				if (i == 3) {
					currentround = Get_Match(team, round2).second;
				}
				else if (i == 3 + round2 - round1) {
					currentround = Get_Match(team, round1).second;
				}
				else {
					currentround = Get_Match(team, round1 - 3 + i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " << round2 - 2 + i << " bei Team " << team << endl;;
					return false;
				}
			}
		}

		else { //kein edge case
			counter = 1;
			tmp = Get_Match(team, round1 - 3).second;
			for (int i = 1; i < 7 + round2 - round1; i++) {
				if (i == 3) {
					currentround = Get_Match(team, round2).second;
				}
				else if (i == 3 + round2 - round1) {
					currentround = Get_Match(team, round1).second;
				}
				else {
					currentround = Get_Match(team, round1 - 3 + i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " << round2 - 2 + i << " bei Team " << team << endl;;
					return false;
				}
			}
		}
	}

	else { //3 oder mehr voneinander entfernt
		if (round1 < 3 && round2 > u_number_rounds - 4) {  //edge case beide seiten
			counter = 1;
			if (round1 == 0) {
				tmp = Get_Match(team, round2).second;
			}
			else {
				tmp = Get_Match(team, 0).second;
			}
			for (int i = 1; i < round1 + 4; i++) {
				if (i == round1) {
					currentround = Get_Match(team, round2).second;
				}
				else {
					currentround = Get_Match(team, i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " <<  i << " bei Team " << team << endl;
					return false;

				}
			}
			counter = 1;
			tmp = Get_Match(team, round2 - 3).second;
			for (int i = 1; i < (u_number_rounds - round2 + 3); i++) {
				if (i == 3) {
					currentround = Get_Match(team, round1).second;
				}
				else {
					currentround = Get_Match(team, round2 - 3 + i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " << round2 - 2 + i << " bei Team " << team << endl;;
					return false;
				}
			}
		}

		else if (round1 < 3 && round2 < u_number_rounds - 3) { //edge case links
			counter = 1;
			if (round1 == 0) {
				tmp = Get_Match(team, round2).second;
			}
			else {
				tmp = Get_Match(team, 0).second;
			}
			for (int i = 1; i < round1 + 4; i++) {
				if (i == round1) {
					currentround = Get_Match(team, round2).second;
				}
				else {
					currentround = Get_Match(team, i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " <<  i << " bei Team " << team << endl;
					return false;

				}
			}
			counter = 1;
			tmp = Get_Match(team, round2 - 3).second;
			for (int i = 1; i < 7; i++) {
				if (i == 3) {
					currentround = Get_Match(team, round1).second;
				}
				else {
					currentround = Get_Match(team, round2 - 3 + i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " << round2 - 2 + i << " bei Team " << team << endl;;
					return false;
				}
			}

		}
		else if (round1 > 2 && round2 > u_number_rounds - 4) { //edge case rechts
			counter = 1;
			tmp = Get_Match(team, round1 - 3).second;
			for (int i = 1; i < 7; i++) {
				if (i == 3) {
					currentround = Get_Match(team, round2).second;
				}
				else {
					currentround = Get_Match(team, round1 - 3 + i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " << round1 - 2 + i << " bei Team " << team << endl;
					return false;

				}
			}
			counter = 1;
			tmp = Get_Match(team, round2 - 3).second;
			for (int i = 1; i < (u_number_rounds - round2 + 3); i++) {
				if (i == 3) {
					currentround = Get_Match(team, round1).second;
				}
				else {
					currentround = Get_Match(team, round2 - 3 + i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " << round2 - 2 + i << " bei Team " << team << endl;;
					return false;
				}
			}
		}

		else { //kein edge case
			counter = 1;
			tmp = Get_Match(team, round1 - 3).second;
			for (int i = 1; i < 7; i++) {
				//cout << "Runde: " << round1 - 2 +i << " Counter: " << counter << " Tmp (letzte Runde): " << tmp << endl;
				if (i == 3) {
					currentround = Get_Match(team, round2).second;
					//cout << "Current Round home oder away (Runde in der geswapped wird): " << currentround << endl;
				}
				else {
					currentround = Get_Match(team, round1 - 3 + i).second;
					//cout << "Current Round home oder away (Runde vor oder nach geswappter Runde): " << currentround << endl;
				}
				if (tmp == currentround) {
					counter++;
					//cout << "selbes Home away. Neuer Counter: " << counter << endl;
				}
				else {
					counter = 1;
					tmp = currentround;
					//cout << "Anderes Home away. Neuer Counter: " << counter << " neues tmp: " << tmp << endl;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " << round1 - 2 + i << " bei Team " << team << endl;
					return false;

				}
			}
			counter = 1;
			tmp = Get_Match(team, round2 - 3).second;
			for (int i = 1; i < 7; i++) {
				if (i == 3) {
					currentround = Get_Match(team, round1).second;
				}
				else {
					currentround = Get_Match(team, round2 - 3 + i).second;
				}
				if (tmp == currentround) {
					counter++;
				}
				else {
					counter = 1;
					tmp = currentround;
				}
				if (counter > 3) {
					//cout << "Fehler in Runde " << round2 - 2 + i << " bei Team " << team;
					return false;
				}
			}
		}
	}

	return true;
}

//norepeater check:
bool heuristic::norepeater(unsigned team, unsigned round1, unsigned round2) {
	if (Get_Match(team, round1).first == Get_Match(team, round2).first) {
		//cout << "No Repeater Verletzung bei Team " << team << " in den Runden " << round1 << " und " << round2 << endl;
		return false;
	}
	return true;
}


//aufgaben:


pair<int, int> heuristic::TestSwapHA(unsigned team_i, unsigned team_j)
{
	int delta_cost = 0; // gain
	int num_vio = 0; // At first only 0 for feasible and 1 for infeasible
					 
	//welche runden sind betroffen?:

	unsigned round_k = Which_rounds(team_i, team_j).first;
	unsigned round_l = Which_rounds(team_i, team_j).second;

	//Delta costs:

	//distanz_vorher = distanz_team_i(runde k-1 -> k -> k+1) + distanz_team_i(runde l-1 -> l -> l+1) + distanz_team_j(runde k-1 -> k -> k+1) + distanz_team_j(runde l-1 -> l -> l+1)
	int distance_before = 0;
	
	distance_before += distance_vorher(team_i, round_k);
	distance_before += distance_vorher(team_j, round_k);
	distance_before += distance_vorher(team_i, round_k + 1);
	distance_before += distance_vorher(team_j, round_k + 1);
	distance_before += distance_vorher(team_i, round_l);
	distance_before += distance_vorher(team_j, round_l);
	distance_before += distance_vorher(team_i, round_l + 1);
	distance_before += distance_vorher(team_j, round_l + 1);
	//cout << "distance before: " << distance_before << endl;

	//distanz_nachher = distanz_team_i(runde k-1 -> k -> k+1) + distanz_team_i(runde l-1 -> l -> l+1) + distanz_team_j(runde k-1 -> k -> k+1) + distanz_team_j(runde l-1 -> l -> l+1)
	int distance_after = 0;

	distance_after += distance_nachher_vorne(team_i, round_k);
	distance_after += distance_nachher_vorne(team_j, round_k);
	distance_after += distance_nachher_hinten(team_i, round_k + 1);
	distance_after += distance_nachher_hinten(team_j, round_k + 1);
	distance_after += distance_nachher_vorne(team_i, round_l);
	distance_after += distance_nachher_vorne(team_j, round_l);
	distance_after += distance_nachher_hinten(team_i, round_l + 1);
	distance_after += distance_nachher_hinten(team_j, round_l + 1);
	//cout << "distance after: " << distance_after << endl;

	//delta_distanz (als verbesserung) = distanz_vorher - distanz_nachher
	delta_cost = distance_before - distance_after;
	//cout << "delta_costs: " << delta_cost << endl;

	//anzahl aufeinanderfolgender home/away <= 3 weiterhin erfuellt vor und nach den 4 getauschten home/away spielen?:
	//wenn nein -> num_vio = 1
	bool test;
	test = swapHA_homeaway_test(team_i, round_k, round_l);
	if (test) {
		test = swapHA_homeaway_test(team_j, round_k, round_l);
	}

	if (!test) {
		num_vio = 1;
	}

	return make_pair(delta_cost, num_vio);
}

void heuristic::SwapHA(unsigned team_i, unsigned team_j)
{
	//welche runden?:
	unsigned hinrunde = Which_rounds(team_i, team_j).first;
	unsigned rueckrunde = Which_rounds(team_i, team_j).second;

	//home und aways neu:
	bool hinrunde_team_i = Get_Match(team_j, hinrunde).second;


	//home away tauschen:
	Set_Match(team_i, hinrunde, team_j, hinrunde_team_i);
	Set_Match(team_i, rueckrunde, team_j, !hinrunde_team_i);
	Set_Match(team_j, hinrunde, team_i, !hinrunde_team_i);
	Set_Match(team_j, rueckrunde, team_i, hinrunde_team_i);

}

pair<int, int> heuristic::TestSwapRds(unsigned round_k, unsigned round_l)
{
	int delta_cost = 0; // gain
	int num_vio = 0; // At first only 0 for feasible and 1 for infeasible

	int distance_before = 0;
	int distance_after = 0;

	//Delta costs:

	//Kosten vorher unterschieden  in 2 Cases: runden sind direkt nacheinander oder nicht:
	if (round_k + 1 == round_l) { //direkt nacheinander
		for (unsigned i = 0; i < u_number_teams; i++) {
			distance_before += distance_vorher(i, round_k);
			//distance_before += distance_vorher(i, round_l);
			distance_before += distance_vorher(i, round_l + 1);

			distance_after += distance_nachher_vorne2(i, round_k, round_l);
			//distance_after += distance_nachher_mitte(i, round_l, round_k);
			distance_after += distance_nachher_hinten2(i, round_l + 1, round_k);
		}
	}
	else { //min eine runde abstand zwischen den runden:
		for (unsigned i = 0; i < u_number_teams; i++) {
			distance_before += distance_vorher(i, round_k);
			distance_before += distance_vorher(i, round_k+1);
			distance_before += distance_vorher(i, round_l);
			distance_before += distance_vorher(i, round_l+1);

			distance_after += distance_nachher_vorne2(i, round_k, round_l);
			distance_after += distance_nachher_hinten2(i, round_k + 1, round_l);
			distance_after += distance_nachher_vorne2(i, round_l, round_k);
			distance_after += distance_nachher_hinten2(i, round_l + 1, round_k);
		}
	}

	//delta_distanz (als verbesserung) = distanz_vorher - distanz_nachher
	delta_cost = distance_before - distance_after;
	//cout << "delta_costs: " << delta_cost << endl;

	//check ob valide:
	bool test = true;

	//home/away check:
	for (int i = 0; i < u_number_teams; i++) {
		test = swapHA_homeaway_test(i, round_k, round_l);
		if (!test) {
			break;
		}
	}

	//Norepeater check:
	//2 cases: runden direkt nacheinander oder min. eine runde abstand:
	if (test) {
		if (round_k + 1 == round_l) {//nacheinander
			if (round_k != 0) {
				for (unsigned i = 0; i < u_number_teams; i++) {
					if (!norepeater(i, round_k - 1, round_l)) {
						test = false;
						break;
					}
				}
			}
			if (round_l != u_number_rounds-1){
				for (unsigned i = 0; i < u_number_teams; i++) {
					if (!norepeater(i, round_k, round_l + 1)) {
						test = false;
						break;
					}
				}
			}
		}
		else {//mit abstand
			if (round_k != 0) {
				for (unsigned i = 0; i < u_number_teams; i++) {
					//cout << "DEBUGGING! i = " << i << " mit Runde_k = " << round_k << " und Runde_l = " << round_l << endl;
					if (!norepeater(i, round_k - 1, round_l)) {
						test = false;
						break;
					}
				}
			}
			for (unsigned i = 0; i < u_number_teams; i++) {
				if (!norepeater(i, round_l, round_k + 1)) {
					test = false;
					break;
				}
				if (!norepeater(i, round_l - 1, round_k)) {
					test = false;
					break;
				}
			}
			if (round_l != u_number_rounds - 1) {
				for (unsigned i = 0; i < u_number_teams; i++) {
					if (!norepeater(i, round_k, round_l + 1)) {
						test = false;
						break;
					}
				}
			}
		}
	}

	if (!test) {
		num_vio = 1;
		//cout << "not valid tho" << endl;
	}

	return make_pair(delta_cost, num_vio);
}

void heuristic::SwapRds(unsigned round_k, unsigned round_l)
{
	//Spalten tauschen von oben nach unten:
	for (unsigned i = 0; i < u_number_teams; i++) {
		pair<unsigned, bool> tmp;
		tmp = Get_Match(i, round_k);
		Set_Match(i, round_k, Get_Match(i, round_l).first, Get_Match(i, round_l).second);
		Set_Match(i, round_l, tmp.first, tmp.second);

	}
}

pair<int, int> heuristic::TestSwapTms(unsigned team_i, unsigned team_j)
{
	int delta_cost = 0; // gain
	int num_vio = 0; // At first only 0 for feasible and 1 for infeasible
					 // TODO

	return make_pair(delta_cost, num_vio);
}

void heuristic::SwapTms(unsigned team_i, unsigned team_j)
{
	for (unsigned i = 0; i < u_number_rounds; i++) {
		pair<unsigned, bool> tmp_i = Get_Match(team_i, i);

		//spielen teams gegeneinander in der runde? wenn nein dann:
		if (tmp_i.first != team_j) {
			pair<unsigned, bool> tmp_j = Get_Match(team_j, i);

			//tauschen der matchups:
			//Team_i:

			Set_Match(team_i, i, tmp_j.first, tmp_j.second);
			Set_Match(tmp_j.first, i, team_i, !tmp_j.second);

			//Team_j:

			Set_Match(team_j, i, tmp_i.first, tmp_i.second);
			Set_Match(tmp_i.first, i, team_j, !tmp_i.second);

		}
	}
}

pair<int, int> heuristic::TestPrtSwapRds(unsigned team_i, unsigned round_k, unsigned round_l)
{
	int delta_cost = 0; // gain
	int num_vio = 0; // At first only 0 for feasible and 1 for infeasible

	int distance_before = 0;
	int distance_after = 0;


	////////////////////////////////////////welche teams sind betroffen:
	//To DO
	unsigned tmp_row = team_i;
	vector <unsigned> used_teams;
	for (unsigned i = 0; i < u_number_teams; i++) {
		used_teams.push_back(tmp_row);
		if (i % 2 == 0) {
			tmp_row = Get_Match(tmp_row, round_l).first;
		}
		else {
			tmp_row = Get_Match(tmp_row, round_k).first;
		}
		if (tmp_row == team_i) {
			break;
		}

	}
	///////////////////////////////////////////////////////////////////////


	//Delta costs:

	//Kosten vorher unterschieden  in 2 Cases: runden sind direkt nacheinander oder nicht:
	if (round_k + 1 == round_l) { //direkt nacheinander
		for (unsigned i : used_teams) { 
			distance_before += distance_vorher(i, round_k);
			//distance_before += distance_vorher(i, round_l);
			distance_before += distance_vorher(i, round_l + 1);

			distance_after += distance_nachher_vorne2(i, round_k, round_l);
			//distance_after += distance_nachher_mitte(i, round_l, round_k);
			distance_after += distance_nachher_hinten2(i, round_l + 1, round_k);
		}
	}
	else { //min eine runde abstand zwischen den runden:
		for (unsigned i : used_teams) { 
			distance_before += distance_vorher(i, round_k);
			distance_before += distance_vorher(i, round_k + 1);
			distance_before += distance_vorher(i, round_l);
			distance_before += distance_vorher(i, round_l + 1);

			distance_after += distance_nachher_vorne2(i, round_k, round_l);
			distance_after += distance_nachher_hinten2(i, round_k + 1, round_l);
			distance_after += distance_nachher_vorne2(i, round_l, round_k);
			distance_after += distance_nachher_hinten2(i, round_l + 1, round_k);
		}
	}

	//delta_distanz (als verbesserung) = distanz_vorher - distanz_nachher
	delta_cost = distance_before - distance_after;
	//cout << "delta_costs: " << delta_cost << endl;

	//check ob valide:
	bool test = true;

	//home/away check:
	for (unsigned i : used_teams) { 
		test = swapHA_homeaway_test(i, round_k, round_l);
		if (!test) {
			break;
		}
	}

	//Norepeater check:
	//2 cases: runden direkt nacheinander oder min. eine runde abstand:
	if (test) {
		if (round_k + 1 == round_l) {//nacheinander
			if (round_k != 0) {
				for (unsigned i : used_teams) { 
					if (!norepeater(i, round_k - 1, round_l)) {
						test = false;
						break;
					}
				}
			}
			if (round_l != u_number_rounds - 1) {
				for (unsigned i : used_teams) {
					if (!norepeater(i, round_k, round_l + 1)) {
						test = false;
						break;
					}
				}
			}
		}
		else {//mit abstand
			if (round_k != 0) {
				for (unsigned i : used_teams) { 
					//cout << "DEBUGGING! i = " << i << " mit Runde_k = " << round_k << " und Runde_l = " << round_l << endl;
					if (!norepeater(i, round_k - 1, round_l)) {
						test = false;
						break;
					}
				}
			}
			for (unsigned i : used_teams) {
				if (!norepeater(i, round_l, round_k + 1)) {
					test = false;
					break;
				}
				if (!norepeater(i, round_l - 1, round_k)) {
					test = false;
					break;
				}
			}
			if (round_l != u_number_rounds - 1) { 
				for (unsigned i : used_teams) {
					if (!norepeater(i, round_k, round_l + 1)) {
						test = false;
						break;
					}
				}
			}
		}
	}

	if (!test) {
		num_vio = 1;
		//cout << "not valid tho" << endl;
	}

	return make_pair(delta_cost, num_vio);
}

void heuristic::PrtSwapRds(unsigned team_i, unsigned round_k, unsigned round_l)
{
	unsigned tmp_row = team_i;
	for (unsigned i = 0; i < u_number_teams; i++) {
		pair<unsigned, bool> tmp = Get_Match(tmp_row, round_k);
		Set_Match(tmp_row, round_k, Get_Match(tmp_row, round_l).first, Get_Match(tmp_row, round_l).second);
		Set_Match(tmp_row, round_l, tmp.first, tmp.second);

		if (i % 2 == 0) {
			tmp_row = Get_Match(tmp_row, round_k).first;
		}
		else {
			tmp_row = Get_Match(tmp_row, round_l).first;
		}
		if (tmp_row == team_i) {
			break;
		}

	}
}

pair<int, int> heuristic::TestPrtSwapTms(unsigned team_i, unsigned team_j, unsigned round_k)
{
	int delta_cost = 0; // gain
	int num_vio = 0; // At first only 0 for feasible and 1 for infeasible
					 // TODO

	return make_pair(delta_cost, num_vio);
}

void heuristic::PrtSwapTms(unsigned team_i, unsigned team_j, unsigned round_k)
{
	// TODO
}

int heuristic::Move_SwapHA(unsigned k)
{
	int gain = 0;
	int better_count = 0;
	pair <unsigned, unsigned> current_best;
	pair <int, int> tmp_forspeed;
	for (unsigned a = 0; a < u_number_teams; a++) {
		for (unsigned b = a + 1; b < u_number_teams; b++) {
			tmp_forspeed = TestSwapHA(a,b);
			if (tmp_forspeed.first >= gain && tmp_forspeed.second == 0) {
				gain = tmp_forspeed.first;
				current_best = make_pair(a, b);
				better_count++;
				if (better_count == k) {
					break;
				}
			}
		}
		if (better_count == k) {
			break;
		}
	}
	

	if (better_count > 0) {
		SwapHA(current_best.first, current_best.second);
		cout << "SwapHA Schritt mit Team " << current_best.first << " und Team " << current_best.second << " durchgefuehrt. Dabei wurde eine Verbesserung von " << gain << " erreicht." << endl;
	}
	else {
		cout << "Lokales Optima erreicht." << endl;
	}

	return gain;
}

int heuristic::Move_SwapRds(unsigned k)
{
	int gain = 0;
	int better_count = 0;
	pair <unsigned, unsigned> current_best;
	pair <int, int> tmp_forspeed;
	for (unsigned a = 0; a < u_number_rounds; a++) {
		for (unsigned b = a + 1; b < u_number_rounds; b++) {
			//cout << "Paar: " << a << ", " << b << endl;
			tmp_forspeed = TestSwapRds(a, b);
			if (tmp_forspeed.first >= gain && tmp_forspeed.second == 0) {
				gain = tmp_forspeed.first;
				current_best = make_pair(a, b);
				better_count++;
				if (better_count == k) {
					break;
				}
			}
		}
		if (better_count == k) {
			break;
		}
	}


	if (better_count > 0) {
		SwapRds(current_best.first, current_best.second);
		cout << "SwapRds Schritt mit Runde " << current_best.first << " und Runde " << current_best.second << " durchgefuehrt. Dabei wurde eine Verbesserung von " << gain << " erreicht." << endl;
	}
	else {
		cout << "Lokales Optima erreicht." << endl;
	}

	return gain;
}

int heuristic::Move_SwapTms(unsigned k)
{
	int gain = 0;
	int better_count = 0;
	pair <unsigned, unsigned> current_best;
	pair <int, int> tmp_forspeed;
	for (unsigned a = 0; a < u_number_teams; a++) {
		for (unsigned b = a + 1; b < u_number_teams; b++) {
			tmp_forspeed = TestSwapTms(a, b);
			if (tmp_forspeed.first >= gain && tmp_forspeed.second == 0) {
				gain = tmp_forspeed.first;
				current_best = make_pair(a, b);
				better_count++;
				if (better_count == k) {
					break;
				}
			}
		}
		if (better_count == k) {
			break;
		}
	}


	if (better_count > 0) {
		SwapTms(current_best.first, current_best.second);
		cout << "SwapTms Schritt mit Team " << current_best.first << " und Team " << current_best.second << " durchgefuehrt. Dabei wurde eine Verbesserung von " << gain << " erreicht." << endl;
	}
	else {
		cout << "Lokales Optima erreicht." << endl;
	}

	return gain;
}

int heuristic::Move_PrtSwapRds(unsigned k)
{
	int gain = 0;
	int better_count = 0;
	pair <unsigned, unsigned> current_best_rounds;
	unsigned current_best_team;
	pair <int, int> tmp_forspeed;
	for (unsigned c = 0; c < u_number_teams; c++) {
		for (unsigned a = 0; a < u_number_rounds; a++) {
			for (unsigned b = a + 1; b < u_number_rounds; b++) {
				//cout << "Paar: " << a << ", " << b << endl;
				tmp_forspeed = TestPrtSwapRds(c, a, b);
				if (tmp_forspeed.first >= gain && tmp_forspeed.second == 0) {
					gain = tmp_forspeed.first;
					current_best_rounds = make_pair(a, b);
					current_best_team = c;
					better_count++;
					if (better_count == k) {
						break;
					}
				}
			}
			if (better_count == k) {
				break;
			}
		}
		if (better_count == k) {
			break;
		}
	}


	if (better_count > 0) {
		PrtSwapRds(current_best_team, current_best_rounds.first, current_best_rounds.second);
		cout << "PrtSwapRds Schritt bei Team " << current_best_team << " in Runde " << current_best_rounds.first << " und Runde " << current_best_rounds.second << " durchgefuehrt. Dabei wurde eine Verbesserung von " << gain << " erreicht." << endl;
	}
	else {
		cout << "Lokales Optima erreicht." << endl;
	}

	return gain;
}

int heuristic::Move_PrtSwapTms(unsigned k)
{
	int gain = 0;
	// TODO

	return gain;
}

unsigned heuristic::Calculate_Distance()
{
	unsigned cost = 0;
	for (unsigned t = 0; t < u_number_teams; t++)
	{
		if (!Get_Match(t, 0).second)
			cost += vv_distance[t][Get_Match(t, 0).first];

		for (unsigned n = 1; n < u_number_rounds; n++)
		{
			if (Get_Match(t, n - 1).second && !Get_Match(t, n).second)
			{
				cost += vv_distance[t][Get_Match(t, n).first];
			}
			else if (!Get_Match(t, n - 1).second && Get_Match(t, n).second)
			{
				cost += vv_distance[Get_Match(t, n - 1).first][t];
			}
			else if (!Get_Match(t, n - 1).second && !Get_Match(t, n).second)
			{
				cost += vv_distance[Get_Match(t, n - 1).first][Get_Match(t, n).first];
			}
		}
		if (!Get_Match(t, u_number_rounds - 1).second)
			cost += vv_distance[Get_Match(t, u_number_rounds - 1).first][t];
	}
	return cost;
}

//method prints the distance-matrix
void heuristic::Print_Distance()
{
	cout << "Matrix of distances:" << endl;
	for (unsigned i = 0; i < u_number_teams; i++)
	{
		for (unsigned j = 0; j < u_number_teams; j++)
			cout << vv_distance[i][j] << "\t";
		cout << endl;
	}
}

//method prints the matrix of consequtive opponents
void heuristic::Print_Consecutives()
{
	cout << "Matrix of consecutive opponents:" << endl;
	for (unsigned i = 0; i < u_number_teams; i++)
	{
		for (unsigned j = 0; j < u_number_teams; j++)
			cout << Get_Num_Cons_Opp(i,j) << "\t";
		cout << endl;
	}
}

//method prints the schedule
void heuristic::Print_Schedule(bool zero_based) 
{
	int number_to_add = 0;
	if (!zero_based)
		number_to_add = 1;
	cout << "schedule:" << endl;
	for (unsigned t = 0; t < u_number_teams; t++) 
	{
		for (unsigned n = 0; n < u_number_rounds; n++)
			cout << (Get_Match(t,n).second?"+":"-") << Get_Match(t,n).first + number_to_add << "\t";
		cout << endl;			
	}	
}
void heuristic::Read_Schedule(string file)
{
	ifstream schedulefile;
	schedulefile.open(file.c_str(), ios::in);

	if (schedulefile.is_open())
	{
		int teams_buffer = -1;
		//test team number
		schedulefile >> teams_buffer;
		if (teams_buffer != u_number_teams)
			cerr << "ERROR! Num Teams in schedule does not fit with instance" << endl;

		//read schedule
		string helper = "-1";
		int opponent = 0;

		bool home = true;
		for (int i = 0; i<(int)u_number_teams; i++)
		{
			for (int j = 0; j<(int)u_number_rounds; j++)
			{
				schedulefile >> helper;

				if (helper == "-0")
				{
					opponent = std::stoi(helper);
					home = false;
				}
				else
				{
					opponent = std::stoi(helper);
					if (opponent >= 0)
						home = true;
					else
					{
						opponent *= -1;
						home = false;
					}
					Set_Match(i, j, opponent, home);
				}
			}
		}
	}

	else
	{
		cout << "Could not read file";
	}
	schedulefile.close();
}
