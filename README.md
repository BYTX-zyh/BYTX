# BYTX

## init

关于初始化：

调用`BYTX init` 



## 文档书写

在文档开头通过如下形式加入文档信息：

```
---
title:my new markdown file
data:2020/02/29 09:08:21
tags:about init
---
```

- [ ] init初始化结束后进行主题选择等，对index.htmltag.HTML初始化
- [ ] 增加函数确定是否有文件被删除，同时增加delete操作
- [ ] 检测markdown内部嵌套的HTML元素，与需要转义的<>等区分开
- [ ] 在最后的时候增加gitignore文件
- [ ] 考虑是否存在某一行有两个*但是不是为了字体转化等类似问题
- [ ] 检测内部嵌套HTML标签
- [ ] 考虑如何后期嵌套CSS
- [ ] 考虑行内特殊形式的转化，如行内代码，行内高亮等。
- [ ] 考虑关键字转化以后是否存在使用了HTML标签失败的情况如`<br>`等
- [ ] 在turn_word中加入行内检测
- [ ] 加入对index和tag页面的更新
- [ ] 对换行hr和代码块的区分
- [ ] head部分的渲染考虑如何增加highlight.js以及如何保证主题的更改。