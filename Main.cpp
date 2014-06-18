
#include <iostream>
#include "h/AppConfig.h"
#include "h/EMGProvider.h"
#include "h/EMGFileProvider.h"
#include "h/EMGOTProvider.h"
#include "h/MultiClassSVM.h"
#include "h/Trainer.h"
#include "h/Classifier.h"
#include "h/Plotter.h"

/**
* It is necessary to pass an argument with the path to the configuration file.
* The argument has to look like 'online_classificator.config <path_to_file>'
* with a whitespace before the path.
*/
int main(int argc, char *argv[]) {
	//loads all configurations and inits the logging system
	try {
		motion_classifier::AppConfig::load(argc, argv);
		motion_classifier::AppConfig::getInstance()->initLogging();
	}
	catch (std::invalid_argument& ex) {
		std::cerr << "Exception while loading configuration: " << ex.what() << std::endl;
		std::cerr << "Motion-Classifier usage: " << motion_classifier::AppConfig::CONFIG_ARGUMENT << " <path_to_config>" << std::endl;
	}

	std::string path = "c:\\Users\\Marc\\Dropbox\\Informatik\\Studium\\6. Semester\\Bachelor Thesis\\MARC\\data\\data8_AN-format.txt";

	//It takes very much time to delete EMGProvider and/or Classifier.
	//Therefor I added this block, so that both are destroyed before the end
	//of the program is reached.
	{
		//EMGOTProvider emgProvider{};
		motion_classifier::EMGFileProvider emgProvider{ path };
		motion_classifier::MultiClassSVM svm;

		motion_classifier::Trainer trainer(&emgProvider, &svm);
		//trainer.train();

		motion_classifier::Classifier classifier(&emgProvider, &svm);
		classifier.send(motion_classifier::Signal::START);
		Sleep(30000);
		classifier.send(motion_classifier::Signal::SHUTDOWN);
	}

	//closes all open resources, releases heap memory, ...
	Plotter::release();
	motion_classifier::AppConfig::release();

	//Only used to keep the terminal still visible, when the program is finished
	std::cout << "Please press Enter to close program... ";
	std::cin.get();
	return EXIT_SUCCESS;
}