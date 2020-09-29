#include "PlatformerGame.h"
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	PlatformerGame* game = new PlatformerGame(argc, argv);
	ofstream myfile;
	myfile.open("0.txt");
	myfile << "Writing this to a file.\n";
	myfile.close();
}