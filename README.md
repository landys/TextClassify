# TextClassify

It's a Chinese text multiple classification application. It's the AI course project in my first year graduate in 2007 done with a friend.

##

首先，在使用本文本分类器需把系统时间改为2007年或之前，因为本系统使用的CSW库免费版截止在2007年或之前。

压缩包包括源代码（TextClassify），文档（pdf版），测试结果文件。

TextClassify中TextClassify.sln可在Visual Studio 2003及以上版本打开，里面包括三个工程：SplitSystem，svm_multiclass_learn，svm_multiclass_classify，分别在Release文件夹下生成三个相应的可执行文件。bat下为四个辅助运行脚本。sample下为测试目录，其中sample\properties.dat为配置文件，文件内不要有空行；生成的结果位于sample\result中，或在sample\properties.dat中配置。sample\sample下有两套分好类别的测试和训练数据，每套分为同样的10类。

详细见文档中使用说明。