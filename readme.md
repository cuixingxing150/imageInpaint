# Image Completion approaches using the statistics of Similar Patches



| 原图                         | label                       | result |
| ---------------------------- | --------------------------- | ------ |
| ![](/images/1.jpg) | ![](/images/5.jpg) | ![](/images/3.jpg)        |


# Implementation detail
1. calculate the patch offsets  histogram by PatchMatch
2. get the k dominate offset  
3. multi-label by graph-cut
4. gradient-domain fusion

# Requires

1. OpenCV <br>
2. Matlab>=2017b <br>
3. VS2013/2015/2017/2019/... <br>
tested only on win10 <br>

# reference
Kaiming He, Jian Sun. [Image Completion Approaches Using the Statistics of Similar Patches](http://kaiminghe.com/publications/pami14completion.pdf )