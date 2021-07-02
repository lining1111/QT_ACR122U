# QT_ACR122U
读卡器ACR122U操作软件，基于QT界面的ACR122U读卡器操作软件

## 目录分布说明
 acr122u_linux_lib 是官网下载的linux(x86_64，i386)的库，以及使用说明 地址 
 https://www.acs.com.hk/cn/driver/73/acr122u-nfc%E8%AF%BB%E5%86%99%E5%99%A8-usb%E6%8E%A5%E5%8F%A3/
 很遗憾只有x86的库，没有arm的，毕竟sdk当初设计应该是为界面服务的。
## acr122u pc/sc
 需要完成 libusb libpcsc-lite libccid以及acs官方的ascccid的安装，应用程序的依赖库是libpcsclite，
 服务则是pcscd,API的参考是官网的API-ACR122U-CN-2.04.pdf。这些资料都在acr122u_linux_lib文件夹内。
 
奇怪的地方是，系统自启动的pcscd服务对应用和pcsc_scan命令并不有好，需要进行以下操作

sudo killall pcscd

sudo pcscd -fd

重新启动服务，然后才起作用