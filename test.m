clear all;

%% 测试调用dll
image = imread('testsImage/image3.jpg');
mask = imread('testsImage/mask3.jpg');
halfPatchWidth = 3;

%% inpaint
tic;
if size(mask,3)~=1
    mask = rgb2gray(mask);
end

[workedImg,labelimg,histimg,resultimg] = inPaint(image,mask,halfPatchWidth);
toc;

%% show
figure;imshow(image);title('origin image');
figure;imshow(mask);title('mask image');

figure;imshow(workedImg);title('worked image')
figure;imshow(labelimg);title('label image')
figure;imshow(histimg);title('histogram image')
figure;imshow(resultimg);title('result image')