
#include <iostream>
#include "h/AppConfig.h"
#include "h/EMGFileProvider.h"
#include "h/Logger.h"
#include "h/MultiClassSVM.h"
#include "h/Properties.h"
#include "h/SVMClassifier.h"
#include "h/Sample.h"
#include "h/Trainer.h"

void testLogger(std::string message);

/**
* It is necessary to pass an argument with the path to the configuration file.
* The argument has to look like 'online_classificator.config <path_to_file>'
* with a whitespace before the path.
*/
int main(int argc, char *argv[]) {

	//loads all configurations and inits the logging system
	try {
		motion_classifier::AppConfig::load(argc, argv);
		Logger::getInstance()->addLogFunction(&testLogger);
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
		//configuration for SVMClassifier
		Properties prop("svm_config.txt");

		//EMGOTProvider emgProvider{};
		motion_classifier::EMGFileProvider emgProvider{ path };
		motion_classifier::EMGProviderImpl *p = &emgProvider;
		motion_classifier::MultiClassSVM svm(&prop);
		motion_classifier::SVMClassifier classifier{ &emgProvider, &svm, &prop };

		//motion_classifier::Trainer trainer{};
		//auto trainingsData = trainer.train(&emgProvider);
		//classifier.train(trainingsData);

		classifier.send(motion_classifier::Signal::START);
		std::this_thread::sleep_for(std::chrono::milliseconds(30000));
		classifier.send(motion_classifier::Signal::SHUTDOWN);
	}

	//closes all open resources, releases heap memory, ...
	motion_classifier::AppConfig::release();

	//Only used to keep the terminal still visible, when the program is finished
	std::cout << "Please press Enter to close program... ";
	std::cin.get();
	return EXIT_SUCCESS;
}

void testLogger(std::string message) {
	std::cout << message << std::endl;
}