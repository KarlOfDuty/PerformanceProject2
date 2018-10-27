#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

float generateRand(int max)
{
	return (((float)rand() / (float)RAND_MAX)*max);
}

void createDataset(int datasetSize, int bufferSize, std::string datasetFilename)
{
	std::ofstream out;
	out.open(datasetFilename);
	
	std::string newLine = "";
	for (int i = 0; i < bufferSize; i++)
    {
		out << newLine << std::fixed << generateRand(datasetSize);
		newLine = "\n";
	}

	out.close();
}

float* loadDataset(std::string datasetFilename, int bufferSize)
{
	float* newds = new float[bufferSize];

	std::ifstream in;
	in.open(datasetFilename); 
	
	int i = 0;
	while (!in.eof())
	{
		in >> newds[i++]; //Ehm borde detta fungera? Det gör det iaf
	}

	return newds;
}

float average(float ds[], int bufferSize)
{
	float total = 0.f;
	for (int i = 0; i < bufferSize; i++)
    {
		total += ds[i];
	}
	return total / bufferSize;
}

float maxvalue(float ds[], int bufferSize)
{
	float max = 0.f;
	for (int i = 0; i < bufferSize; i++)
    {
		if (ds[i] > max)
        {
			max = ds[i];
		}
	}
	return max;
}

float minvalue(float ds[], int bufferSize)
{
	float min = FLT_MAX;
	for (int i = 0; i < bufferSize; i++)
    {
		if (ds[i] < min)
        {
			min = ds[i];
		}
	}
	return min;
}

float* sortDataset(float* ds, int bufferSize)
{
	//Deepcopy right?
	float* sds = new float[bufferSize];
	for (int i = 0; i < bufferSize; i++)
    {
		sds[i] = ds[i];
	}

	//Sort on SDS
	float key;
	int j;
	for (int i = 1; i < bufferSize; i++)
    {
		key = sds[i];
		j = i - 1;
		while (j >= 0 && sds[j] > key)
        {
			sds[j + 1] = sds[j];
			j = j - 1;
		}
		sds[j + 1] = key;
	}
	return sds;
}

void writeDataset(std::string OutputFilename, float sds[], int bufferSize, float avg, float min, float max)
{
	std::ofstream out;
	out.open(OutputFilename);
	
	out << "Average = " << avg << "\n";
	out << "Max = " << max << "\n";
	out << "Min = " << min;

	for (int i = 0; i < bufferSize; i++)
	{
		out << "\n" << std::fixed << sds[i];
	}
	out.close();
}

int main()
{
	//PROBLEM MED RANDOM GENERERING? Väldigt många lika nummer
	srand((unsigned int)time(0));
	
	int datasetSize = 100; 
	int bufferSize = 1024*100;

	createDataset(datasetSize, bufferSize, "testfil.txt");

	float* ds = loadDataset("testfil.txt", bufferSize);
	
	float avg = average(ds, bufferSize);
	
	float max = maxvalue(ds, bufferSize);
	
	float min = minvalue(ds, bufferSize);

	float* sds = sortDataset(ds, bufferSize);

	writeDataset("testfil2.txt", sds, bufferSize, avg, min, max);

	return 0;
}