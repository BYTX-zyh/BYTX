# BYTX



## 操作及指令：

### 初始化

调用`BYTX init` 



## 注意事项

1.  如果您在使用中发现了一些问题，可以通过1872839448@qq.com 对我进行联系。
2.  由于本人菜鸡，所以不会写流程图，故而建议如果必须插入流程图的话请使用图片格式。
3.  在即将推出的主题中，已经存在了页面内索引，故而不建议使用`[TOC]`标签，因为会打印出`[TOC]`。
4.  关于斜体和加粗的一些字体格式化，推荐使用HTML标签`<b>`,`<i>`如果在使用中发现bug请联系我。
5.  关于HTML标签的使用，由于只做了原始的标签判定所以只支持`<b>`这种，而不支持内联样式的HTML标签。
6.  除非数学公式请不要使用`$$` 
7.  不建议使用非`#`的标题，将会导致无法识别

## 文档书写

在文档开头通过如下形式加入文档信息：

```
+++
title:my new markdown file
data:2020/02/29 09:08:21
tags:about init
+++
```

- [ ] init初始化结束后进行主题选择等，对index.htmltag.HTML初始化
- [ ] 增加函数确定是否有文件被删除，同时增加delete操作
- [ ] 检测markdown内部嵌套的HTML元素，与需要转义的<>等区分开
- [ ] 在最后的时候增加gitignore文件
- [ ] 检测内部嵌套HTML标签
- [ ] 考虑如何后期嵌套CSS
- [ ] 考虑行内特殊形式的转化，如行内代码
- [ ] 考虑关键字转化以后是否存在使用了HTML标签失败的情况如`<br>`等
- [ ] 加入对index和tag页面的更新
- [ ] head部分的渲染考虑如何增加highlight.js以及如何保证主题的更改。