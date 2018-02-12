# lizhiFM

- **声明：**

本软件基于Qt 5.10编写

FM音频资源全部引用自荔枝FM网页版，本软件不具有FM音频资源版权

目的仅为学习和交流，以及方便更多的荔枝FM的爱好者收听节目

本软件请勿用于任何商业行为

本软件部分素材来自互联网，有任何侵权行为请发送邮件到：itoday_06#qq.com （将#换为@）

- 关于捐赠：

![image](https://github.com/gcmwhite/lizhiFM/blob/master/printscreen/2018-02-12%2013-34-41%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE.png)

本次增加了捐赠的二维码，支持支付宝和微信捐赠

手中有余的朋友可以使用这两种方式进行捐赠

囊中羞涩的朋友可以使用支付宝扫码领取赏金，领取并使用赏金后我将获得一部分的赏金，这也算一种间接的支持吧。

支付宝赏金每天可领取一次，第六期截止时间是2018年2月31日，大家尽快哦！

二维码顺序：支付宝、微信赞赏、支付宝赏金

- 下载地址：[https://pan.baidu.com/s/1nw6Vzcd ](https://pan.baidu.com/s/1nw6Vzcd )密码: 3f4m

- 安装

若下载为Appimage格式的，赋予可执行权限，终端打开

```
./荔枝FM_1.1-x86_64.AppImage
```

若下载为tar.bz2格式的，直接解压出来放在你要安装的目录，赋予lizhifm文件可执行权限，双击应该就可以运行了

运行creatDesktop.sh创建桌面快捷方式

Appimage格式请自行创建桌面文件



- 荔枝FM客户端 1.1
![image](https://github.com/gcmwhite/lizhiFM/blob/master/printscreen/2018-02-12%2013-33-33%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE.png)
更新日期：2018年2月11日

- 本次更新内容：

优化界面风格，添加了8款皮肤背景，并且可以设置自定义背景

添加了皮肤、github、设置、捐赠、关于按钮，其中设置项不可用

保存播放列表、进度等信息，不用每次打开都重新开始

不要问为什么没有搜索按钮，因为官方主页就没有搜索功能

- 可能会出现的问题：
1. ++有时候播放的时候会突然没声音++

网络原因，等待加载。或先暂停一段时间再运行，或直接拖动进度条

若使用终端打开的查看终端输出

![image](https://github.com/gcmwhite/lizhiFM/blob/master/printscreen/2018-02-12%2013-23-56%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE.png)

发现buffer一直处于几乎为0的状态，说明网络不通或正在加载，请耐心等待

![image](https://github.com/gcmwhite/lizhiFM/blob/master/printscreen/2018-02-12%2013-24-22%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE.png)

如图等缓冲加载到100的时候才可以正常播放

2. ++软件无法正常启动++

报告qt_5_10 not foun错误，没有安装qt运行库，安装qt运行库就行了

3. ++软件可以启动但是没有声音++

没有安装解码器，安装通用解码器即可

4. ++Linux Deepin重新打开应用无法恢复进度条位置++

已知问题，正在调整

- 程序部分截图：

主界面
![image](https://github.com/gcmwhite/lizhiFM/blob/master/printscreen/2018-02-12%2013-33-33%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE.png)

分类列表
![image](https://github.com/gcmwhite/lizhiFM/blob/master/printscreen/2018-02-12%2013-33-44%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE.png)

皮肤设置
![image](https://github.com/gcmwhite/lizhiFM/blob/master/printscreen/2018-02-12%2013-33-59%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE.png)


- 荔枝FM客户端 1.0

修复了一些BUG，优化启动速度。

增加了进度条的拖动

重新设计了界面逻辑

声音初始化为100，默认使用系统音量调节，就不单独设置音量调节器了

版本号为1.0

软件以单独打包为appimage格式

百度云下载地址：[https://pan.baidu.com/s/1nw6Vzcd](https://pan.baidu.com/s/1nw6Vzcd) 密码: 3f4m

下载文件 荔枝FM_7-x86_64.AppImage

赋予文件可执行权限

运行程序终端

./M_7-x86_64.AppImage



