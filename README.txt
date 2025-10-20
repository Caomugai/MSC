# fire Detection Threshold Generation Tool

## Overview
A C++ based automated tool for generating optimal thresholds for fire detection in satellite imagery. The tool analyzes pixel libraries of fire and non-fire areas to determine the most effective thresholds for reliable fire detection.

## Features
- **Dual Detection Methods**: Implements both single-band and band-difference thresholding approaches
- **Accuracy Optimization**: Automatically selects thresholds that maximize detection accuracy while minimizing false positives
- **Multi-Sensor Support**: Configurable for various satellite sensors (Sentinel-2, Landsat, etc.)
- **Flexible Band Configuration**: Easy adaptation to different band combinations

## Detection Methods

### 1. Single Band Method
- Analyzes individual bands to find optimal reflectance thresholds
- Tests thresholds from 0.0 to 1.0 in 0.01 increments
- Selects thresholds that meet accuracy criteria

### 2. Band Difference Method  
- Uses differences between band pairs for enhanced fire detection
- Particularly effective for cirrus fire detection using SWIR bands
- Implements single threshold approach for band differences

## Accuracy Criteria
- **False Alarm Rate**: < 5% (non-fire pixels incorrectly classified as fire)
- **Detection Accuracy**: > 85% (fire pixels correctly identified)  
- **Net Accuracy**: Detection accuracy minus false alarm rate must exceed 85%

## Project Structure


Thresholds_Generation_Case/
├── Thresholds_Generation_Case.cpp # Main application entry point
├── Method.h # Class declaration and interface
├── Method.cpp # Class implementation and algorithms
└── Test_data/ # Pixel library directory
├── fire/ # Fire pixel samples
│ ├── 8.txt # Band 8 reflectance values
│ ├── 11.txt # Band 11 reflectance values
│ └── 12.txt # Band 12 reflectance values
└── nonfire/ # Non-fire pixel samples
├── 8.txt
├── 11.txt
└── 12.txt



## Input Data Format
- Each band requires a separate text file
- Files contain reflectance values (0.0 to 1.0), one value per line
- fire pixels in 'fire' directory, non-fire pixels in 'nonfire' directory

## Output
Generates a threshold file (e.g., Sentinel2.txt) containing:
single_band <band_number> <threshold> <accuracy>
difference_band <band1> <band2> <threshold> <accuracy>



## Configuration
Modify the main.cpp file to set:
1. **Test_data_path**: Path to your pixel library directory
2. **sensor**: Name of your satellite sensor
3. **band array**: Bands to use for analysis

Example configuration for Sentinel-2:
```cpp
string Test_data_path = "your/path/to/Test_data/";
string sensor = "Sentinel2";
string band[3] = { "8", "11", "12" };