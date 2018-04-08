
#include "WorkSpace.h"

using namespace std;

void WorkSpace::LoadPlotting() {

        ofstream macro("data/macro_vis.m");

        macro   << "s = load(\"data/func.dat\");" << endl
                << "plot(s(:,1), s(:,2))";

        macro.close();

        system("gnome-terminal -x sh -c \"tty > data/termpath.dat; bash\"");

        ifstream termpath("data/termpath.dat");
        termpath >> _termPath;
        termpath.close();

	string temp("exec 1>"+_termPath+"; octave --no-gui --persist data/macro_vis.dat");
	system(temp.c_str());
//	system("octave --no-gui --persist");
//	system("exec 1>/dev/pts/4");

	_screen->AddOutputBuffer("Loading plotting system on "+_termPath+"... ", _CHECKABLE);
	_screen->CheckOutputBuffer();

}

void WorkSpace::ExecPlotting(string cmd) {

	_screen->AddOutputBuffer("exit "+_termPath+"... ",_noCHECKABLE);

	string temp("exec 1>"+_termPath+"; exit");
//	system(temp.c_str());
//	system(cmd.c_str());
//	system("exit");
}
