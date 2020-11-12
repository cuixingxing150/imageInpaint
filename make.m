% Notice: first use "mex -setup" to choose your c/c++ compiler
clear functions;

%% -------------------------------------------------------------------
%% the configuration of compiler
% You need to modify this configuration according to your own path of OpenCV
% 注意：你的VS OpenCV平台一定要匹配Matlab 64位的！
OUTDIR= ' -outdir ./';% 库文件输出位置，current folder
CPPFLAGS = [' -Iinpainter.h',...
    ' -Igco/*.h',...
    ' -I./*.h',...
    ' -IE:\opencv4_4_0\opencv\build\include',...
    ' -IE:\opencv4_4_0\opencv\build\include\opencv2',...
    ]; % your OpenCV "include" path
LDFLAGS = ' -LE:\opencv4_4_0\opencv\build\x64\vc14\lib'; % use OpenCV release  "lib" path
LIBS = ' -lopencv_world440'; % release版本的lib，无后缀，系统会自动加上去
OUTNAME =' -output inPaint'; % inPaint为生成的mexw64名字
VERBOSE = ' -v';% 是否显示编译信息
VERSION = ' -R2017b';


%% add your files here!
compile_files = [
    % the list of your code files which need to be compiled
    ' ./inPaint.cpp',...
    ' ./inpainter.cpp',...
    ' ./*.cpp',...
    ' ./gco/*.cpp'
    ];
%-------------------------------------------------------------------
%% compiling...
str = compile_files;
fprintf('compilation of: %s\n', str);
str = [str ,OUTDIR,CPPFLAGS,LDFLAGS,LIBS,OUTNAME,VERBOSE,VERSION];
expression = '\s+';
args = regexp(str, expression, 'split');
mex(args{:});

fprintf('Congratulations, compilation successful!!!\n');