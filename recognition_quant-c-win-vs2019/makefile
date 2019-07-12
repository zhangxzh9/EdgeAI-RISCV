# makefile
#简单版makefile文件
# recognition : recognition.o data.o
# 	gcc -o recognition recognition.o data.o
# recognition.o : recognition.c data.h
# 	gcc -c recognition.c
# data.o : data.c data.h 
# 	gcc -c data.c

# clean:
# 	del /F *.o *.exe


# 描述： multifile 项目 makefile文件
# 版本： v1.0
# 修改记录:  1.先测试普通的cpp文件的编译运行
#           2.使用变量来改进我们的makefile文件
#           3.新加了一个源文件
#           4.使用伪目标，加上clean规则
#           5.使用wildcard函数，自动扫描当前目录下的源文件
#           6.加入了自动规则依赖 
# 定义了可执行文件变量
executable:= recognition
# 定义了源文件列表变量
sources := $(wildcard *.c)
# 使用变量的引用替换，定义了object文件列表
objects := $(sources:.c=.o)
# 使用变量引用替换，定义依赖文件列表
deps  := $(sources:.c=.d)

# LIBS    := 
# LDFLAGS := -L
# INCLUDE := -I


# 定义编译命令变量
CC := gcc
RM := del /F
#RM := rm -mf
# 终极目标规则，生成可执行文件
$(executable) : $(objects)
	$(CC) -o $@ $^ 

$(objects): %.o:%.c 
	$(CC) -o $@ -c $<

# 自动规则依赖
sinclude $(deps)
$(deps):%.d:%.c
	$(CC) -MM $< > $@

#clean规则 
.PHONY: clean
clean:
#清除编译生成的所有文件
#$(RM) $(executable) $(objects) $(deps)
	$(RM) $(objects) $(deps) *.exe

# 描述： multifile 项目 makefile文件
# 版本： v2.0
# 修改记录:  1.先测试普通的cpp文件的编译运行
#           2.使用变量来改进我们的makefile文件
#           3.新加了一个源文件
#           4.使用伪目标，加上clean规则
#           5.使用wildcard函数，自动扫描当前目录下的源文件
#           6.加入了自动规则依赖
#           7.实现了opencv程序的编译
# # 定义了可执行文件变量
# executable  := main
# # 定义了源文件列表变量
# sources := $(wildcard *.cpp)
# # 使用变量的引用替换，定义了object文件列表
# objects := $(sources:.cpp=.o)
# # 使用变量引用替换，定义依赖文件列表
# deps    := $(sources:.cpp=.d)
# # 定义编译命令变量
# CC  := g++
# rm  := rm -rf

# #需要调用的链接库
# LIBS    :=  -lopencv_calib3d \
#             -lopencv_core \
#             -lopencv_dnn \
#             -lopencv_features2d \
#             -lopencv_flann \
#             -lopencv_highgui \
#             -lopencv_imgcodecs \
#             -lopencv_imgproc \
#             -lopencv_ml \
#             -lopencv_objdetect \
#             -lopencv_photo \
#             -lopencv_shape \
#             -lopencv_stitching \
#             -lopencv_superres \
#             -lopencv_video \
#             -lopencv_videoio \
#             -lopencv_videostab
# # 头文件路径
# INCLUDE :=  -I/usr/local/include/opencv \
#             -I/usr/local/include/opencv2
# # 链接库路径
# LDFLAGS :=  -L/usr/local/lib
# # 终极目标规则，生成可执行文件
# $(executable) : $(objects)
#     $(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

# $(objects): %.o:%.cpp 
#     $(CC) -o $@ -c $< $(INCLUDE)

# #clean规则 
# .PHONY: clean
# clean:
# #清除编译生成的所有文件
# #$(RM) $(executable) $(objects) $(deps)
# #清除编译生成的所有文件,不包括可执行文件
#     $(RM) $(objects) $(deps)

# # 自动规则依赖
# sinclude $(deps)
# $(deps):%.d:%.cpp
#     $(CC) -MM $< > $@