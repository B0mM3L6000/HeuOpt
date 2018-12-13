#include "heuristic.h"

using namespace std;

void CreateStartSolution( string filename, string output ) 
{
	// start timer
	c_TimeInfo timer;

	//create an object of instance heuristic for the calulation of the upper bounds
	heuristic heureka(filename);

	// Erstelle eine Startl�sung
	timer.Restart();
	heureka.Easy_Startingsolution();
	cout << "Die berechnete Startloesung kostet: " << heureka.Calculate_Distance() << endl;
	timer.Stop();

	cout << "Das Erstellen der Startloesung hat " << timer.FormattedTime() << " Sekunden gedauert." << endl;


	// Hier kommt die Lokale Suche hin
	// TODO
	int k = 100;
	timer.Restart();
	heureka.Move_SwapHA(k); //usw.
	timer.Stop();

	heureka.Print_Schedule(true);
	cout << "Heuristische Suche hat " << timer.FormattedTime() << " Sekunden gedauert." << endl;
	cout << "Die berechnete Loesung kostet: " << heureka.Calculate_Distance() << endl;
	system("pause");

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
		"instances/circ16" };

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
		max_inst = 6;
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

	for ( int idx = instance_idx1; idx <= instance_idx2; idx++ )
	{
		// Eingabe
		string filename = string(filenames[idx]) + ".txt";

		// Ausgabe
		string output = string(filenames[idx]) + "_solution.txt";

		// Konstruiere eine Startl�sung
		CreateStartSolution(filename, output);
	}
}
