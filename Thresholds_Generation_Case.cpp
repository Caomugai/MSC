#include <iostream>
#include "Method.h"

int main()
{
	// Create method object
	CMethod Method;

	// Set test data path - modify this path according to your data location
	string Test_data_path = "E:\\programs\\Thresholds_Generation_Case\\Thresholds_Generation_Case\\Test_data\\";

	// Set sensor name (e.g., Sentinel2, Landsat8, etc.)
	string sensor = "Sentinel2";

	// Define bands to be used for fire detection
	// Modify this array based on available bands for your sensor
	string band[3] = {  "8", "11", "12", };	

	// Calculate number of bands in the array
	int len = sizeof(band) / sizeof(string);

	// Vector to store band indexes
	vector<string> bandIndexes;

	// Populate bandIndexes vector with band numbers
	for (int i = 0; i < len; i++)
	{
		bandIndexes.push_back(band[i]);
	}

	// Execute the main algorithm to generate fire detection thresholds
	Method.Algorithm(Test_data_path, sensor, bandIndexes);

	return 0;


}