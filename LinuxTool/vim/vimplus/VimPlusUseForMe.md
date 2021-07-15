# VimPlus的操作手册(For Swin)

## cpp-mode
用于一些类的定义实现等  
注意Class的标准写法:类名占据一行
* **,a** :Switch c++头文件和实现文件切换
* **,y** :CopyCode 拷贝函数或变量，和:PasteCode结合使用
* **,p** :PasteCode 生成函数实现或变量定义，和:CopyCode结合使用
* **,U** :GoToFunImpl 转到函数实现，该功能可替代ycm提供的转到函数实现

## vim-edit
在某行快速复制剪切，以及替换单词  
拷贝的内容用p粘贴
* **Y** :CopyText 拷贝文本 - 从当前光标到指定字符位置 内容
* **D** :DeleteText 剪切文本 - 从当前光标到指定字符位置 内容
* **C** :ChangeText 改写文本 - 从当前光标到指定字符位置 内容，并进入insert模式
* **,r** :ReplaceTo 全局替换 - 将当前单词替换成目标单词(单个文件)

## change-colorscheme
快速切换终端主题
* **F9** :PreviousColorScheme 加载上一个主题
* **F10** :NextColorScheme 加载下一个主题

## prepare-code
新建文件时，生成预定义代码片段

## vim-buffer
* **ctrl + n** :NextBuffer 切换到下一个buffer
* **ctrl + p** :PreviousBuffer 切换到上一个buffer(在我设置的软件中有其他快捷键)
* **,d** :CloseCurrentBuffer 关闭当前buffer
* **,D** :BufOnly 删除当前buffer外的所有buffer

## vimplus-startify
vimplus开始页面

## Tagbar
显示类/方法/变量
* **,t** 打开Tagbar
* **ctrl + w + ←/→** 窗口切换
* **F1** 打开/关闭帮助
* **+/-** 展开/折叠索引
* **回车/p** 转到索引处/转到索引处，光标仍在Tagbar

## vim-plug
插件管理器

## YouCompleteMe
基于语义的自动补全插件，支持C/C++、C#、Python、PHP等语言

## NerdTree
代码资源管理器
* **,n** 打开代码资源管理器
* **回车** 打开该文件

## vim-nerdtree-syntax-highlight
NerdTree文件类型高亮

## nerdtree-git-plugin
NerdTree显示git状态

## vim-devicons
显示文件类型图标

## Airline
可以取代powerline的状态栏美化插件

## auto-pairs
自动补全引号、圆括号、花括号等

## LeaderF
比ctrlp更强大的文件的模糊搜索工具
* **,bt** 打开变量函数类搜索
* **,fl** 打开当前文件夹文件搜索

## ack
强大的文本搜索工具 - 搜索当前文件下(递归)所有文件关键词
* **,F** 在当前文件夹下搜索所有文件中的关键词

## vim-surround
自动增加、替换配对符的插件

## vim-commentary
快速注释代码插件
* **gcc** 注释当前行代码
* **gc** 使用可视化模式下选中代码以将选中的代码进行注释
* **gcgc** 取消注释

## vim-repeat
重复上一次操作

## vim-endwise
if/end/endif/endfunction补全

## tabular
代码、注释、表格对齐

## vim-easymotion
强大的光标快速移动工具，强大到颠覆你的插件观
* **,w** 输入标签后将快速定位到某个地方

## incsearch.vim
模糊字符搜索插件
* **/** 搜索指定值，按下回车定位到当前光标之后的值
* **?** 搜索指定值，按下回车定位到当前光标之前的值

## vim-fugitive
集成Git

## gv
显示git提交记录

## vim-slash
优化搜索，移动光标后清除高亮

## echodoc
补全函数时在命令栏显示函数签名

## vim-smooth-scroll
让翻页更顺畅

## clever-f.vim
强化f和F键

## 补充按键
* **,u** 转到函数声明/变量声明
* **,o** 打开include文件
* **,ff** 语法错误自动修复
* **F5** 显示语法错误提示窗口

## 基本操作
##### 插入模式
* **i** 在光标处进入插入模式
* **:e <filename> / open <filename>** 另起一个buffer打开文件
* **复制 + 粘贴** V进入可视化模式后y复制多行p粘贴

##### 窗口切分
* **sp <filename>** 横向切分窗口并打开文件
* **vsp <filename>** 竖向切分窗口并打开文件
* **ctrl + w + c** 关闭当前窗口
* **ctrl + w + o** 关闭其他窗口

##### 文件编辑
* **dd** 删除当前行
* **u** 撤销上一次操作


