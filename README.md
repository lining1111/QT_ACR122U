# QT_ACR122U
读卡器ACR122U操作软件，基于QT界面的ACR122U读卡器操作软件

## 目录分布说明
 acr122u_linux_lib 是官网下载的acsccid的库，以及使用说明 地址 
 https://www.acs.com.hk/cn/driver/73/acr122u-nfc%E8%AF%BB%E5%86%99%E5%99%A8-usb%E6%8E%A5%E5%8F%A3/
## acr122u pc/sc
 需要完成 libusb libpcsc-lite libccid以及acs官方的ascccid的安装，应用程序的依赖库是libpcsclite，
 服务则是pcscd,API的参考是官网的API-ACR122U-CN-2.04.pdf。这些资料都在acr122u_linux_lib文件夹内。
 
奇怪的地方是，系统自启动的pcscd服务对应用和pcsc_scan命令并不有好，需要进行以下操作

sudo killall pcscd

sudo pcscd -fd

重新启动服务，然后才起作用

对于上面问题的解决方法是，应该是系统apt安装了pcscd，和源码安装的那个冲突了，需要卸载apt安装的pcscd

sudo apt remove pcscd

将pcsclite源码安装到系统，这时pcscd服务应该有些问题，需要操作

sudo systemctl unmask pcscd.service

sudo systemctl unmask pcscd.socket

到这里旧的服务痕迹就修理完毕，重新是能服务就好

sudo systemctl enable pcscd.service

终端测试pscsd服务 pscs_scan 如果有读卡器连接就有显示了，同时应用程序也正常了

##20210706
完成pcsc uuid nfc环境的搭建和测试，准备通过nfc读取m1卡信息。(其实在nfc上面还有一层，就是freefare，专门用来读取m1卡的库)
但是，介于后面卡的多样性扩展需要，暂时在nfc层进行读写。

工程中 nfc文件夹下，NFC_Operate是主要调用类，其他文件都是从libnfc库源文件的utils文件夹下拷贝的，其实命令行中
nfc-list就是utils下编译出来的。**所以说优秀的基础类程序，既有完整的例子，还的有可执行的程序做测试，这点很重要**