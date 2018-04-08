#include "Buffer.h"

using namespace std;

Buffer::Buffer (string name) {

	_name = name;

	_size = 0;
	_duration = 0;
	_sampleSize = 32;
//	_sampleFreq = 1000;
	_sampleFreq = 44100;

	_durationLock = false;

	Wave* wave = new Wave(_sampleFreq);
	_wave.push_back(wave);

	_waveIndex = 0;
}


Buffer::~Buffer () {

	for (int i(0); i < _wave.size(); i++) {

		delete _wave[i];
	}
}


int Buffer::Scaling (double volume) {

	if (_raw.size() != 0 && _sampleSize > 0) {
	
		double min(_INF_), max(-_INF_), value;

		for (int i(0); i < _size; i++) {
			value = _raw[i];
			
			if (value < min) { min = value; }
			if (value > max) { max = value; }
		}

		double amp = volume*pow(2.,_sampleSize)/2.;
		double scale = pow(2.,_sampleSize)/(max-min)*volume;

		_data.clear();
		_data.resize(_size);
		for (int i(0); i < _size; i++) {
			_data[i] = _raw[i]*scale;
		}

	} else {
		cout << "  |-- Buffer::Scaling Error | Raw array is empty, Size = " << _size << " sample" << endl;
		return -1;
	}

	return 0;
}

int Buffer::Normalizing () {

	if (_raw.size() != 0 && _sampleSize > 0) {
	
		double min(_INF_), max(-_INF_), value;

		for (int i(0); i < _size; i++) {
			value = _raw[i];
			
			if (value < min) { min = value; }
			if (value > max) { max = value; }
		}

		double scale = 2./(max-min);

		_data.clear();
		_data.resize(_size);
		for (int i(0); i < _size; i++) {
			_data[i] = _raw[i]*scale;
		}

	} else {
		cout << "  |-- Buffer::Normalizing Error | Raw array is empty, Size = " << _size << " sample" << endl;
		return -1;
	}

	return 0;
}


void Buffer::LoadWave(int nb, double offset, double volume) {

	double duration = _wave[nb]->GetDuration();

	if (!_durationLock && ((duration+offset) > _duration)) {
		_duration = duration+offset;
		_size = floor(_sampleFreq*_duration);
		
		for (int i(0); i < _size; i++) {
			
			if (i < _time.size()) { _time[i] = i/(_size+0.)*_duration; }
			else { _time.push_back(i/(_size+0.)*_duration); }
		}
	}

	int imax = floor((offset+duration)/_duration*_size);


	double value;
	for (int i(0); i < imax; i++) {
		
//cout << "pute = " << i-ceil(offset/_duration*_size) << "/" << _wave[nb]->GetData().size() << endl;
		if (i < (offset+duration)/_duration*_size && i > offset/_duration*_size) {
			value = _wave[nb]->GetData()[i-ceil(offset/_duration*_size)];
		} else { value = 0; }

		if (i < _raw.size())	{ _raw[i] += value; }
		else 			{ _raw.push_back(value); }
//cout << "chienne" << endl;
	}

}


void Buffer::InfoB() {

	cout << "  |   |-- @" << _name << "\t\t" << _duration << " sec\t" << _sampleSize << " bit\t" << _sampleFreq << " Hz\tSize " << _size << endl;

		cout << "  |   |   |" << endl;
		for (int i(0); i < _wave.size(); i++) {

			if (i == _wave.size()-1) {
				cout << "  |   |   |_";
				if (i == _waveIndex) { cout << ">"; }
				else { cout << "_"; }
			} else {
				cout << "  |   |   |-";
				if (i == _waveIndex) { cout << ">"; }
				else { cout << "-"; }
			}

			cout << " #" << i << " :\t" << _wave[i]->GetFunction() << "\t\t" << _wave[i]->GetDuration() << " sec" << endl;
		}
		cout << "  |   |" << endl;
}

void Buffer::Info(int nb, int buffIndex) {

	if (nb == buffIndex) { cout << "  >"; }
	else { cout << "  |"; }

	cout << "-- #" << nb << " : " << _name << "\t\t" << _duration << " sec\t" << _sampleSize << " bit\t" << _sampleFreq << " Hz\tSize " << _size << endl;

	if (nb == buffIndex) {
		cout << "  |   |" << endl;
		for (int i(0); i < _wave.size(); i++) {

			if (i == _wave.size()-1) { cout << "  |   |__"; }
			else { cout << "  |   |--"; }

			cout << " #" << i << " :\t" << _wave[i]->GetFunction() << "\t\t" << _wave[i]->GetDuration() << " sec" << endl;
		}
		cout << "  |" << endl;
	}
}


int Buffer::SetWaveFunction(int nbWave, string func, double duration) {

	_wave[nbWave]->SetDuration(duration);
	_wave[nbWave]->Compute(func);
	DisplayWave(0);
}


int Buffer::SequWaveFunction(int nbWave, int nbSequ) {
/*
        if (_lastCmd.size() == 2) {

                int markNb(atoi(_lastCmd[1].c_str()));
*/
                vector<double> markData(_wave[nbSequ]->GetData());

                for(int i(0); i < markData.size(); i++) {
                        if (markData[i] == 1 && markData[i-1] == 0) {
//cout << (double)i/(_sampleFreq+0.) << endl;
                                LoadWave(_waveIndex,(double)i/(_sampleFreq+0.));
 /*                      }
                }

        } else {
                cout << "  |   |--> Use : sequ [marking_wave]" << endl;
        }

*/}}	
}


int Buffer::AddWave() {

	Wave* wave = new Wave(_sampleFreq);
	_wave.push_back(wave);

	return 0;
}

int Buffer::RemoveWave(int nb) {

	if (nb < _wave.size() && nb >= 0) { _wave.erase(_wave.begin()+nb); }

	return 0;
}

int Buffer::ChangeWave(int nb) {

	if (nb < _wave.size() && nb >= 0) { _waveIndex = nb; }

	return 0;
}

void Buffer::DisplayWave(int nb) {
/*
        ofstream macro("data/macro_vis.m");

        macro   << "s = load(\"data/func.dat\");" << endl
		<< "plot(s(:,1), s(:,2))";

        macro.close();

	system("gnome-terminal -x sh -c \"tty > data/termpath.dat; octave --no-gui --persist data/macro_vis.m; bash\"");

	ifstream termpath("data/termpath");
	termpath >> _termpath;
	termpath.close();
//        system("octave --no-gui --persist data/macro_vis.m");
*/
}

