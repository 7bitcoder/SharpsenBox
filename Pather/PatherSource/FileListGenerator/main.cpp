#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qvariant.h>
#include <iostream>
#include "Generator.hpp"

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "wrong argument number\n";
		//return 0;
	}
	gn::Generator generator;
	std::filesystem::path p = argv[0];
	generator.generate("C:/Users/Sylwester/Desktop/Sylio"); //for testing 
}
