#ifndef __WAVE__
#define __WAVE__

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>

class Wave {

	private:
		
		std::vector<double> _data;
		std::vector<double> _time;
		std::vector<std::string> _formated;
		std::string _function;

		double _sampleRate;
		double _duration;
		
	
	public:
	
		Wave(double sampleRate);
		int Compute(std::string func);

		std::vector<double> GetData()	{ return _data; }
		std::string GetFunction()	{ return _function; }
		double GetDuration()		{ return _duration; }

		void SetDuration(double duration) { _duration = duration; }

};

#endif

