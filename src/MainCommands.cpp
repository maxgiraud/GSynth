
#include "WorkSpace.h"

using namespace std;

// Explorer

void WorkSpace::ToUpBuffer() {
	if (_buffIndex <= 0) { _buffIndex = _buffer.size()-1; }
	else { _buffIndex--; }
	_arrowPressed = _arwNULL;
}

void WorkSpace::ToDownBuffer() {
	if (_buffIndex >= _buffer.size()-1) { _buffIndex = 0; }
	else { _buffIndex++; }
	_arrowPressed = _arwNULL;
}

void WorkSpace::ToUpWave() {
	if (_waveIndex <= 0) { _waveIndex = _buffer[_buffIndex]->GetWave().size()-1; }
	else { _waveIndex--; }
	_arrowPressed = _arwNULL;
}

void WorkSpace::ToDownWave() {
	if (_waveIndex >= _buffer[_buffIndex]->GetWave().size()-1) { _waveIndex = 0; }
	else { _waveIndex++; }
	_arrowPressed = _arwNULL;
}


// Commands

int WorkSpace::cmdStr() {

	if (_lastCmd[1] == "restart") {
		_screen->AddOutputBuffer("Restarting Audio streaming... ",_CHECKABLE);
		_soundIO->Off(); _soundIO = new SoundIO; _soundIO->On();
		_screen->CheckOutputBuffer();
	}

	return 0;
}

int WorkSpace::cmdPlay() { _soundIO->Play(); }

int WorkSpace::cmdPause() { _soundIO->Pause(); }

int WorkSpace::cmdSend() {

	string temp;

	if (_waveIndex == -1) {

		_buffer[_buffIndex]->Normalizing();
		_soundIO->Send(_buffer[_buffIndex]->GetRaw());

	} else {

		temp = _toolBox.toString(_waveIndex);
		_screen->AddOutputBuffer("Sending "+_screen->Write("Wave #"+temp,_Wred)+"... ",_CHECKABLE);
		_soundIO->Send(_buffer[_buffIndex]->GetWave(_waveIndex));
		_screen->CheckOutputBuffer();
	}
}

