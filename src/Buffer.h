#ifndef __BUFFER__
#define __BUFFER__

#include <iostream>
#include <cmath>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <sstream>

#include "Wave.h"

#define _INF_ 1e30

class Buffer {

	private:

		std::string _name;

		std::vector<int> _data;
		std::vector<double> _raw;
		std::vector<double> _time;

		std::vector<Wave*> _wave;

		int _size;
		double _duration;	// seconds
		int _sampleSize;	// bits
		int _sampleFreq;	// Hertz

		bool _durationLock;

		// Interpreter

		bool _on;
		int _waveIndex;
		std::vector<std::string> _lastCmd;

	public:

		Buffer(std::string name);
		~Buffer();

		int Scaling(double volume);
		int Normalizing();
		
		void Info(int nb, int buffIndex);
		void InfoB();
	
		std::string GetName()			{ return _name; 			}
		std::vector<int> GetData() 		{ return _data; 			}
		std::vector<double> GetRaw()		{ return _raw; 				}
		std::vector<double> GetTime() 		{ return _time; 			}
		std::vector<Wave*> GetWave()		{ return _wave; 		}
		std::vector<double> GetWave(int nb)	{ return _wave[nb]->GetData(); 		}
		int GetSize() 				{ return _size; 			}
		double GetDuration()	 		{ return _duration; 			}
		double GetSampleSize() 			{ return _sampleSize; 			}
		double GetSampleFreq() 			{ return _sampleFreq; 			}

		void SetName(std::string name) 		{ _name = name; 			}
		void SetData(std::vector<int> data) 	{ _data = data; 			}
		void SetRaw(std::vector<double> raw) 	{ _raw = raw; 				}
		void SetTime(std::vector<double> time) 	{ _time = time; 			}
		void SetSize(int size) 			{ _size = size; 			}
		void SetDuration(double duration) 	{ _duration = duration; 		}

		int SetWaveFunction(int nbWave, std::string func, double duration);
		int SequWaveFunction(int nbWave, int nbSequ);
		int AddWave();
		int RemoveWave(int nb);
		int ChangeWave(int nb);
		void LoadWave(int nb, double offset=0., double volume=1.);
		void DisplayWave(int nb);

};

#endif


