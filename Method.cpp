
#include "Method.h"
#include "afxdialogex.h"
#include <sstream>
#include <io.h>

using namespace std;

CMethod::CMethod()
{

	n_singleband = 0;
	n_chazhi = 0;

}

CMethod::~CMethod()
{
}


/* -------------------------------------------------------------------- */
	/*                 Count number of pixels in pixel library
	/*                   filePath: Input file path
	/*                   return: Number of pixels
	/* -------------------------------------------------------------------- */
int CMethod::StatisticRow(string filePath)
{
	int mm = 0;
	fstream infile_fire;
	infile_fire.open(filePath, ios::in);
	if (infile_fire.is_open())
	{
		char str[50];
		while (infile_fire.getline(str, sizeof(str)))
		{
			if (str[0] != '\0')   mm++;
		}
	}
	else
	{
		cout << "Fail to open the fire pixels library!!" << endl;
	}
	infile_fire.close();
	return mm;
}

/* -------------------------------------------------------------------- */
	/*                   Read pixels from library to array
	/*      filePath:  Input file path
	/*      Library:    Array to store pixel reflectances
	/*        line:   Number of pixels
	/*        bandIndexes: Band indices to read
	/* -------------------------------------------------------------------- */
int CMethod::ReadPixel(string  filePath, float** Library, int line, vector<string>bandIndexes)
{
	// Create 2D array to store fire pixel library
	for (int i = 0; i < bandIndexes.size(); i++)
	{
		char buffer[20];

		int num = stoi(bandIndexes[i]);
		std::stringstream ss;
		string index;
		ss << num;
		ss >> index;
		string linshi = filePath;
		string path = linshi.append(index);
		path = path.append(".txt");

		fstream infile;
		infile.open(path, ios::in);
		if (infile.is_open())
		{
			infile.clear();
			infile.seekg(0, ios::beg);
			int x = 0;
			while (!infile.eof())
			{
				memset(buffer, '\0', 20 * sizeof(char));
				infile.getline(buffer, 20, '\n');

				if (buffer[0] != '\0')
				{
					string tem(buffer);
					float f1 = atof(buffer);					
					Library[i][x] = f1;					
					x++;
				}
			}
		}
		else
		{			
			cout << "Failed to open pixel library file!!" << endl;
		}
		infile.close();
		cout << num << endl;
	}
	return 0;
}

/* -------------------------------------------------------------------- */
	/*                    Main algorithm generation process
	/*          Test_data_path: Path to test data
	/*          bandIndexes:   Band indices
	/*          sensor:       Sensor name
	/* -------------------------------------------------------------------- */
int CMethod::Algorithm(string sensTest_data_pathor, string sensor, vector<string>bandIndexes)
{
#pragma region Read pixel library data

	// Read fire pixel library
	string fire_path(sensTest_data_pathor);
	fire_path.append("fire\\");
	string linshi = fire_path;
	linshi.append("8.txt");
	int line_fire = StatisticRow(linshi);
	cout << "fire pixels: " << line_fire << endl;

	// Allocate memory for fire pixel data
	float** fire = new float* [3];
	for (int i = 0; i < 3; i++)
	{
		fire[i] = new float[line_fire];
	}	
	ReadPixel(fire_path, fire, line_fire, bandIndexes);


	// Read non-fire pixel library
	string clear_path(sensTest_data_pathor);
	clear_path.append("nonfire\\");
	string linshi2 = clear_path;
	linshi2.append("8.txt");

	int line_clear = StatisticRow(linshi2);
	cout << "Non-fire pixels: " << line_clear << endl;

	// Allocate memory for non-fire pixel data
	float** clear = new float* [3];
	for (int i = 0; i < 3; i++)
	{
		clear[i] = new float[line_clear];
	}
	
	ReadPixel(clear_path, clear, line_clear, bandIndexes);

#pragma endregion Read pixel library data


#pragma region Algorithm generation

	string filePath(sensTest_data_pathor);
	filePath.append(sensor.c_str());
	filePath.append(".txt");

	// Generate detection algorithms
	Generate(fire, clear, filePath, bandIndexes, line_fire, line_clear);


#pragma endregion Algorithm generation

	// Free memory
	for (int i = 0; i < 3; i++)
	{
		delete[]fire[i];
	}
	delete[]fire;

	for (int i = 0; i < 3; i++)
	{
		delete[]clear[i];
	}
	delete[]clear;

	return 0;
}


/* -------------------------------------------------------------------- */
	/*               Generate fire detection algorithms from pixel library
	/*      dataset_fire:  fire pixel data
	/*      dataset_clear:   non-fire pixel data
	/*       filePath:   Output file path
	/*      bandIndexes:    Band indices
	/*       linefire:       Number of fire pixels
	/*       lineClear:      Number of non-fire pixels
	/* -------------------------------------------------------------------- */
