#include <fstream>
#include "iostream"
#include <vector>
#include <string>
#include <string.h>
using namespace std;

class CMethod
{

public:
	CMethod();   // Standard constructor
	~CMethod();

	//Accuracy rates for single band method
	int band_s[50];//Bands for single band method
	float threshold_s[50];//Thresholds for single band method
	float zhengquel_s[50]; //Accuracy rates for single band method
	int n_singleband;//Accuracy rates for single band method

	//Band difference method variables
	int band_chazhi[500][2];//Band pairs for difference method [band1, band2]
	float threshold_chazhi[500];//Thresholds for band difference method 
	float zhengquel_chazhi[500];//Accuracy rates for band difference method
	int n_chazhi;	//Number of band difference methods

	/* -------------------------------------------------------------------- */
	/*                 Count number of pixels in pixel library    
	/*                   filePath: Input file path  
	/*                   return: Number of pixels    
	/* -------------------------------------------------------------------- */
	int StatisticRow(string filePath);

	/* -------------------------------------------------------------------- */
	/*                   Read pixels from library to array
	/*      filePath:  Input file path
	/*      Library:    Array to store pixel reflectances 
	/*        line:   Number of pixels
	/*        bandIndexes: Band indices to read
	/* -------------------------------------------------------------------- */
	int ReadPixel(string  filePath, float** Library, int line, vector<string>bandIndexes);


	/* -------------------------------------------------------------------- */
	/*                    Main algorithm generation process
	/*          Test_data_path: Path to test data
	/*          bandIndexes:   Band indices  
	/*          sensor:       Sensor name         
	/* -------------------------------------------------------------------- */
	int Algorithm(string sensTest_data_pathor, string sensor, vector<string>bandIndexes);


	

	/* -------------------------------------------------------------------- */
	/*               Generate fire detection algorithms from pixel library 
	/*      dataset_fire:  fire pixel data
	/*      dataset_clear:   non-fire pixel data
	/*       filePath:   Output file path 
	/*      bandIndexes:    Band indices  
	/*       linefire:       Number of fire pixels
	/*       lineClear:      Number of non-fire pixels   
	/* -------------------------------------------------------------------- */
	int Generate(float** dataset_fire, float** dataset_clear, string filePath, vector<string>bandIndexes, int linefire, int lineClear);

};

