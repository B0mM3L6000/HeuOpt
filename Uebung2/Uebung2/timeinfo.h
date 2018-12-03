#ifndef TIMEINFO_H
#define TIMEINFO_H

#include <iostream>
#include <sstream>
#include <iomanip>

#include <time.h>
#include <math.h>

class c_TimeInfo {
    clock_t start;
    clock_t stop;
    double offset;
    int i_num_stops;
    double d_min_time;
    double d_max_time;
public:
  c_TimeInfo()
        : start( clock() ), offset(0), i_num_stops(0), d_min_time( 10e30), d_max_time(0.0)
    {
        stop = start;
    }

  void Start()      { start = clock(); offset=0; i_num_stops=0; }
  void Restart()		{ offset += stop-start; start=clock(); }
  void Stop()       { stop=clock();
                                        i_num_stops++;
                                        if ( stop-start < d_min_time ) d_min_time = stop-start;
                                        if ( stop-start > d_max_time ) d_max_time = stop-start;
                                    }
    void Reset()    {   offset=0;   i_num_stops=0; start = clock(); stop = start; d_min_time=10e30; d_max_time=0; }

    int NumStops() const { return i_num_stops; }
    double Hours() const                { return double( stop - start + offset ) / ( CLOCKS_PER_SEC  * 3600.0 ); }
    double Minutes() const          { return double( stop - start + offset ) / ( CLOCKS_PER_SEC  * 60.0 ); }
    double Seconds() const          { return double( stop - start + offset ) / CLOCKS_PER_SEC;  }
    double MilliSeconds() const { return double( stop - start + offset ) / ( CLOCKS_PER_SEC / 1000.0 ); }
    double AvgHours() const                 { return double( stop - start + offset ) / ( CLOCKS_PER_SEC  * 3600.0 ) / double(i_num_stops); }
    double AvgMinutes() const               { return double( stop - start + offset ) / ( CLOCKS_PER_SEC  * 60.0 )/ double(i_num_stops); }
    double AvgSeconds() const               { return double( stop - start + offset ) / CLOCKS_PER_SEC / double(i_num_stops);  }
    double AvgMilliSeconds() const  { return double( stop - start + offset ) / ( CLOCKS_PER_SEC / 1000.0 )/ double(i_num_stops); }
    double MinHours() const                 { return double( d_min_time ) / ( CLOCKS_PER_SEC  * 3600.0 ); }
    double MinMinutes() const               { return double( d_min_time ) / ( CLOCKS_PER_SEC  * 60.0 ); }
    double MinSeconds() const               { return double( d_min_time ) / CLOCKS_PER_SEC;  }
    double MinMilliSeconds() const  { return double( d_min_time ) / ( CLOCKS_PER_SEC / 1000.0 ); }
    double MaxHours() const                 { return double( d_max_time ) / ( CLOCKS_PER_SEC  * 3600.0 ); }
    double MaxMinutes() const               { return double( d_max_time ) / ( CLOCKS_PER_SEC  * 60.0 ); }
    double MaxSeconds() const               { return double( d_max_time ) / CLOCKS_PER_SEC;  }
    double MaxMilliSeconds() const  { return double( d_max_time ) / ( CLOCKS_PER_SEC / 1000.0 ); }

    std::string FormattedTime() const {
        int hours = (int) floor( Hours() );
        int mins  = (int) floor( Minutes() ) - 60 * (int) floor( Hours() );
        int secs  = (int) floor( Seconds() ) - 60 * (int) floor( Minutes() );
        int ms    = (int) floor( MilliSeconds() ) - 1000 * (int) floor( Seconds() );
				using namespace std;
				ostringstream buffer;
				buffer << setfill('0') << setw(2) << hours << ":";
				buffer << setfill('0') << setw(2) << mins << ":";
				buffer << setfill('0') << setw(2) << secs << ",";
				buffer << setfill('0') << setw(3) << ms;
				return buffer.str();
    }
    std::string FormattedAvgTime() const {
        int hours = (int) floor( AvgHours() );
        int mins  = (int) floor( AvgMinutes() ) - 60 * (int) floor( AvgHours() );
        int secs  = (int) floor( AvgSeconds() ) - 60 * (int) floor( AvgMinutes() );
        int ms    = (int) floor( AvgMilliSeconds() ) - 1000 * (int) floor( AvgSeconds() );
				using namespace std;
				ostringstream buffer;
				buffer << setfill('0') << setw(2) << hours << ":";
				buffer << setfill('0') << setw(2) << mins << ":";
				buffer << setfill('0') << setw(2) << secs << ",";
				buffer << setfill('0') << setw(3) << ms;
				return buffer.str();
    }
    std::string FormattedMinTime() const {
        int hours = (int) floor( MinHours() );
        int mins  = (int) floor( MinMinutes() ) - 60 * (int) floor( MinHours() );
        int secs  = (int) floor( MinSeconds() ) - 60 * (int) floor( MinMinutes() );
        int ms    = (int) floor( MinMilliSeconds() ) - 1000 * (int) floor( MinSeconds() );
				using namespace std;
				ostringstream buffer;
				buffer << setfill('0') << setw(2) << hours << ":";
				buffer << setfill('0') << setw(2) << mins << ":";
				buffer << setfill('0') << setw(2) << secs << ",";
				buffer << setfill('0') << setw(3) << ms;
				return buffer.str();
		}
    std::string FormattedMaxTime() const {
        int hours = (int) floor( MaxHours() );
        int mins  = (int) floor( MaxMinutes() ) - 60 * (int) floor( MaxHours() );
        int secs  = (int) floor( MaxSeconds() ) - 60 * (int) floor( MaxMinutes() );
        int ms    = (int) floor( MaxMilliSeconds() ) - 1000 * (int) floor( MaxSeconds() );
				using namespace std;
				ostringstream buffer;
				buffer << setfill('0') << setw(2) << hours << ":";
				buffer << setfill('0') << setw(2) << mins << ":";
				buffer << setfill('0') << setw(2) << secs << ",";
				buffer << setfill('0') << setw(3) << ms;
				return buffer.str();    
		}
};

#endif