int  CMethod::Generate(float** dataset_fire, float** dataset_clear, string filepath, vector<string>bandindexes, int linefire, int lineclear)
{
	// Open output file for writing algorithms
	fstream outfile;
	outfile.open(filepath, ios::out);
	if (!outfile.is_open())
	{
		cout << "Failed to create output file!, error" << endl;
		return 0;
	}

	float mmm;//Fire detection accuracy
	float nnn;//False alarm rate

#pragma region Single band method
	// Single band threshold method
	for (int i = 0; i < bandindexes.size(); i++)
	{
		int linshi_band[100]; // Temporary band storage
		float linshi_threshold[100]; // Temporary threshold storage
		float linshi_zhengquelv[100]; // Temporary accuracy storage
		int ncount = 0; 

		// Test different threshold values from 1.0 to 0.0
		for (float a = 1.0; a > 0.0; a = a - 0.01)
		{
			int m = 0; int n = 0;
			// Count fire pixels above threshold
			for (int j = 0; j < linefire; j++)
			{
				if (dataset_fire[i][j] > a)
				{
					m++;
				}
			}
			// Count non-fire pixels above threshold (false alarms)
			for (int j = 0; j < lineclear; j++)
			{
				if (dataset_clear[i][j] > a)
				{
					n++;
				}
			}
			nnn = float(n) / float(lineclear);//False alarm rate
			mmm = float(m) / float(linefire);//Fire detection accuracy		

			// Criteria: false alarm rate < 5% and accuracy > 85%
			if (nnn < 0.05 && (mmm - nnn) > 0.85)//mmm-2*nnn//
			{
				linshi_band[ncount] = i;
				linshi_threshold[ncount] = a;
				linshi_zhengquelv[ncount] = mmm - nnn;

				ncount++;// Count of valid thresholds
			}
		}

		// Find the threshold with highest accuracy
		int jjj = 0;
		float chushi = 0;
		for (int jj = 0; jj < ncount; jj++)
		{
			if (chushi < linshi_zhengquelv[jj] && linshi_zhengquelv[jj] < 1.0)
			{
				chushi = linshi_zhengquelv[jj];
				jjj = jj;
			}
		}
		// Output the best threshold for this band
		if (ncount != 0)
		{
			threshold_s[n_singleband] = linshi_threshold[jjj];
			zhengquel_s[n_singleband] = linshi_zhengquelv[jjj];

			outfile << "single_band" << " " <<
				atoi(bandindexes[i].c_str()) << " " << threshold_s[n_singleband] << " " <<
				zhengquel_s[n_singleband] << endl;
			n_singleband++;
		}
	}

#pragma endregion Single band method


#pragma region Band difference method	
	// Band difference threshold method
for (int i = 0; i < bandindexes.size(); i++)
{
	for (int j = 0; j < bandindexes.size(); j++)
	{
		if (j != i)// Use different bands
		{
			std::vector<float> diffire(linefire);// Fire pixel differences
			std::vector<float> difclear(lineclear);//Non-fire pixel differences

			int linshi_band[100][2];// Temporary band pair storage
			float linshi_threshold[100];  // Temporary threshold storage
			float linshi_zhengquelv[100];// Temporary accuracy storage
			int ncount = 0;

			// Calculate band differences for fire pixels
			for (int x = 0; x < linefire; x++)
			{
				diffire[x] = dataset_fire[i][x] - dataset_fire[j][x];
			}
			// Calculate band differences for non-fire pixels
			for (int y = 0; y < lineclear; y++)
			{
				difclear[y] = dataset_clear[i][y] - dataset_clear[j][y];
			}

			// Test different threshold values
			for (float a = 0.0; a <= 1.0; a = a + 0.01)
			{
				int m = 0; int n = 0;
				// Count fire pixels with difference above threshold
				for (int x = 0; x < linefire; x++)
				{
					if (diffire[x] > a)
					{
						m++;
					}
				}
				// Count non-fire pixels with difference above threshold
				for (int y = 0; y < lineclear; y++)
				{
					if (difclear[y] > a)
					{
						n++;
					}
				}
				float nnn = float(n) / float(lineclear); // False alarm rate
				float mmm = float(m) / float(linefire);// Detection accuracy

				// Criteria: false alarm rate < 5% and accuracy > 85%
				if (nnn < 0.05 && (mmm - nnn) > 0.85)//mmm-nnn0.70
				{
					linshi_band[ncount][0] = i;
					linshi_band[ncount][1] = j;
					linshi_threshold[ncount] = a;  
					linshi_zhengquelv[ncount] = mmm - nnn;
					ncount++;

					if (ncount >= 100) break;
				}
			}
			// Find the best threshold for this band pair
			if (ncount > 0)
			{
				int jjj = 0;
				float chushi = linshi_zhengquelv[0];
				for (int jj = 1; jj < ncount; jj++)
				{
					if (chushi < linshi_zhengquelv[jj] && linshi_zhengquelv[jj] < 1.0)
					{
						chushi = linshi_zhengquelv[jj];
						jjj = jj;
					}
				}
				if (chushi != 0)
				{
					band_chazhi[n_chazhi][0] = i + 1;
					band_chazhi[n_chazhi][1] = j + 1;
					threshold_chazhi[n_chazhi] = linshi_threshold[jjj];  
					zhengquel_chazhi[n_chazhi] = linshi_zhengquelv[jjj];

					outfile << "difference_band" << " " <<
						atoi(bandindexes[i].c_str()) << " " << atoi(bandindexes[j].c_str()) << " " <<
						threshold_chazhi[n_chazhi] << " " << zhengquel_chazhi[n_chazhi] << endl;
					n_chazhi++;
				}
			}
		}
	}
}

#pragma endregion Band difference method

	return 0;
}

