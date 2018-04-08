#include "WorkSpace.h"

using namespace std;

WorkSpace::WorkSpace () {

	_on = true;
	_buffIndex = 0;
	_waveIndex = -1;
	_historyIndex = 1;
	_historyMark = 0;

	Buffer* temp = new Buffer("root");
	_buffer.push_back(temp);

//	_wavIO = new WavIO;

	_soundIO = new SoundIO;
	_soundIO->On();

	_screen = new Display(_buffer, _buffIndex, _waveIndex);

	_mode = _mNORMAL;
	_arrowPressed = _arwNULL;

//	LoadPlotting();

	LoadHistoryCmd();
}

WorkSpace::~WorkSpace () {

	_soundIO->Off();

	SaveHistoryCmd();

	delete _screen;

	for (int i(0); i < _buffer.size(); i++) {
		delete _buffer[i];
	}

//	delete _wavIO;
	delete _soundIO;
}

void WorkSpace::Run() {

	char carac(0);

	while (_on) {

		carac = 0;

		_screen->Run(_mode);
		initscr();
		noecho();
		timeout(500);
		cbreak();
		carac = getch();
		Arrows(carac);
		nocbreak();
		endwin();
		system("stty cooked");

		Mode(carac);
		if (!_screen->GetInputOn()) {	
			Execute();
		}
	}
}


int WorkSpace::Mode(char carac) {

	if (!_screen->GetInputOn()) {

		switch(carac) {
	
			case ':':
				_screen->SetInputOn(true);
				break;

			case '\n':
				_mode = _mEDITBUFFER;
				_waveIndex = 0;
				break;

			case 127:
				_mode = _mNORMAL;
				_waveIndex = -1;
				break;
		};
	
	} else if (_screen->GetInputOn()) {
	
		string temp;
		temp.assign(&carac, 1);
	
		if (carac == '\n') {
			_screen->SetInputOn(false);
			Format(_screen->GetKbBuffer());
			_screen->ClearKbBuffer();
			_historyIndex = _history.size();

		} else if (carac == 127) { _screen->SubKbBuffer(); }

		else if (_arrowPressed == _arwUP) {
			if (_historyIndex > 0) {
				_historyIndex--; 
				_screen->SetKbBuffer(_history[_historyIndex]);
			}
			_arrowPressed = _arwNULL;
		}
		else if (_arrowPressed == _arwDOWN) {
			if (_history.empty()) {}
			else if (_historyIndex < (_history.size()-1)) {
				_historyIndex++;
				_screen->SetKbBuffer(_history[_historyIndex]);
			} else if (_historyIndex == _history.size()-1) { _screen->SetKbBuffer(""); _historyIndex++; }
			_arrowPressed = _arwNULL;
		}

		else if (_arrowPressed == _arwLEFT) { _screen->IncCursor(); _arrowPressed = _arwNULL; }
		else if (_arrowPressed == _arwRIGHT) { _screen->DecCursor(); _arrowPressed = _arwNULL; }

		else if (carac != -1) { _screen->AddKbBuffer(temp); }
	
	}

	return 0;
}


void WorkSpace::Format(string cmd) {

	_lastCmd.clear();
	string temp("");

	for (int i(0); i < cmd.length(); i++) {
		if (cmd[i] == ' ') {
			_lastCmd.push_back(temp);
			temp = "";
		} else {
			temp += cmd[i];
		}
	}
	_lastCmd.push_back(temp);

	if (cmd != "") {
		_history.push_back(cmd);
		_historyIndex++;
	}
}



void WorkSpace::Arrows(char carac) {
	if (carac == '\033') {
		getch();
		switch(getch()) {
			case 'A':
				_arrowPressed = _arwUP;
				break;
			case 'B':
				_arrowPressed = _arwDOWN;
				break;
			case 'C':
				_arrowPressed = _arwRIGHT;
				break;
			case 'D':
				_arrowPressed = _arwLEFT;
				break;
		};
	}
}

void WorkSpace::Execute() {

	if (!_screen->GetInputOn() && _arrowPressed != _arwNULL) {

		switch(_arrowPressed) {
			case _arwUP:
				if (_waveIndex == -1) { ToUpBuffer(); }
				else { ToUpWave(); }
				break;

			case _arwDOWN:
				if (_waveIndex == -1) { ToDownBuffer(); }
				else { ToDownWave(); }
				break;

			case _arwRIGHT:
				break;

			case _arwLEFT:
				break;
		};
	}

	if (_lastCmd.size() == 0) 			{			}
	else if (_lastCmd[0] == "str")			{ cmdStr(); 		}
	else if (_lastCmd[0] == "play")			{ cmdPlay(); 		}
	else if (_lastCmd[0] == "pause")		{ cmdPause(); 		}
	else if (_lastCmd[0] == "send")			{ cmdSend(); 		}
	else if (_lastCmd[0] == "new")			{ cmdNewBuffer();	}
	else if (_lastCmd[0] == "rename")		{ cmdRenameBuffer(); 	}
	else if (_lastCmd[0] == "delete")		{ cmdDeleteBuffer(); 	}
	else if (_lastCmd[0] == "set")			{ cmdSetWave(); 	}
	else if (_lastCmd[0] == "sequ")			{ cmdSequWave(); 	}
	else if (_lastCmd[0] == "$")			{ cmdAssignBuffer();	}
	else if (_lastCmd[0] == "") 			{ /*nothing to do*/	}
	else if (_lastCmd[0] == "q") 			{ _on = false; 		}

	_lastCmd.clear();
}


