
#include "Display.h"

using namespace std;

Display::Display(vector<Buffer*> &buffer, int &buffIndex, int &waveIndex) {
	
	_buffer = &buffer;
	_buffIndex = &buffIndex;
	_waveIndex = &waveIndex;

	_inputON = false;
	_outputON = true;

	_lastFrame = clock();
	_FPS = 50;
	_SPF = 1./_FPS;

	_cursorOffset = 0;
}

Display::~Display() {
//	system("clear");
}

int Display::Run(int mode) {

	double time;
	_lastMode = mode;	

	system("clear");

	if (mode == _mNORMAL) { Menu(); }
	else if (mode == _mEDITBUFFER) { BufferEdit(); }

	if (_inputON) { Input(); }
	if (_outputON) { Output(); }

	time = (clock()-_lastFrame)/(CLOCKS_PER_SEC+0.);
	_maxFPS = 1./time;
	_minSPF = time;
	if (time < _SPF) {
		usleep((_SPF-time)*1e6);
	}
	_lastFrame = clock();

	return 0;
}



		// *** MODES *** //


void Display::BufferEdit() {

	stringstream ss;

	cout << endl;

	Banner();
	cout << endl << endl;

	string out;

	out = _buffer->at(*_buffIndex)->GetName();
	Title(out,1,_noCHECKABLE);

	out = "  ";
	out += _buffer->at(*_buffIndex)->GetWave().size();
	out += " wave";
	out = "\t\tDuration = ";
	ss << _buffer->at(*_buffIndex)->GetDuration();
	out += ss.str(); ss.str(std::string()); ss.clear();
	out += " sec\t\tSR = ";
	ss << _buffer->at(*_buffIndex)->GetSampleFreq();
	out += ss.str(); ss.str(std::string()); ss.clear();
	out += " Hz\t\tSS = ";
	ss << _buffer->at(*_buffIndex)->GetSampleSize();
	out += ss.str(); ss.str(std::string()); ss.clear();
	out += " bits\t\tSize = ";
	ss << _buffer->at(*_buffIndex)->GetSize()*_buffer->at(*_buffIndex)->GetSampleSize()/1e6;
	out += ss.str(); ss.str(std::string()); ss.clear();
	out += " Mo";
	TextBox(out,-1);

	
	for (int i(0); i < _buffer->at(*_buffIndex)->GetWave().size(); i++) {
		out = "Wave #";
		ss << i;
		out += ss.str(); ss.str(std::string()); ss.clear();
		Title(out,2,i, *_waveIndex);

		out = "  Duration = ";
		ss << _buffer->at(*_buffIndex)->GetWave()[i]->GetDuration();
		out += ss.str(); ss.str(std::string()); ss.clear();
		out += " sec \tSize = ";
		ss << _buffer->at(*_buffIndex)->GetWave()[i]->GetData().size()*_buffer->at(*_buffIndex)->GetSampleSize()/1e6;
		out += ss.str(); ss.str(std::string()); ss.clear();
		out += " Mo\t\t";
		out += Write(_buffer->at(*_buffIndex)->GetWave()[i]->GetFunction(),_Wred);
		TextBox(out,-1);
	}
}


void Display::Menu() {

	stringstream ss;

	cout << endl;

	Banner();
	cout << endl << endl;

	string out;
	for (int i(0); i < _buffer->size(); i++) {
		out = _buffer->at(i)->GetName();
		Title(out,1,i,*_buffIndex);

		out = "\t\tDuration = ";
		ss << _buffer->at(i)->GetDuration();
		out += ss.str(); ss.str(std::string()); ss.clear();
		out += " sec\t\tSR = ";
		ss << _buffer->at(i)->GetSampleFreq();
		out += ss.str(); ss.str(std::string()); ss.clear();
		out += " Hz\t\tSS = ";
		ss << _buffer->at(i)->GetSampleSize();
		out += ss.str(); ss.str(std::string()); ss.clear();
		out += " bits\t\tSize = ";
		ss << _buffer->at(i)->GetSize()*_buffer->at(i)->GetSampleSize()/1e6;
		out += ss.str(); ss.str(std::string()); ss.clear();
		out += " Mo";

		TextBox(out,-1);

		if (i == *_buffIndex) {
			for (int j(0); j < _buffer->at(i)->GetWave().size(); j++) {
				out = "Wave #";
				ss << j;
				out += ss.str(); ss.str(std::string()); ss.clear();
				Title(out,2,j, *_waveIndex);
	
				out = "  Duration = ";
				ss << _buffer->at(i)->GetWave()[j]->GetDuration();
				out += ss.str(); ss.str(std::string()); ss.clear();
				out += " sec \tSize = ";
				ss << _buffer->at(i)->GetWave()[j]->GetData().size()*_buffer->at(i)->GetSampleSize()/1e6;
				out += ss.str(); ss.str(std::string()); ss.clear();
				out += " Mo\t\t";
				out += Write(_buffer->at(i)->GetWave()[j]->GetFunction(),_Wred);
				TextBox(out,-1);
			}
		}
	}
}

