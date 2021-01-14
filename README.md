Jetson nano QT图形化界面烧录工具    
-------------2021-01-14---------------    
Release Version:V3.0.0    
1.添加导入文件系统镜像的功能,可以将.raw的文件系统镜像挂载到rootfs目录下    
    
1.Add filesystem load function,can mount .raw under rootfs/    
-------------2020-11-22---------------    
Release Version:V2.0.0
1.添加Jetson nx的支持包括nx emmc以及nx sdcard版本    
2.暂时不支持Jetson nx单独烧录内核的功能。    

1.Add Jetson nx support including nx emmc and nx sdcard version    
2.It does not support Jetson nx to burn the kernel separately.    

-------------2020-10-12---------------    
Release Version:V1.0.0    
    
1.运行环境 Ubuntu18.04 amd64    
    
2.支持烧录的板卡     
Jetson nano B01 EMMC    
Jetson nano B01 SDCARD    
    
3.使用说明    
需要把Nvidia_flasher放置到Nvidia的烧录目录下即Linux_for_Tegra/执行(not root不需要使用root权限)    
此目录下存在flash.sh，同时使用过SDKmanager烧录过板卡后即可使用Nvidia_flasher    
    
4.注意    
不要修改软件名称(即不要重命名Nvidia_flasher)    
需要qt5支持    
即需要sudo apt-get install qt5-default    

    I use qt5 write a program for debugging kernel or produce.     
    
    I use in Ubuntu18.04 amd64.    
    Can flash for Jetson nano B01 EMMC or Jetson nano B01 SDCARD,buy usb.    

    so need sudo apt-get install qt5-default.    

    and put my program into path Linux_for_Tegra/    
    In this path need have flash,sh and also need SDKManager to flash board at least once.    
    So you can run Nvidia_flasher.    

   Attention ：    
   It also have some bugs,but General function is OK.    
   AND DO NOT RENAME Nvidia_flasher.    
  It do not root.only input passwd.     


-------------2020-10-10-------------    
大体上Jetson nano emmc版本可以正常使用。“\r”解析还是有点问题，不过不影响    

