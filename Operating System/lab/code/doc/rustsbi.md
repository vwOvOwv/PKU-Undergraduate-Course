# xv6-k210 移植记录 RustSBI 专题

## 前言
移植 `xv6-k210` 的很大一部分工作都是依赖于 `RustSBI` 这一强大的项目，本文将对 `RustSBI` 作一些介绍。  

## 什么是 SBI

SBI 是一个 RISC-V 的标准，在 RISC-V 相关文档中，SBI 是在物理硬件和操作系统的中间层，运行在 M 特权级。（操作系统内核运行在 S 特权级）  
同时 SBI 还有以下特征：  
> Ref: https://github.com/luojia65/DailySchedule/blob/master/2020-slides/RustSBI%E7%9A%84%E8%AE%BE%E8%AE%A1%E4%B8%8E%E5%AE%9E%E7%8E%B0.pdf  

+ 引导程序
    - 开发者对这一类软件的第一印象。它将引导启动操作系统，和 UEFI 比较相似
+ 统一的硬件环境
    - RISC-V 架构中，常驻后台的固件，为操作系统提供一系列接口，以便其获取和操作硬件信息。常驻后台是 SBI 和其他引导程序相比，有特点的地方
    - 这些统一的接口被称为“监管模式二进制接口”，即 `SBI`
+ RISC-V SBI 标准
    - RISC-V 的 SBI 标准规定了一系列接口，包括重启，跨核软中断，设备树等功能模块。目前的设计目标大致是支持类 Unix 系统的启动与运行。

## SBI 的功能和组成部分
> Ref: https://github.com/luojia65/DailySchedule/blob/master/2020-slides/RustSBI%E7%9A%84%E8%AE%BE%E8%AE%A1%E4%B8%8E%E5%AE%9E%E7%8E%B0.pdf  

+ 硬件运行时
    - 处理中断和异常。厂家专有的中断处理器，也需要 SBI 实现适配和处理。SBI 实现将委托一部分的异常到操作系统，这些异常包括缺页等等
+ 硬件环境接口
    - 和系统调用类似，`ecall` 指令触发异常而陷入内核，由此实现操作系统向环境的调用
+ 引导启动模块
    - 初始化各个 RISC-V 寄存器，最终使用 `mret` 指令下降到 S 特权级，启动操作系统
+ 兼容模块
    - 捕捉部分新版本指令，使用旧版本指令模拟它，从而达到运行新版程序的目的

## SBI 的作用
> Ref: https://github.com/luojia65/DailySchedule/blob/master/2020-slides/RustSBI%E7%9A%84%E8%AE%BE%E8%AE%A1%E4%B8%8E%E5%AE%9E%E7%8E%B0.pdf  

+ 模块化
    - SBI 是一个标准，它有多个实现。根据用户的选择，可以自己搭配 SBI 实现使用
    - 不同的 SBI 实现可能拥有不同的功能
    - 并发操作系统时，可以通过更换 SBI 实现，来排除可能的故障和问题
+ 兼容性
    - 只需要更新 SBI 实现，就能支持新版本标准的操作系统。在一段时间内，无需更换硬件。  
    - 可以通过 SBI 软件，模拟硬件上暂未实现的指令集
    - 延长了 RISC-V 硬件的生命周期

## RustSBI
RustSBI 是 RISC-V SBI 标准的一个 Rust 语言实现，目前已经发布 0.1 版本[v1.0](https://crates.io/crates/rustsbi)  
> Ref: https://github.com/luojia65/DailySchedule/blob/master/2020-slides/RustSBI%E7%9A%84%E8%AE%BE%E8%AE%A1%E4%B8%8E%E5%AE%9E%E7%8E%B0.pdf  

+ 适配 RISC-V SBI 规范 v0.2 版本
+ 对类 Unix 系统有非常好的支持
+ 完全使用 Rust 语言实现
+ 具备 `OpenSBI` 的大部分功能
+ 支持 `QEMU` 模拟器，它使用 RISC-V 特权级版本 v1.11
+ 向下兼容 RISC-V 特权级版本 v1.9
+ 支持 K210 芯片，包含内存管理单元和 S 模式

## RustSBI 对 xv6-k210 提供的支持

## RustSBI 的设计

## RustSBI 进入国际 RISC-V SBI 标准

