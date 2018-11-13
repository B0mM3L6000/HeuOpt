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
	for (int n = 1; n < u_number_rounds; n++)
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
	for (int n = 1; n < u_number_rounds; n++)
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
	//initialisieren teamvektor:
	size_t anzahlteams = u_number_teams;
	vector<int> teams(anzahlteams);
	for (int i = 0; i < anzahlteams; ++i) {
		teams[i] = i;
	}
	bool checkvarteami = true;
	bool checkvarteamvsi = true;

	//testoutputs:
	//cout << endl << "Laenge Array (anzahl teams): " << teams.size() << endl;
	//cout << "Team 1: " << teams[0] << endl;
	//cout << "Anzahl Runden(hinrunde): " << u_number_rounds / 2 << endl;

	//schleife für alle hinrunden:

	for (int j = 0; j < u_number_rounds/2; ++j) {

		//cout << "Runde: " << j + 1 << endl;
		//erstellen spielplan für runde i:

		for (int i = 0; i < u_number_teams/2; ++i) {
			//erstellen cases für heim oder auswärts - TO DO
			//erstellen von n1 und n2
			int n1 = 0;
			int n2 = 0;
			int k = 1;
			if (j-k >0){
				checkvarteami = Get_Match(teams[i], j-1).second;
				//cout << "Home/Guest(1/0): " << checkvarteami;
				while (checkvarteami == Get_Match(teams[i],j-k).second){
					//cout << endl << "n1 wird erhoeht!  k= " <<k << " j=" << j << " j-k=" << j-k << endl;
					++n1;
					++k;
					if (j - k < 0) {
						break;
					}
				}
			}
			else if (j-k==0) { n1 = 1; }
			else { n1 = 0; };

			k = 1;
			if (j - k >0) {
				checkvarteamvsi = Get_Match(teams[u_number_teams-i-1], j - 1).second;
				//cout << "Home/Guest(1/0): " << checkvarteamvsi;
				while (checkvarteamvsi == Get_Match(teams[u_number_teams - i - 1], j - k).second) {
					//cout << endl << "n1 wird erhoeht!  k= " <<k << " j=" << j << " j-k=" << j-k << endl;
					++n2;
					++k;
					if (j - k < 0) {
						break;
					}
				}
			}
			else if (j - k == 0) { n2 = 1; }
			else { n2 = 0; };
			//cout << "team " << teams[i] + 1 << ":  " <<  "n1: " << n1 << endl;
			//cout << "team " << teams[u_number_teams - i - 1] + 1 << ":  " << "n2: " << n2 << endl;
			//n1 = 0;

			//entscheiden nach cases anhand n1 und n2:

			if (n2 > n1) {
				if (checkvarteamvsi == true) {
					//team2 auswaerts
					//cout << "match setzen";
					Set_Match(teams[i], j, teams[u_number_teams - i - 1], true);
					Set_Match(teams[u_number_teams - i - 1], j, teams[i], false);
					Set_Match(teams[i], j + u_number_rounds / 2, teams[u_number_teams - i - 1], false);
					Set_Match(teams[u_number_teams - i - 1], j + u_number_rounds / 2, teams[i], true);
				}
				else {
					//team2 daheim
					//cout << "match setzen";
					Set_Match(teams[i], j, teams[u_number_teams - i - 1], false);
					Set_Match(teams[u_number_teams - i - 1], j, teams[i], true);
					Set_Match(teams[i], j + u_number_rounds / 2, teams[u_number_teams - i - 1], true);
					Set_Match(teams[u_number_teams - i - 1], j + u_number_rounds / 2, teams[i], false);
				}
			}
			else if (n1 > n2) {
				if (checkvarteami == true) {
					//team1 auswaerts
					//cout << "match setzen";
					Set_Match(teams[i], j, teams[u_number_teams - i - 1], false);
					Set_Match(teams[u_number_teams - i - 1], j, teams[i], true);
					Set_Match(teams[i], j + u_number_rounds / 2, teams[u_number_teams - i - 1], true);
					Set_Match(teams[u_number_teams - i - 1], j + u_number_rounds / 2, teams[i], false);
				}
				else {
					//team1 daheim
					//cout << "match setzen";
					Set_Match(teams[i], j, teams[u_number_teams - i - 1], true);
					Set_Match(teams[u_number_teams - i - 1], j, teams[i], false);
					Set_Match(teams[i], j + u_number_rounds / 2, teams[u_number_teams - i - 1], false);
					Set_Match(teams[u_number_teams - i - 1], j + u_number_rounds / 2, teams[i], true);
				}
			}
			else if (n1 == n2) {
				if (checkvarteami == checkvarteamvsi) {
					//random
					int rndnumber;
					rndnumber = (rand() % 2 + 1);
					if (rndnumber == 1) {
						//team 2 daheim
						//cout << "match setzen";
						Set_Match(teams[i], j, teams[u_number_teams - i - 1], false);
						Set_Match(teams[u_number_teams - i - 1], j, teams[i], true);
						Set_Match(teams[i], j + u_number_rounds / 2, teams[u_number_teams - i - 1], true);
						Set_Match(teams[u_number_teams - i - 1], j + u_number_rounds / 2, teams[i], false);
					}
					else {
						//team 1 daheim
						//cout << "match setzen";
						Set_Match(teams[i], j, teams[u_number_teams - i - 1], true);
						Set_Match(teams[u_number_teams - i - 1], j, teams[i], false);
						Set_Match(teams[i], j + u_number_rounds / 2, teams[u_number_teams - i - 1], false);
						Set_Match(teams[u_number_teams - i - 1], j + u_number_rounds / 2, teams[i], true);
					}
				}
				else if (checkvarteami == true) {
					//team 2 daheim
					//cout << "match setzen";
					Set_Match(teams[i], j, teams[u_number_teams - i - 1], false);
					Set_Match(teams[u_number_teams - i - 1], j, teams[i], true);
					Set_Match(teams[i], j + u_number_rounds / 2, teams[u_number_teams - i - 1], true);
					Set_Match(teams[u_number_teams - i - 1], j + u_number_rounds / 2, teams[i], false);
				}
				else if (checkvarteami == false) {
					//team 1 daheim
					//cout << "match setzen";
					Set_Match(teams[i], j, teams[u_number_teams - i - 1], true);
					Set_Match(teams[u_number_teams - i - 1], j, teams[i], false);
					Set_Match(teams[i], j + u_number_rounds / 2, teams[u_number_teams - i - 1], false);
					Set_Match(teams[u_number_teams - i - 1], j + u_number_rounds / 2, teams[i], true);
				}
			}

			//setzen in spielplan:
			//Set_Match(teams[i], j, teams[u_number_teams - i - 1], true);
			//Set_Match(teams[u_number_teams - i - 1], j, teams[i], false);
			//Set_Match(teams[i], j + u_number_rounds/2, teams[u_number_teams - i - 1], false);
			//Set_Match(teams[u_number_teams - i - 1], j + u_number_rounds/2, teams[i], true);
		}



		//cout << endl;
		//neuanordnen des teamvektors:
		int tmp = teams[u_number_teams - 2];
		//cout << "Temp variable index: " << tmp << endl;
		for (int i = 2; i < anzahlteams; ++i) {
			teams[u_number_teams - i] = teams[u_number_teams - i - 1];
			//cout << i << endl;
		}
		teams[0] = tmp;

		//testoutputs
		//for (auto i : teams)
		//	cout << i << ' ';
		//cout << endl;


	}

	
}
//method creates a starting solution
void heuristic::Advanced_Startingsolution()
{
	// TODO 
}


bool heuristic::Check_Constraints()
{
	// TODO
	cout << "Error: Feasibility check for the schedule not implemented";
	throw;

	return false;
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
				cost += vv_distance[Get_Match(t, n).first][t];
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
