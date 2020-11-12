

#include "mex.h"
#include "matrix.h"
#include "opencv2/opencv.hpp"
#include "inpainter.h"
 
Inpainter inpaintObj;

void checkInputs_init(int nrhs, const mxArray *prhs[])
{
 
	// Check number of inputs
	if (nrhs != 3)
	{
		mexErrMsgIdAndTxt("MATLAB:inputempty", "Incorrect number of inputs. Function expects 3 inputs.");
	}
	if (!mxIsUint8(prhs[0]))
	{
		mexErrMsgIdAndTxt("MATLAB:inputimage", "输入图像image必须是uint8类型.");
	}
	if (!mxIsUint8(prhs[1]))
	{
		mexErrMsgIdAndTxt("MATLAB:inputimage", "输入图像mask必须是uint8类型.");
	}
	if (!mxIsScalar(prhs[2]))
	{
		mexErrMsgIdAndTxt("MATLAB:inputimage", "输入的patchSize必须是double类型.");
	}
}

 
// matlab call inpaintObj(image,mask,halfPatchWidth)
void init(const mxArray *prhs[])
{
	uchar * inputPr1 = (uchar *)mxGetData(prhs[0]);
	const mwSize *dim1 = mxGetDimensions(prhs[0]);
	const mwSize dim1_num = mxGetNumberOfDimensions(prhs[0]);

	int m1 = dim1[0];// rows
	int n1 = dim1[1];// cols
	if (dim1_num != 3)
	{
		mexErrMsgIdAndTxt("MATLAB:inputempty", "image data must 3 channels!");
	}
	Mat image = cv::Mat::zeros(m1, n1, CV_8UC3);
	for (size_t i = 0; i < m1; i++)
	{
		for (size_t j = 0; j < n1; j++)
		{
			image.at<Vec3b>(i, j)[0] = inputPr1[j*m1 + i + 2 * m1*n1]; // blue
			image.at<Vec3b>(i, j)[1] = inputPr1[j*m1 + i + m1*n1];
			image.at<Vec3b>(i, j)[2] = inputPr1[j*m1 + i];
		}
	}
	if (image.empty())
	{
		mexErrMsgIdAndTxt("MATLAB:inputempty","image is empty()!");
	}

	uchar * inputPr2 = (uchar *)mxGetData(prhs[1]);
	const mwSize *dim2 = mxGetDimensions(prhs[1]);
	const mwSize dim2_num = mxGetNumberOfDimensions(prhs[1]);

	int m2 = dim2[0];// rows
	int n2 = dim2[1];// cols
	if (dim2_num >2)
	{
		mexErrMsgIdAndTxt("MATLAB:inputempty", "mask data must 1 channels!");
	}
	if ((m1!=m2)||(n1!=n2))
	{
		mexErrMsgIdAndTxt("MATLAB:inputempty", "image和mask 图像size必须大小一致！");
	}
	Mat mask = cv::Mat::zeros(m2, n2, CV_8UC1);
	for (size_t i = 0; i < m2; i++)
	{
		for (size_t j = 0; j < n2; j++)
		{
			mask.at<uchar>(i, j) = inputPr2[j*m2 + i];
		}
	}
	if (mask.empty()) {
		mexErrMsgIdAndTxt("MATLAB:inputempty", "mask is empty()!");// 不能有中文符号
	}

	/* Check to be sure input argument is a scalar */
	if (!(mxIsScalar(prhs[2]))) {
		mexErrMsgIdAndTxt("MATLAB:mxIsScalar:invalidInputType",
			"Input must be a scalar.");
	}
	int halfPatchWidth = (int)mxGetScalar(prhs[2]); 

	 // 初始化
	inpaintObj.Init(image, mask, halfPatchWidth);

}
 
