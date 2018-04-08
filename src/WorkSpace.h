#ifndef __WORKSPACE__
#define __WORKSPACE__

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sstream>
#include <ncurses.h>
#include <fstream>

//#include "WavIO.h"

#include "SoundIO.h"
#include "Buffer.h"
#include "Display.h"
#include "Tools.h"

#include "GUIMode.h"

#define _arwNULL  0
#define _arwUP    1
#define _arwDOWN  2
#define _arwRIGHT 3
#define _arwLEFT  4

#define _CHECKABLE 1

class WorkSpace {

	private:

		std::vector<Buffer*> _buffer;
//		WavIO* _wavIO;
		SoundIO* _soundIO;
		Display* _screen;
		Tools _toolBox;

		int _argOffset;
		int _argTime;
		double _argDelay;

		bool _on;
		std::vector<std::string> _lastCmd;
		std::vector<std::string> _history;
		int _historyIndex;
		int _historyMark;
		int _buffIndex;
		int _waveIndex;

		int _mode;
		int _arrowPressed;

		// Plotting

		std::string _termPath;

	public:

		WorkSpace();
		~WorkSpace();

		void Run();
		int Mode(char carac);
		void Format(std::string cmd);
		std::vector<std::string> Read();
		void Arrows(char carac);
		void Execute();

		void RefreshInterpreter();

		// Commands.cpp

			// Buffer
			void ToUpBuffer();
			void ToDownBuffer();
			int cmdNewBuffer();
			int cmdDeleteBuffer();
			int cmdRenameBuffer();
			int cmdAssignBuffer();
			
			// Wave
			void ToUpWave();
			void ToDownWave();
			int cmdSetWave();
			int cmdSequWave();

			// Audio stream
			int cmdStr();
			int cmdPlay();
			int cmdPause();
			int cmdSend();

		// KeyboardInput.cpp

		int LoadHistoryCmd();
		int SaveHistoryCmd();

		// Plotting.cpp

		void LoadPlotting();
		void ExecPlotting(std::string cmd);

		// Tools.cpp

		bool BufferNbTest(int nb);
};

#endif

