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

//-------------------------------------------------------------------------------------------------------------

//easy method creates a starting solution

void heuristic::Easy_Startingsolution()
{
	timer.Start();

	//initialisieren teamvektor:
	size_t anzahlteams = u_number_teams;
	vector<int> teams(anzahlteams);
	for (int i = 0; i < anzahlteams; ++i) {
		teams[i] = i;
	}

	//initialisiere bool varibalen für später
	bool checkvarteami = true;  //team1
	bool checkvarteamvsi = true;  //team2

	//schleife für alle hinrunden (anzahl runden/2):

	for (int j = 0; j < u_number_rounds/2; ++j) {

		//erstellen spielplan für runde i:

		for (int i = 0; i < u_number_teams/2; ++i) {
			//erstellen cases für heim oder auswärts
			//erstellen von n1 und n2:
			//Initialisieren:
			int n1 = 0;
			int n2 = 0;
			int k = 1;

			//n1:
			if (j-k >0){    //runde 3 und mehr
				checkvarteami = Get_Match(teams[i], j-1).second;
				while (checkvarteami == Get_Match(teams[i],j-k).second){
					++n1;
					++k;
					if (j - k < 0) {
						break;
					}
				}
			}
			else if (j-k==0) { n1 = 1; }  //runde 2
			else { n1 = 0; };   //runde 1


			//n2:
			k = 1;
			if (j - k >0) {    //runde 3 und mehr
				checkvarteamvsi = Get_Match(teams[u_number_teams-i-1], j - 1).second;
				while (checkvarteamvsi == Get_Match(teams[u_number_teams - i - 1], j - k).second) {
					++n2;
					++k;
					if (j - k < 0) {
						break;
					}
				}
			}
			else if (j - k == 0) { n2 = 1; }   //runde 2
			else { n2 = 0; };  //runde 1


			//entscheiden nach cases anhand n1 und n2 und dann entsprechendes setzten der paarung für hin- und rueckrunde:

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


		//neuanordnen des teamvektors (drehung polygon):
		int tmp = teams[u_number_teams - 2];
		for (int i = 2; i < anzahlteams; ++i) {
			teams[u_number_teams - i] = teams[u_number_teams - i - 1];
		}
		teams[0] = tmp;


	}


	//Auswertung:

	timer.Stop();

	//SolutionFile erstellen
	bool neueDatei = false;
	string filename = "easy_solution.csv";
	ifstream temp(filename);
	if (!temp) neueDatei = true;
	else temp.close();

	ofstream stream(filename, ios::app);
	locale myloc("");
	stream.imbue(myloc);

	if (neueDatei)
		stream << "Instanzname;Anzahl Teams;Dist;Time;" << endl; // ';'= column separator
																							   //Name
	stream << s_instance_name << ";";
	//Num Teams
	stream << u_number_teams << ";";
	//Dist
	stream << Calculate_Distance() << ";";
	//Time
	stream << timer.Seconds() << ";";
	stream << endl;

}


//-------------------------------------------------------------------------------------------------------------

//advanced method creates a starting solution

// -> (hilfsfunktion für vektor sortieren(absteigend erste spalte):)
bool sortcol(const vector<int>& v1, const vector<int>& v2) {
	return v1[0] > v2[0];
}
// -> (hilfsfunktion für vektor sortieren(aufsteigend erste spalte):)
bool sortcolasc(const vector<int>& v1, const vector<int>& v2) {
	return v1[0] < v2[0];
}
// -> (hilfsfunktion für vektor sortieren(aufsteigend zweite spalte):)
bool sortcolasc2(const vector<int>& v1, const vector<int>& v2) {
	return v1[1] < v2[1];
}


void heuristic::Advanced_Startingsolution()
{
	timer.Start();

	//Berechne und speichere ConsecOpps für alle abstrakten Teams im Dummyspielplan (aus easy_solution):
	
	int counter;

	for (int i = 0; i < u_number_teams; ++i) {
		for (int j = 1+i; j < u_number_teams; ++j) {
			counter = 0;
			for (int n = 1; n < u_number_rounds; ++n)
			{
				if (Get_Match(i, n - 1).first == Get_Match(j, n).first)
					counter++;
				if (Get_Match(i, n).first == Get_Match(j, n - 1).first)
					counter++;
			}
			Set_Num_Cons_Opp(i, j, counter);
			Set_Num_Cons_Opp(j, i, counter);
		}
	}



	//Erstelle absteigend sortierte Liste der consecopps:

	// -> Liste erstellen:
	vector<vector<int>> c_o_vector; //Vektor in dem die consecopp paare stehen
	vector<int> pairwithconsec;   // Anzahl consecopps , team1, team2

	for (int i = 0; i < u_number_teams; ++i) {
		for (int j = 1 + i; j < u_number_teams; ++j) {
			//neuer Eintrag:
			pairwithconsec.push_back(Get_Num_Cons_Opp(i,j));
			pairwithconsec.push_back(i);
			pairwithconsec.push_back(j);
			c_o_vector.push_back(pairwithconsec);
			//pairwithconsec zurücksetzen:
			while (!pairwithconsec.empty()) {
				pairwithconsec.pop_back();
			}

		}
	}

	// -> Sortieren

	////////vektor anzeigen (debugging:)
	/*
	int m = c_o_vector.size();
	int n = c_o_vector[0].size();
	cout << "The consec before sorting is:\n";
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			cout << c_o_vector[i][j] << " ";
		cout << endl;
	}
	*/

	sort(c_o_vector.begin(), c_o_vector.end(), sortcol);  //sortiert absteigend anhand erster spalte

	////////vektor anzeigen (debugging:)
	/*
	cout << "The consec after sorting is:\n";
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			cout << c_o_vector[i][j] << " ";
		cout << endl;
	}
	*/



	//Erstelle aufsteigend sortierte Liste der Teampaarung nach Distanz der realen Teams (aus Distanzmatrix):

	// -> Liste erstellen:
	vector<vector<int>> distance_vector; //Vektor in dem die Distanz Teampaare stehen
	vector<int> pairwithdistance;   // distance , team1, team2

	for (int i = 0; i < u_number_teams; ++i) {
		for (int j = 1 + i; j < u_number_teams; ++j) {
			//neuer Eintrag:
			pairwithdistance.push_back(Get_Distance(i, j));
			pairwithdistance.push_back(i);
			pairwithdistance.push_back(j);
			distance_vector.push_back(pairwithdistance);
			//pairwithdistance zurücksetzen:
			while (!pairwithdistance.empty()) {
				pairwithdistance.pop_back();
			}

		}
	}

	// -> Sortieren:

	sort(distance_vector.begin(), distance_vector.end(), sortcolasc);  //sortiert aufsteigend anhand erster spalte



	//Ordne reale Teams abstrakten Teams zu:

	vector<vector<int>> team_matched;   //abstraktes team, reales team
	int tmpteam1;  //reales team
	int tmpteam2;  //reales team
	int absteam1;  //abstraktes team
	int absteam2;  //abstraktes team
	int tmpabsteam1;
	int tmpabsteam2;
	bool allreadymatched1;  //reales team
	bool allreadymatched2;  //reales team
	bool absallreadymatched1;  //abstraktes team
	bool absallreadymatched2;  //abstraktes team
	bool usedabsteam;

	for (int j = 0; j < distance_vector.size(); j++) {
		
		//wenn alle teams zugeordnet, dann beende die Zuordnung 
		if (team_matched.size() == u_number_teams) {
			break;
		}

		tmpteam1 = distance_vector[j][1]; //reales team 1 aus paarungseintrag
		tmpteam2 = distance_vector[j][2]; //reales team 2 aus paarungseintrag
		
		//ist reales team bereits zugeordnet? team1:
		allreadymatched1 = false;
		for (int i = 0; i < team_matched.size(); ++i) {
			if (tmpteam1 == team_matched[i][1]) {
				allreadymatched1 = true;
				tmpabsteam1 = team_matched[i][0];
				break;
			}
		}

		//ist reales team bereits zugeordnet? team2:
		allreadymatched2 = false;
		for (int i = 0; i < team_matched.size(); ++i) {
			if (tmpteam2 == team_matched[i][1]) {
				allreadymatched2 = true;
				tmpabsteam2 = team_matched[i][0];
				break;
			}
		}

		//falls beide noch nicht zugeordnet sind
		if (allreadymatched1 == false && allreadymatched2 == false) {
			for (int k = 0; k < c_o_vector.size(); ++k) {
				absteam1 = c_o_vector[k][1];
				absteam2 = c_o_vector[k][2];

				//ist abstraktes team bereits zugeordnet? team1:
				absallreadymatched1 = false;
				for (int i = 0; i < team_matched.size(); ++i) {
					if (absteam1 == team_matched[i][0]) {
						absallreadymatched1 = true;
						//cout << "erste team zugeordnet" << endl;
						break;
					}
				}

				//ist abstraktes team bereits zugeordnet? team2:
				absallreadymatched2 = false;
				for (int i = 0; i < team_matched.size(); ++i) {
					if (absteam2 == team_matched[i][0]) {
						absallreadymatched2 = true;
						//cout << "zweite team bereits zugeordnet" << endl;
						break;
					}
				}

				//wenn beide abstrakten teams ebenfalls frei sind, dann zuordnen zu den beiden realen teams und in vektor das paar eintragen. 
				//anschließend innere schleife abbrechen und zum nächsten distanz paar realer teams gehen
				if (absallreadymatched2 == false && absallreadymatched1 == false) {
					team_matched.push_back({ absteam1, tmpteam1 });
					team_matched.push_back({ absteam2, tmpteam2 });
					//cout << absallreadymatched1 << absallreadymatched2 << endl;
					//cout << "Matching absteam " << absteam1 << " mit real team " << tmpteam1 << "in doppelmatching." << endl;
					//cout << "dazu noch absteam " << absteam2 << " mit real team " << tmpteam2 << endl;
					break;
				}


			}
		}


		//falls NUR team1 (tmpteam1) noch nicht zugeordnet ist:

		if (allreadymatched1 == false && allreadymatched2 == true) {
			for (int k = 0; k < c_o_vector.size(); ++k) {
				absteam1 = c_o_vector[k][1];
				absteam2 = c_o_vector[k][2];
				
				
				if (absteam1 == tmpabsteam2) { //ist absteam1 das bereits zugeordnete?
					usedabsteam = false;
					//Ist das andere abstrakte team noch frei?
					for (int m = 0; m < team_matched.size(); ++m) {
						if (absteam2 == team_matched[m][0]) {
							usedabsteam = true;
							break;
						}
					}

					//falls das team noch frei ist, dann zuordnen:
					if(usedabsteam == false){
						team_matched.push_back({ absteam2, tmpteam1 });
						//cout << "matching team in erster einser schleife" << absteam2 << " mit real " << tmpteam1 << endl;
						break;
					}
				}
				else if (absteam2 == tmpabsteam2) { //ist absteam2 das bereits zugeordnete?
					usedabsteam = false;
					//Ist das andere abstrakte team noch frei?
					for (int m = 0; m < team_matched.size(); ++m) {
						if (absteam1 == team_matched[m][0]) {
							usedabsteam = true;
							break;
						}
					}

					//falls das team noch frei ist, dann zuordnen:
					if (usedabsteam == false) {
						team_matched.push_back({ absteam1, tmpteam1 });
						//cout << "matching team in erster zweier schleife" << absteam1 << " mit real " << tmpteam1 << endl;
						break;
					}
				}
			}
		}

		//falls NUR team2 (tmpteam2) noch nicht zugeordnet ist:

		if (allreadymatched1 == true && allreadymatched2 == false) {
			for (int k = 0; k < c_o_vector.size(); ++k) {
				absteam1 = c_o_vector[k][1];
				absteam2 = c_o_vector[k][2];


				if (absteam1 == tmpabsteam1) { //ist absteam1 das bereits zugeordnete?
					usedabsteam = false;
					//Ist das andere abstrakte team noch frei?
					for (int m = 0; m < team_matched.size(); ++m) {
						if (absteam2 == team_matched[m][0]) {
							usedabsteam = true;
							break;
						}
					}

					//falls das team noch frei ist, dann zuordnen:
					if (usedabsteam == false) {
						team_matched.push_back({ absteam2, tmpteam2 });
						//cout << "matching team in zweiter einser schleife" << absteam2 << " mit real " << tmpteam2 << endl;
						break;
					}
				}
				else if (absteam2 == tmpabsteam1) { //ist absteam2 das bereits zugeordnete?
					usedabsteam = false;
					//Ist das andere abstrakte team noch frei?
					for (int m = 0; m < team_matched.size(); ++m) {
						if (absteam1 == team_matched[m][0]) {
							usedabsteam = true;
							break;
						}
					}

					//falls das team noch frei ist, dann zuordnen:
					if (usedabsteam == false) {
						team_matched.push_back({ absteam1, tmpteam2 });
						//cout << "matching team in zweiter zweier schleife" << absteam1 << " mit real " << tmpteam2 << endl;
						break;
					}
				}
			}
		}

		//falls beide teams schon zugeordnet sind tue nichts und weiter zum nächsten eintrag


	}


	sort(team_matched.begin(), team_matched.end(), sortcolasc2);  //sortiert aufsteigend anhand zweiter spalte (reales team)

	//DEBUGGING: gebe vektor aus mit den zuordnungen:
	cout << endl << "ZUGEORDNETE TEAMS (ABSTRAKT/REAL):" << endl;
	for (int i = 0; i < team_matched.size(); ++i) {
		cout << team_matched[i][0] << " - " << team_matched[i][1] << endl;
	}



	//erstellen temporärer Spielplan anhand des alten spielplans und der zuordnung:
	
	vector < vector < pair <int, bool> > > tmpspielplan;
	int abstractteam;
	int tmpabstractteam;
	int tmprealteam;
	bool tmpabshomeguest;

	for (int i = 0; i < u_number_teams; ++i) { //schleife pro reihe , also pro reales team
		abstractteam = team_matched[i][0];
		for (int j = 0; j < u_number_rounds; ++j) { //schleife pro spalte, also pro spielplanrunde für dieses reale team
			tmpabstractteam = Get_Match(abstractteam, j).first;
			tmpabshomeguest = Get_Match(abstractteam, j).second;
			
			for (int k = 0; k < team_matched.size(); ++k) {
				if (team_matched[k][0] == tmpabstractteam) {
					tmprealteam = team_matched[k][1];
					break;
				}
			}
			tmpspielplan.push_back({});
			tmpspielplan[i].push_back({ tmprealteam,tmpabshomeguest });
		}
	}
	


	
	//Erstelle neuen Spielplan anhand abstrakten Spielplan und Zuordnung:
	
	for (int i = 0; i < u_number_teams; ++i) { //teams
		for (int j = 0; j < u_number_rounds; ++j) { //runden für dieses team
			Set_Match(i, j, tmpspielplan[i][j].first, tmpspielplan[i][j].second);
		}
	}
	



	//Auswertung:

	timer.Stop();

	//SolutionFile erstellen
	bool neueDatei = false;
	string filename = "advanced_solution.csv";
	ifstream temp(filename);
	if (!temp) neueDatei = true;
	else temp.close();

	ofstream stream(filename, ios::app);
	locale myloc("");
	stream.imbue(myloc);

	if (neueDatei)
		stream << "Instanzname;Anzahl Teams;Dist;Time;" << endl; // ';'= column separator
																							   //Name
	stream << s_instance_name << ";";
	//Num Teams
	stream << u_number_teams << ";";
	//Dist
	stream << Calculate_Distance() << ";";
	//Time
	stream << timer.Seconds() << ";";
	stream << endl;

}

//-------------------------------------------------------------------------------------------------------------

//method that checks if solution is valid

bool heuristic::Check_Constraints()
{
	//Initialisieren:
	vector<pair<int,bool>> reihe;
	int check;
	bool location;
	pair<int, bool> paarung;

	//Checken ob Reihen valide (jede zahl nur einmal pro Reihe):
	for (int j = 0; j < u_number_teams; ++j) {  //team j (reihe)
		for (int i = 0; i < u_number_rounds; ++i) { //runde i (spalte)

			//eintrag in reihe und spalte i,j auslesen
			location = Get_Match(j, i).second;
			check = Get_Match(j, i).first;
			paarung = { check, location };

			//testen ob bereits in vektor vorhanden
			if (find(reihe.begin(), reihe.end(), paarung) != reihe.end()) {
				//falls ja keine valide loesung -> fehler
				cout << "Invalid solution: Error in row " << j << endl;
				return false;
			}
			else {
				//falls nein ist die loesung weiterhin valide
				//element an vektor anfügen:
				reihe.push_back(paarung);
			}


		}
		//vektor zurücksetzten für die nächste reihe:
		while (!reihe.empty()) {
			reihe.pop_back();
		}


	}

	//Checken ob Spalten Valide:
	//Jede Zahl nur einmal pro Spalte
	
	for (int i = 0; i < u_number_rounds; ++i) {   //runde i (spalte)
		for (int j = 0; j < u_number_teams; ++j) {   //team j (reihe)

			//eintrag in reihe und spalte i,j auslesen
			location = Get_Match(j, i).second;
			check = Get_Match(j, i).first;
			paarung = { check, location };

			//testen ob bereits in vektor vorhanden
			if (find(reihe.begin(), reihe.end(), paarung) != reihe.end()) {
				//falls ja keine valide loesung -> fehler
				cout << "Invalid solution: Error in column " << i << endl;
				return false;
			}
			else {
				//falls nein ist die loesung weiterhin valide
				//element an vektor anfügen:
				reihe.push_back(paarung);
			}
		}
		//vektor zurücksetzten für die nächste reihe:
			
		while (!reihe.empty()) {
			reihe.pop_back();
		}
		
	}
	
	//Paarungen passen?

	for (int i = 0; i < u_number_rounds; ++i) {   //runde i (spalte)
		for (int j = 0; j < u_number_teams; ++j) {   //team j (reihe)
			if (j != Get_Match(Get_Match(j, i).first, i).first) {   //spielen die selben teams gegeneinander?
				cout << "Invalid solution: Wrong oppenent in round " << i << " for team " << j << endl;
				return false;
			}
			else if (Get_Match(j, i).second == Get_Match(Get_Match(j, i).first, i).second) {     //ein team home ein team guest?
				cout << "Invalid solution: Home/Guest pairing error in round " << i << " for team " << j << endl;
				return false;
			}

		}
	}

	//wenn alle tests keinen fehler werfen ist es eine valide lösung:
	cout << "Valid solution given." << endl;
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
