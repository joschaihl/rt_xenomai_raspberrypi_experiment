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

void printer(RingBufferConsumer &rbuf, IIncrementableIndex &indexer)
{
	unsigned char color = 0;
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	compression_params.push_back(0);
	Mat a(800, 800, CV_8UC1);
	int x, y;
	for(y = 0; y < 800; y++)
		for(x = 0; x < 800; x++)
		{
			if(rbuf.getSensorValue(indexer.getIndex())==1)
				color = 255;
			else
				color = 0;
			a.at<unsigned char>(y,x) = color;
			if(indexer.incrementIndex()==false)
				break;
		}


/*	do
    {
    	printf("%llu %u %llu %u\n",indexer.getIndex(),
    			rbuf.getSensorID(indexer.getIndex()),
    			rbuf.getSampleTimeCode(indexer.getIndex()),
    			rbuf.getSensorValue(indexer.getIndex()));
    } while(indexer.incrementIndex());*/

	imwrite("test3.png", a, compression_params);
}

int main(int argc, char **argv)
{
	SensorDataPrinterTool sprinter(argc, argv);
	return sprinter.run(printer);
}

