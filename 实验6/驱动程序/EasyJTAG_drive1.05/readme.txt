    				应用简介

1.  安装
    首先，将EasyJTAG_drive目录下的所有文件复制到ADS的BIN目录，如C:\Program Files\ARM\ADSv1_2\BIN。    
    然后，进入AXD调试环境，打开"Options->Configure  Target..."，弹出"Choose Target"窗口，点击"ADD"添加仿真器的驱动程序，在添加文件窗口选择如C:\Program Files\ARM\ADSv1_2\BIN目录下的EasyJTAG.dll，"打开"即可。

    若在添加文件窗口中没有显示DLL文件，请设置"文件夹选项(O)..."，将查看页中的"隐藏文件"项选用"显示所有文件"，即可。

2.  使用
    在AXD调试环境, 打开"Options->Configure Target..."，弹出"Choose Target"窗口，在"Target Environments"框中选择"EasyJTAG..."项。
    点击"Configure"按钮，进入"EasyARM Configuration"设置窗口，在"ARMcore"项中选择相应的CPU型号。

    在进入AXD调试环境，有时会弹出"Fatal AXD Error"窗口，此时可以点击"Connect mode..."，然后选择"ATTACH ..."项确定，再点击"Restart"即可。接下来就可以使用"File->Load Image..."加载调试文件进行JTAG调试。

3.  其它
    本版EasyJTAG.dll目前只能在ADS软件中使用。

