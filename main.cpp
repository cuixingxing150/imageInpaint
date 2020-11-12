/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2008-2012, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "inpainter.h"
int main(int argc, char *argv[])
{
	cv::Mat image, originalImage, inpaintMask;
    //we expect three arguments.
    //the first is the image path.
    //the second is the mask path.
    //the third argument is the halfPatchWidth

    //in case halPatchWidth is not specified we use a default value of 3.
    //in case only image path is speciifed, we use manual marking of mask over the image.
    //in case image name is also not specified , we use default image default.jpg.

    int halfPatchWidth = 3;
	char* imageName = "..\\testsImage\\man.png";
    originalImage=cv::imread(imageName,cv::IMREAD_COLOR);
    if(!originalImage.data){
        std::cout<<std::endl<<"Error unable to open input image"<<std::endl;
        return 0;
    }
    image=originalImage.clone();
	std::string maskName = "..\\testsImage\\man-mask.png";
	inpaintMask = cv::imread(maskName.c_str(), cv::IMREAD_GRAYSCALE);

	Inpainter obj;
	obj.Init(originalImage, inpaintMask, halfPatchWidth);


	if (obj.checkValidInputs() == obj.CHECK_VALID) {
		obj.inpaintImg();

		cv::imwrite("..\\results\\lableMap.jpg", obj.Label);
		cv::imwrite("..\\results\\graphCut.jpg", obj.workImage);
		cv::imwrite("..\\results\\result.jpg", obj.result);
		cv::namedWindow("result");
		cv::imshow("result", obj.result);
		cv::waitKey();
	}
	else
	{
		std::cout << std::endl << "Error : invalid parameters" << std::endl;
		getchar();
	}

    return 0;
}
