
#include "WorkSpace.h"

using namespace std;

int WorkSpace::LoadHistoryCmd() {

	string filename("data/history.dat"), temp;

	ifstream file(filename.c_str());
	_screen->AddOutputBuffer("Loading commands history "+filename+"... ",_CHECKABLE);

	if (file.is_open()) {

		while (getline(file,temp)) {

			_history.push_back(temp);
			_historyIndex++;
		}
		_historyMark = _historyIndex;
		file.close();

	} else { _screen->UnCheckOutputBuffer(); return -1; }

	_screen->CheckOutputBuffer();

	return 0;
}

int WorkSpace::SaveHistoryCmd() {

	string filename("data/history.dat");

	ofstream file(filename.c_str(), ios::app);

	if (file.is_open()) {
		
		for (int i(_historyMark); i < _history.size(); i++) {
			file << _history[i] << endl;
		}
		file.close();
	}
	
	return 0;
}

