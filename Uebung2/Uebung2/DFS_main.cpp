#include "heuristic.h"
#include <fstream>
using namespace std;

void CreateStartSolution( string filename, string output, int k ) 
{
	// start timer
	c_TimeInfo timer;

	//create an object of instance heuristic for the calulation of the upper bounds
	heuristic heureka(filename);

	// Erstelle eine Startlösung
	timer.Restart();
	heureka.Easy_Startingsolution();
	cout << "Die berechnete Startloesung kostet: " << heureka.Calculate_Distance() << endl;
	timer.Stop();

	int tmpdistance = heureka.Calculate_Distance();

	cout << "Das Erstellen der Startloesung hat " << timer.FormattedTime() << " Sekunden gedauert." << endl;

	heureka.Check_Constraints();

	heureka.Print_Schedule(true);

	// Hier kommt die Lokale Suche hin
	// TODO

	//int k = 100;
	timer.Restart();
	//heureka.Move_PrtSwapRds(k); //usw.
	//heureka.Move_SwapHA(k);
	//heureka.Move_SwapRds(k);
	//heureka.Move_PrtSwapTms(k);
	//heureka.Move_SwapTms(k);
	//heureka.VND(k);
	heureka.VNS(k, 100);
	timer.Stop();




	
	//SolutionFile erstellen
	bool neueDatei = false;
	string filename2 = "solution_uebung3_tests.csv";
	ifstream temp(filename2);
	if (!temp) neueDatei = true;
	else temp.close();

	ofstream stream(filename2, ios::app);
	locale myloc("");
	stream.imbue(myloc);

	if (neueDatei)
		stream << "Instanz; Metaheuristik; k; Kosten vorher; kosten nachher; Verstrichene Zeit;" << endl; // ';'= column separator
																							   //Name
	//instanz
	stream << filename << ";";
	//swapart (per hand anpassen)
	stream << "VNS" << ";";
	//k
	stream << k << ";";
	//Distanz vorher
	stream << tmpdistance << ";";
	//Distanz nacher
	stream << heureka.Calculate_Distance() << ";";
	//Time
	stream << timer.Seconds() << ";";
	stream << endl;

	


	heureka.Print_Schedule(true);
	cout << "Heuristische Suche hat " << timer.FormattedTime() << " Sekunden gedauert." << endl;
	cout << "Die berechnete Loesung kostet: " << heureka.Calculate_Distance() << endl;
	heureka.Check_Constraints();
	system("pause");   //auskommentieren für datenauswertung am ende

}

void main()
{
	const char* circ_files[] = {
		"instances/circ4",
		"instances/circ6",
		"instances/circ8",
		"instances/circ10",
		"instances/circ12",
		"instances/circ14",
		"instances/circ16",
		"instances/circ18",
		"instances/circ20" };

	const char** filenames;
	int instance_idx1 = -1;
	int instance_idx2 = -1;
	int max_inst = -1;
	int selected_set = -1;
	
	//default value filenames
	const char* default_files[] =
	{
		"default"
	};

	filenames = default_files;


	while (selected_set < 0 || selected_set > 3)
	{
		cout << "Which benchmark set?" << endl;
		cout << " 0 = Circular" << endl;
		cin >> selected_set;
	}
	switch (selected_set)
	{
	case 0: // GDB
		max_inst = 9;
		filenames = circ_files;
		break;
	}

	if (filenames == default_files)
	{
		std::cout << "Error: Filename set to Default";
		throw;
	}
	while (instance_idx1 < 0 || instance_idx1 >= max_inst || instance_idx2 < 0 || instance_idx2 >= max_inst)
	{
		cout << "Choose instance:" << endl;
		for (int i = 0; i < max_inst; i++)
			cout << " " << i << " = " << filenames[i] << endl;
		cout << " " << max_inst << " = all" << endl;

		cout << "from ";
		cin >> instance_idx1;
		if ( instance_idx1 == max_inst )
		{
			instance_idx1 = 0; //
			instance_idx2 = max_inst-1;
		}
		else 
		{
			cout << "to ";
			cin >> instance_idx2;
		}
	}

	/*   hier wenn k als input sein soll
	//Input k fuer lokale Suche:
	cout << "Welches k soll fuer die k-Besten-Suche verwendet werden? Bitte eingeben: ";
	int k = -1;
	cin >> k;
	while (k < 1) {
		cout << "Ungueltige Eingabe (k < 1)! Bitte wiederholen: ";
		cin >> k;
	}
	cout << "Auswahl: K = " << k << endl;
	*/
	int k = 5;   //k hier einstellen
	int durchlaufe = 1;  //wieviele durchläufe pro instanz

	for (int idx = instance_idx1; idx <= instance_idx2; idx++)
	{
		// Eingabe
		string filename = string(filenames[idx]) + ".txt";

		// Ausgabe
		string output = string(filenames[idx]) + "_solution.txt";

		// Konstruiere eine Startlösung und führe metaheuristik durch
		for (int i = 0; i < durchlaufe; i++) {
			CreateStartSolution(filename, output, k);
			cout << "Iteration: " << i << endl;
		}
	}
}

