#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#define USE_INSERTIONSORT true
LARGE_INTEGER frequency;

float generateRand(int max)
{
	return (((float)rand() / (float)RAND_MAX)*max);
}

void createDataset(int datasetSize, std::string datasetFilename)
{
	std::ofstream out;
	out.open(datasetFilename);
	
	std::string newLine = "";
	for (int i = 0; i < datasetSize; i++)
    {
		out << newLine << std::fixed << generateRand(100);
		newLine = "\n";
	}

	out.close();
}

float* loadDataset(std::string datasetFilename, int datasetSize, int bufferSize)
{
	float* dataset = new float[datasetSize];

	std::ifstream in;

	char* buffer = new char[bufferSize];
	in.rdbuf()->pubsetbuf(buffer, bufferSize);

	in.open(datasetFilename); 
	
	int i = 0;
	while (!in.eof())
	{
		in >> dataset[i++];
	}

	return dataset;
}

float average(float ds[], int datasetSize)
{
	float total = 0.f;
	for (int i = 0; i < datasetSize; i++)
    {
		total += ds[i];
	}
	return total / datasetSize;
}

float maxvalue(float ds[], int datasetSize)
{
	float max = 0.f;
	for (int i = 0; i < datasetSize; i++)
    {
		if (ds[i] > max)
        {
			max = ds[i];
		}
	}
	return max;
}

float minvalue(float ds[], int datasetSize)
{
	float min = FLT_MAX;
	for (int i = 0; i < datasetSize; i++)
    {
		if (ds[i] < min)
        {
			min = ds[i];
		}
	}
	return min;
}

int partition(float* arr, int low, int high)
{
    float pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(float* ds, int low, int high)
{
    if (low < high)
    {
        int pi = partition(ds, low, high);
        quickSort(ds, low, pi - 1);
        quickSort(ds, pi + 1, high);
    }
}

float* sortDataset(float* ds, int datasetSize)
{
	//Deepcopy
	float* sds = new float[datasetSize];
	for (int i = 0; i < datasetSize; i++)
    {
		sds[i] = ds[i];
	}

    if (USE_INSERTIONSORT)
    {
	    //Insertion Sort on SDS
	    float key;
	    int j;
	    for (int i = 1; i < datasetSize; i++)
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
    }
    else
    {
        quickSort(sds, 0, datasetSize - 1);
    }
	return sds;
}

void writeDataset(std::string OutputFilename, float sds[], int datasetSize, int bufferSize, float avg, float min, float max)
{
	std::ofstream out;

	char* buffer = new char[bufferSize];
	out.rdbuf()->pubsetbuf(buffer, bufferSize);

	out.open(OutputFilename);
	
	out << "Average = " << avg << "\n";
	out << "Max = " << max << "\n";
	out << "Min = " << min;

	for (int i = 0; i < datasetSize; i++)
	{
		out << "\n" << std::fixed << sds[i];
	}
	out.close();
}

bool fileExists(const std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

int main()
{
    LARGE_INTEGER startTime;
    LARGE_INTEGER endTime;

    float avg = 0.0f;
    float max = 0.0f;
    float min = 0.0f;

    double endTimeMillisecond = 0.0;

    QueryPerformanceFrequency(&frequency);
	srand((unsigned int)time(0));
	
	int datasetSize = 1024*100;
	int bufferSize = 4; //4 Bytes = float size in bytes

    // Creates the dataset if it does not exist
    if (!fileExists("inputdata.txt"))
    {
        std::cout << "Creating dataset file...\n";
	    createDataset(datasetSize, "inputdata.txt");
        std::cout << "Done!\n";
    }

    std::cout << "Starting test...\n";

    // Sets high definition start time
    QueryPerformanceCounter(&startTime);

    // Loads the dataset
	float* dataset = loadDataset("inputdata.txt", datasetSize, bufferSize);
	
    // Performs calculations on the dataset
	avg = average(dataset, datasetSize);
	max = maxvalue(dataset, datasetSize);
	min = minvalue(dataset, datasetSize);

    // Sorts the dataset
	float* sortedDataset = sortDataset(dataset, datasetSize);

    // Write the sorted list to file with results from calculations
	writeDataset("outputdata.txt", sortedDataset, datasetSize, bufferSize, avg, min, max);

    // Sets high definition end time
    QueryPerformanceCounter(&endTime);

    endTimeMillisecond = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;

    std::cout << "Done!\nTime elapsed: " << endTimeMillisecond << "ms.\n";
    system("PAUSE");
	return 0;
}