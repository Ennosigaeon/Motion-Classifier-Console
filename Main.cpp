#include <iostream>
#include "h/AppConfig.h"
#include "h/CrossCorrelation.h"
#include "h/EMGFileProvider.h"
#include "h/Logger.h"
#include "h/MultiClassSVM.h"
#include "h/MSClassifier.h"
#include "h/Properties.h"
#include "h/SVMClassifier.h"
#include "h/Sample.h"
#include "h/Trainer.h"
#include "h/Utilities.h"

using namespace motion_classifier;

/**
* It is necessary to pass an argument with the path to the configuration file.
* The argument has to look like 'online_classificator.config <path_to_file>'
* with a whitespace before the path.
*/
int main(int argc, char *argv[]) {
	//loads all configurations and inits the logging system
	try {
		motion_classifier::AppConfig::load(argc, argv);
	}
	catch (std::invalid_argument& ex) {
		std::cerr << "Exception while loading configuration: " << ex.what() << std::endl;
		std::cerr << "Motion-Classifier usage: " << motion_classifier::AppConfig::CONFIG_ARGUMENT << " <path_to_config>" << std::endl;
	}

	std::string path = "c:/Tmp/data/data8_AN-f";

	//It takes very much time to delete EMGProvider and/or Classifier.
	//Therefor I added this block, so that both are destroyed before the end
	//of the program is reached.
	{
		//configuration for Classifier
		Properties prop("ms_config.txt");

		EMGFileProvider emgProvider{ path };
		MSClassifier classifier{ &emgProvider, &prop };
		classifier.train("C:/Tmp/RMS/");

		CrossCorrelation correlation(&classifier);
		correlation.testClassifier(correlation.loadData("C:/Tmp/RMS/", 1));

		//classifier.send(motion_classifier::Signal::START);
		//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		//classifier.send(motion_classifier::Signal::SHUTDOWN);
	}

	//closes all open resources, releases heap memory, ...
	motion_classifier::AppConfig::release();

	//Only used to keep the terminal still visible, when the program is finished
	std::cout << "Please press Enter to close program... ";
	std::cin.get();
	return EXIT_SUCCESS;
}
