
#include "Tools.h"

using namespace std;

Tools::Tools() {}

string Tools::toString(int x) {
	stringstream ss;
	ss << x;
	return ss.str();
}

/*string Tools::toString(double x) {
	stringstream ss;
	ss << x;
	return ss.str();
}*/
