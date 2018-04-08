#ifndef __DISPLAY__
#define __DISPLAY__

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <vector>

#include "Buffer.h"

#include "GUIMode.h"

#define _LENGTH	145

#define _noCHECKABLE	-2
#define _noCOLOR	"-1"

#define _Wblack		"30"
#define _Wred		"31"
#define _Wgreen		"32"
#define _Wyellow	"33"
#define _Wblue		"34"
#define _Wmagenta	"35"
#define _Wcyan		"36"
#define _Wwhite		"37"

#define _Sblack		"40"
#define _Sred		"41"
#define _Sgreen		"42"
#define _Syellow	"43"
#define _Sblue		"44"
#define _Smagenta	"45"
#define _Scyan		"46"
#define _Swhite		"47"

#define _RESET		"0"     // everything back to normal
#define _BRIGHT		"1"     // often a brighter shade of the same colour
#define _ULINE		"4"
#define _INV		"7"     // swap foreground and background colours
#define _UNBRIGHT	"21"
#define _UNULINE	"24"
#define _UNINV		"27"

class Display {

	private:

		std::vector<Buffer*> *_buffer;
		int *_buffIndex, *_waveIndex;

		bool _on;
		bool _inputON;
		bool _outputON;
		std::vector<std::string> _outputBuffer;

		double _lastFrame;
		double _FPS, _SPF;
		double _maxFPS, _minSPF;

		std::string _kbBuffer;
		int _lastMode;

		int _cursorOffset;

	public:

		Display(std::vector<Buffer*> &buffer, int &buffIndex, int &waveIndex);
		~Display();

		int Run(int _mode);

		void Banner();
		void Menu();
		void BufferEdit();
		void Input();
		void Output();

		void Title(std::string txt, int indent, int titleNb, int index=-3);
		void TextBox(std::string txt, int indent);

		std::string Write(std::string txt, std::string Wcolor=_noCOLOR, std::string Scolor=_noCOLOR);
		void Format(std::string action);

		void SetBuffer(std::vector<Buffer*> buffer)	{ _buffer = &buffer; }
		void SetInputOn(bool on)			{ _inputON = on; }
		void SetOutputOn(bool on)			{ _outputON = on; }

		std::string GetKbBuffer()	{ return _kbBuffer; }
		bool GetInputOn()		{ return _inputON; }
		bool GetOutputOn()		{ return _outputON; }

		void AddKbBuffer(std::string carac) { _kbBuffer.insert(_kbBuffer.size()-_cursorOffset,carac); }
		void SubKbBuffer() { if (_kbBuffer.size() > 0 && _cursorOffset < _kbBuffer.size()) { _kbBuffer.erase(_kbBuffer.end()-_cursorOffset-1); } }
		void SetKbBuffer(std::string cmd) { _kbBuffer = cmd; _cursorOffset = 0; }
		void ClearKbBuffer() { _kbBuffer = ""; _cursorOffset = 0; }

		void AddOutputBuffer(std::string out, bool check);
		void CheckOutputBuffer();
		void UnCheckOutputBuffer();
		void ClearOutputBuffer(std::string out);

		void IncCursor() { if (_cursorOffset < _kbBuffer.size()) { _cursorOffset++; }}
		void DecCursor() { if (_cursorOffset > 0) { _cursorOffset--; }}
};

#endif
