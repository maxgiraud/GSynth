
#include "Wave.h"

using namespace std;

Wave::Wave (double sampleRate) {

	_function = "Empty";

	_sampleRate = sampleRate;
	_duration = 0;
}

/*
int Wave::Compute(string func) {

	string temp("");
	_function = func;
	_formated.clear();

	for (int i(0); i < func.size(); i++) {

		if (func[i] != '*') {
			temp += func[i];

		} else {
			_formated.push_back(temp);
			_formated.push_back("*");
			temp = "";
		}
	}
	_formated.push_back(temp);

}
*/


int Wave::Compute(string func) {

	_function = func;

	ofstream macro("data/macro.m");

	macro	<< "duration = " << _duration << ";" << endl
		<< "sample_rate = " << _sampleRate << ";" << endl
		<< "step = 1/sample_rate;" << endl
		<< "t = [0:step:duration]';" << endl
		<< "rd = rand(size(t));" << endl
		<< "data = " << _function << ";" << endl
		<< "out = [t data];" << endl

		<< "save -ascii data/func.dat out" << endl;

	macro.close();

	system("octave data/macro.m");

	ifstream dataFile("data/func.dat");
	
	if (dataFile.is_open()) {

		double t, data;

		_time.clear();
		_data.clear();

		while (dataFile >> t >> data) {
			_time.push_back(t);
			_data.push_back(data);
		}

		_duration = _time[_time.size()-1];
	}
}

