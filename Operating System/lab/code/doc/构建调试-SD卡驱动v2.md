# SD卡驱动
retrhelo <artyomliu@qq.com>

## 前言
SD卡驱动是xv6-k210项目文件系统开发的一环。在设计中，我们希望Fat32文件系统能够保存在SD卡这样的外存上，而不是通过类似与ramdisk的方式保存在内存中。为此，操作系统需要SD卡驱动来实现对SD卡上数据访存。

早在2020年年末的时候，[hustccc](https://github.com/SKTT1Ryze) 同学就试图通过移植Kendryte的官方代码到xv6-k210项目中来实现SD卡驱动。但所移植的官方代码并不稳定，在读写SD的过程中不时地会出现未知的错误。同时，官方代码与[SD Spec](https://www.sdcard.org/downloads/pls/)中定义的SD卡驱动流程有着大量的出入，同时官方代码的码风也实在让人不敢恭维。因此出于种种考虑，我最终决定重新按照[SD Spec](https://www.sdcard.org/downloads/pls/)中的描述重新编写SD卡驱动。

## 1. SD卡驱动的编写
### 1.1 SD卡驱动的基础
出于简单考虑，选择通过SPI协议完成host与SD卡之间的数据通信。SPI协议只需要四个引脚就可以实现，且工作机制简单易懂。Kendryte官方为我们实现K210下的SPI通信协议（主要表现为`kernel/spi.c`文件）。故我们可以直接使用该代码来完成与通信。

关于SPI协议的相关细节可以参考[_SPI通信协议详解_](https://zhuanlan.zhihu.com/p/150121520)这篇文章。

在解决了通信协议的问题之后，我们便可以开始着手SD卡驱动的开发了。作为参考，SD Association给出的文档_Part1\_Physical\_Layer\_Simplified\_Specification\_Ver8.00.pdf_（以下简称“文档”）的第7章_SPI Mode_中对此有着详细的描述。感兴趣的读者可以自行查阅文档。

上位机对SD卡的操作是通过发送CMD指令来完成的。在SPI模式下，一条完整的CMD指令包含指令序号（Command Index）、指令参数（Argument）以及CRC7校验位。而为了简单起见，大部分时候我们都将使用CMD _X_来代指某一条指令，其中_X_为该指令的序号。CMD指令的格式如下图所示。同时，在SPI模式下，发送任何一条指令都必然能够收到回复（Response），可以通过回复内容判断指令的执行情况。关于CMD指令和回复的详细内容详见文档7.3节。

![](../img/sd_spi_cmd_format.png)

### 1.2 SD卡的初始化
选择在SPI模式下对SD卡进行初始化。文档在7.2.1小节“_Mode Selection and Initialization_”中详细地描述了这一过程。其大致流程如下图所示
![](../img/sd_spi_init.png)
上图中的操作可大致分为如下几个部分：
1. 让SD卡进入SPI模式。这一操作是通过发送CMD0指令来完成的。
2. 确认SD卡的“_Interface Operation Condition_”。该操作通过CMD8指令来完成。通过发送该指令，Host会确认其所提供的工作电压是否满足SD卡的要求。需要注意的是，CMD8并不总是能收到SD卡的回复。能否收到回复取决于SD卡的版本。
3. 进一步确定SD卡的工作电压区间满足要求。这是通过CMD58指令实现的。该指令的回复为SD卡OCR寄存器中的内容（如下图所示）。这里我们主要检查VDD Voltage Window位段。
![](../img/sd_spi_ocr.png)
4. 使SD卡脱离IDLE状态。这是通过发送ACMD41指令完成的。需要注意的是，尽管流程图中没有画出来，但是在发送所有的ACMD指令前需要额外地发送一条CMD55指令。CMD55指令的作用是告诉SD其所接收到的下一条指令会是一条ACMD指令（A是Application的意思）。另外，因为ACMD41指令会让SD卡脱离IDLE状态，所以在ACMD41之后的所有指令所收到的R1 Response中的idle位都不再应该为1（在此之前idle位应该是置1的）。
5. 确定SD卡的Capacity类型。这同样是通过CMD58来完成的。通过OCR寄存器的CCS位段，可以知道当前SD卡是Standard Capacity还是SDHC或SDXC（通常容量小于2GB的SD卡为Standard Capacity，大于2GB的为SDHC/SDXC）。我们需要知道SD卡Capacity类型主要是出于SD卡读写的考虑——Standard Capacity类型的卡与SDHC/SDXC类型的卡在寻址时有所区别。前者使用字节作为地址的基本单位，而后者使用块（Block，SDHC/SDXC规定一个Block的大小为512B）作为基本单位。

### 1.3 SD卡的读操作
SD卡的读操作主要是通过CMD17（读取单个Block）和CMD18（读取多个连续Block）来完成的。为了简单起见在代码中我们仅实现了CMD17读取单个Block的操作。对于多个Block的读操作读者可参考文档7.2.3节自行实现。

![](../img/sd_spi_read_single_block.png)

上图描述了SD卡进行读操作时SPI的MOSI（Master Out Slave In，即上位机的数据输出端口）和MISO（Master In Slave Out，即上位机的数据输入端口）的数据时序情况。可以看到，在CMD17指令发送后，上位机会首先收到对应的Response，然后才开始接收数据。这里有几处细节在图中并未表现，但需要注意
1. 在CMD17的参数是所需要读取的Data Block的起始地址。如1.2节所言，该地址的单位会因SD卡的类型而不同。对于Standard Capacity的卡来说，其地址是以字节为单位的。同时，对于Block Size为512B的设置来说，该地址需要保证512B对齐。
2. Response与Data Block两段数据之间并不是连续的，其间往往会混在有大量的垃圾数据。在收到Response后，会有一个长度为1Byte的'Start Token'（其值为0xfe）来提示接下来的数据属于Data Block。
3. Data Block的长度在Standard Capacity类型的卡中由SD卡的Block Size决定，其可以通过CMD16设定。而在SDHC/SDXC的卡中则强制为512B。而紧随Data Block的CRC数据段的长度则为两个字节。

### 1.4 SD卡的写操作
SD卡的写操作与读操作类似。上位机可以通过CMD24/CMD25来进行单个/多个连续数据块的写入操作。出于简单起见，代码中同样也只实现了写入单个数据块的操作，读者可自行参考文档的7.2.4节实现对于多个连续数据块的写操作。。操作时MOSI和MISO的数据时序情况如下。

![](../img/sd_spi_write_single_block.png)

有几点需要注意：
1. 该图是从SD卡的角度出发的。DataIn与DataOut均是相对于SD卡而言的。
2. CMD24的参数同样也是需要写入的起始地址，其要求与CMD17一致。
3. 在上位机发送完Data Block中的数据后，需要发送一段长度为两个字节的CRC。但SD卡并不会根据CRC对所收到的数据进行校验。
4. 在SD卡完成Data Block的接收后，SD卡才会通过DataOut发送一个字节的'Data Response Token'（该token的取值详见文档7.3.3.1节）。然后SD卡会花费一段时间处理上位机发送的数据。在此期间内SD卡会将DataOut端口置为低电平，直至数据处理完成。
5. 在确认SD卡处理完成后，应当发送CMD13确认SD卡的处理结果。CMD13所收到的回复格式如下
![](../img/sd_spi_cmd13_response.png)

## 尾声
本来在最开始是没打算自己编写SD驱动的，毕竟重复造轮子不可取（笑）。但是奈何使用Kendryte的代码总能跑出奇怪的BUG，就算定位了问题也不知道怎么修改，就索性找到SD Association提供的Specification自己实现一个了。因为是自己编写的，知根知底，日后维护起来也会轻松一些。

在这里我要感谢[hustccc](https://github.com/SKTT1Ryze)同学在此前所做的大量的代码移植工作，代码中SPI协议的部分均来自于他的工作。同时我也要感谢[AtomHeartCoder](https://github.com/AtomHeartCoder)同学，他为xv6-k210系统编写了对Fat32文件系统的相关支持。他的工作为我所编写的代码提供了大量的测试，同时他本人也在代码的编写过程中提多次指出代码中所存在的问题。

最后，受制于时间有限、笔者能力有限等因素，代码以及文档中不可避免地会存在疏漏，欢迎各位读者指正。