/*
 * read_shm_png.cpp
 *
 *  Created on: 16.02.2013
 *      Author: ihl
 */

#include "SensorDataPrinterTool.h"
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

#define IMAGE_WIDTH 800
#define HIGH_COLOR 255
#define LOW_COLOR 0
#define END_COLOR 127
/**
 *
 * @param rbuf
 * @param indexer
 */
void printer(RingBufferConsumer &rbuf, IIncrementableIndex &indexer)
{
	unsigned char color = 0;
	vector<int> compression_params;
	vector<unsigned char> output;
	// Highest PNG Compression
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	compression_params.push_back(0);

	int lines = (indexer.getPossibleIncrementations()+1) / IMAGE_WIDTH;
	if((indexer.getPossibleIncrementations()+1) % IMAGE_WIDTH!=0)
	{
		lines++;
	}

	Mat a(lines, IMAGE_WIDTH, CV_8UC1);
	int x, y;
	bool end_data = false;
	for(y = 0; y < lines; y++)
		for(x = 0; x < IMAGE_WIDTH; x++)
		{
			if(!(indexer.getPossibleIncrementations()==0 && end_data))
			{
				if(indexer.getPossibleIncrementations()==0)
					end_data = true;
				if(rbuf.getSensorValue(indexer.getIndex())==1)
					color = HIGH_COLOR;
				else
					color = LOW_COLOR;
				indexer.incrementIndex();
			}
			else
			{
				color = END_COLOR;
			}
			a.at<unsigned char>(y,x) = color;
		}
	imencode(".png", a, output, compression_params);
	cout.write(reinterpret_cast<const char*>(output.data()), output.size());

	//imwrite("test3.png", a, compression_params);
}
/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{
	SensorDataPrinterTool sprinter(argc, argv);
	return sprinter.run(printer);
}

