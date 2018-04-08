
#include "WorkSpace.h"

using namespace std;


int WorkSpace::cmdNewBuffer() {

	if (_lastCmd.size() == 2) {

		Buffer* temp = new Buffer(_lastCmd[1]);
		_buffer.push_back(temp);

	} else if (_lastCmd.size() == 1) {
		_buffer[_buffIndex]->AddWave();
	} else {
		return -1;
	}

	return 0;
}


int WorkSpace::cmdRenameBuffer() {

	if (_lastCmd.size() == 2) { _buffer[_buffIndex]->SetName(_lastCmd[1]); }

	else { return -1; }

	return 0;
}

int WorkSpace::cmdDeleteBuffer() {
	
	if (_lastCmd.size() < 2) {
		return -1;
	}

	int buffNb(atoi(_lastCmd[1].c_str()));

	if (_lastCmd[1] == "all") {

		if (_waveIndex == -1) { _buffer.clear(); }
		else { _buffer[_buffIndex]->GetWave().clear(); }

	} else {

		if (_waveIndex == -1) { _buffer.erase(_buffer.begin()+buffNb); }
		else { _buffer[_buffIndex]->RemoveWave(buffNb); }
	}

	if (_buffIndex == buffNb) { _buffIndex = 0; }
	if (_waveIndex == buffNb) { _waveIndex = 0; }

	if (_buffer.size() == 0) {
		Buffer* temp = new Buffer("root");
        	_buffer.push_back(temp);
		_buffIndex = 0;
	}

	if (_buffer[_buffIndex]->GetWave().size() == 0) {
        	_buffer[_buffIndex]->AddWave();
		_waveIndex = 0;
	}

	return 0;
}


int WorkSpace::cmdSetWave() {

	if (_lastCmd.size() == 3) {
		_screen->AddOutputBuffer("Setting wave #"+_toolBox.toString(_waveIndex)+" "
					+_screen->Write("f(t) = "+_lastCmd[2],_Wblue, _BRIGHT)
					+" for "+_lastCmd[1]+"s... ",_CHECKABLE);		
		_buffer[_buffIndex]->SetWaveFunction(_waveIndex, _lastCmd[2], atof(_lastCmd[1].c_str()));
		_screen->CheckOutputBuffer();		
//		LoadPlotting();
	} else {
		return -1;
	}

	return 0;
}


int WorkSpace::cmdSequWave() {

	if (_lastCmd.size() == 2) {
		_screen->AddOutputBuffer("Sequencing wave #"+_toolBox.toString(_waveIndex)+" with #"+_lastCmd[1]+"...",_CHECKABLE);
		_buffer[_buffIndex]->SequWaveFunction(_waveIndex, atof(_lastCmd[1].c_str()));
		_screen->CheckOutputBuffer();		

	} else {
		return -1;
	}

	return 0;
}

int WorkSpace::cmdAssignBuffer() {

}