// matlab call this function must in this way:
// [workedImg,labelimg,histimg] = inPaint(); 
void inpaint(int nlhs,mxArray *plhs[])
{
	if (nlhs!=4) // 输出workedimg和label,histimg图像
	{
		mexErrMsgIdAndTxt("MATLAB:A:B", "输出必须有4个参数!");
	}

	inpaintObj.inpaintImg();
	Mat workedImg = inpaintObj.workImage;
	Mat labeledImg = inpaintObj.Label;
	Mat histImg = inpaintObj.histImage;
	Mat resultImg = inpaintObj.result;
	int rows = workedImg.rows;
	int cols = workedImg.cols;

	// convert to mxArray 
	const mwSize sz1[] = {rows,cols ,workedImg.channels() }; //https://github.com/peiyunh/tiny/issues/24
	const mwSize sz2[] = { labeledImg.rows,labeledImg.cols ,labeledImg.channels() };
	const mwSize sz3[] = { histImg.rows, histImg.cols ,histImg.channels() };
	const mwSize sz4[] = { resultImg.rows, resultImg.cols ,resultImg.channels() };
	plhs[0] = mxCreateNumericArray(workedImg.channels(), sz1, mxUINT8_CLASS, mxREAL); // mxCreateNumericArray可以创建高于2维的数据, 代表workedImg
	plhs[1] = mxCreateNumericArray(labeledImg.channels(), sz2, mxUINT8_CLASS, mxREAL); // mxCreateNumericArray可以创建高于2维的数据, 代表labeledImg
	plhs[2] = mxCreateNumericArray(histImg.channels(), sz3, mxUINT8_CLASS, mxREAL); // mxCreateNumericArray可以创建高于2维的数据, 代表histImg
	plhs[3] = mxCreateNumericArray(resultImg.channels(), sz4, mxUINT8_CLASS, mxREAL); // mxCreateNumericArray可以创建高于2维的数据, 代表result

	mxUint8 *temp,*temp2,*temp3,*temp4;
	temp = (mxUint8*)mxGetPr(plhs[0]); //mxGetUint8s 是R2018a引进的函数，应当mex适合加上-R2018a选项
	temp2 = (mxUint8*)mxGetPr(plhs[1]);
	temp4 = (mxUint8*)mxGetPr(plhs[3]);
	for (int i = 0; i <rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			// worked image
			*(temp + i + j * rows + 0) = (mxUint8)workedImg.at<cv::Vec3b>(i, j)[2]; // R
			*(temp + i + j * rows + rows*cols) = (mxUint8)workedImg.at<cv::Vec3b>(i, j)[1];// G
			*(temp + i + j * rows + 2 * rows*cols) = (mxUint8)workedImg.at<cv::Vec3b>(i, j)[0];// B
			// label image
			*(temp2 + i + j * rows) = (mxUint8)labeledImg.at<cv::Vec3b>(i, j)[2];
			*(temp2 + i + j * rows + rows*cols) = (mxUint8)labeledImg.at<cv::Vec3b>(i, j)[1];
			*(temp2 + i + j * rows + 2*rows*cols) = (mxUint8)labeledImg.at<cv::Vec3b>(i, j)[0];
			// result image
			*(temp4 + i + j * rows) = (mxUint8)resultImg.at<cv::Vec3b>(i, j)[2];
			*(temp4 + i + j * rows + rows*cols) = (mxUint8)resultImg.at<cv::Vec3b>(i, j)[1];
			*(temp4 + i + j * rows + 2 * rows*cols) = (mxUint8)resultImg.at<cv::Vec3b>(i, j)[0];
		}
	}		

	temp3 = (mxUint8*)mxGetPr(plhs[2]);
	for (int i = 0; i <histImg.rows; i++)
	{
		for (int j = 0; j < histImg.cols; j++)
		{
			*(temp3 + i + j * histImg.rows) = (mxUint8)histImg.at<cv::Vec3b>(i, j)[2];
			*(temp3 + i + j * histImg.rows+ histImg.rows*histImg.cols) = (mxUint8)histImg.at<cv::Vec3b>(i, j)[1];
			*(temp3 + i + j * histImg.rows+2* histImg.rows*histImg.cols) = (mxUint8)histImg.at<cv::Vec3b>(i, j)[0];
		}
	}
}

void exitFcn()
{

}

//在matlab中必须以这种方式调用： [workedImg,labelimg,histimg,resultimg] = inPaint(image,mask,halfPatchWidth);输入3项，输出4项
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	checkInputs_init(nrhs, prhs);
	init(prhs);
	inpaint(nlhs, plhs);

}