void Display::Input() {

	string temp(_kbBuffer), temp2(" ");

	cout << Write("  Input ",_Wyellow, _INV);
	if (_cursorOffset != 0) {
		cout	<< " > " 	<< Write(temp.substr(0,temp.size()-_cursorOffset),_Wblue, _BRIGHT)
			<< Write(temp.substr(temp.size()-_cursorOffset,1),_INV)
			<< Write(temp.substr(temp.size()-_cursorOffset+1,_cursorOffset-1),_Wblue, _BRIGHT) << endl << endl;
	} else {
		cout	<< " > " << Write(temp,_Wblue,_BRIGHT)
			<< Write(temp2,_INV) << endl << endl;
	}
}

void Display::Output() {

	cout << Write(" Output ",_Wyellow, _INV);

	for (int i(_outputBuffer.size()-1); i >= 0; i--) {
		if (i != _outputBuffer.size()-1) { cout << "\t"; }
		cout << " > " <<  _outputBuffer[i];
	}
	cout << endl << endl;

}

void Display::AddOutputBuffer(string out, bool check) {

	if(_outputBuffer.size() >= 30) { _outputBuffer.erase(_outputBuffer.begin()); }
	_outputBuffer.push_back(out);
	if(!check) { _outputBuffer[_outputBuffer.size()-1] += "\n"; }

	Run(_lastMode);
}

void Display::CheckOutputBuffer() { _outputBuffer[_outputBuffer.size()-1] += Write("done",_Wwhite,_Sgreen)+"\n"; }
void Display::UnCheckOutputBuffer() { _outputBuffer[_outputBuffer.size()-1] += Write("fail",_Wwhite,_Sred)+"\n"; }

void Display::ClearOutputBuffer(std::string out) {

	_outputBuffer.clear();
}


void Display::Banner() {

	string txt = "          G-SYNTH version 2.0     Created by Maxime GIRAUD";	
	string txt2;	
	stringstream ss;
	
	Format(_INV);
	cout << txt;
	
	ss << _minSPF*1000 << " ms / " << round(_minSPF/_SPF*1000.) << "% ";
	txt2 = ss.str();

	for (int i(0); i < (_LENGTH-txt.length()-txt2.length()); i++) { cout << " "; }
	
	cout << txt2;

	Format(_UNINV);
}




		// *** EDITION TOOLS *** //


void Display::Title(string txt, int indent, int titleNb, int index) {

	string out;

	if (indent == 0) {
		Format(_INV);
		for (int i(0); i < 10; i++) { cout << " "; }
		cout << txt;
		for (int i(0); i < (_LENGTH-10-txt.length()); i++) { cout << " "; }
		Format(_UNINV);

	} else {

		if (index == titleNb) { out = ">"; }
		else { out = "-"; }
		out += " ";
		out += txt;
		out += " ";
	
		for (int i(0); i < indent; i++) {
			cout << "\t";
		}
		if (index == titleNb) { out = Write(out,_Wred); }
		Format(_INV);
		cout << out;
		Format(_UNINV);
	}
}

void Display::TextBox(string txt, int indent) {

	string out = "";
	
	for (int i(0); i < indent; i++) { out += "\t  "; }
	for (int i(0); i < txt.length(); i++) {
		
		if (txt[i] == '\n') {
			out += '\n';
			for (int i(0); i < indent; i++) { out += "\t  "; }
		} else {
			out += txt[i];
		}
	}
	cout << out << endl << endl;
}

string Display::Write(string txt, string Wcolor, string Scolor) {

	string str("");

	if (Wcolor != _noCOLOR || Scolor != _noCOLOR) {
		str += "\033[";
		if (Wcolor != _noCOLOR) { str += Wcolor; }
		if (Wcolor != _noCOLOR && Scolor != _noCOLOR) { str += ";"; }
		if (Scolor != _noCOLOR) { str += Scolor; }
		str += "m";
	}

	str += txt;

	if (Wcolor != _noCOLOR || Scolor != _noCOLOR) { str += "\033[0m"; }

	return str;
}

void Display::Format(string action) {

	cout << "\033[" << action << "m";
}


