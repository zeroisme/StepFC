﻿#pragma once
// License: MIT  http://opensource.org/licenses/MIT
// Author: dustpg   mailto:dustpg@gmail.com

#include "sfc_rom.h"
#include "sfc_code.h"
#include "sfc_cpu.h"
#include "sfc_ppu.h"

// typedef
struct sfc_famicom;
typedef struct sfc_famicom sfc_famicom_t;

/// <summary>
/// The SFC disassembly buf length
/// </summary>
enum { SFC_DISASSEMBLY_BUF_LEN2 = 48 };

// 指定地方反汇编
void sfc_fc_disassembly(
    uint16_t address,
    const sfc_famicom_t* famicom,
    char buf[SFC_DISASSEMBLY_BUF_LEN2]
);

/// <summary>
/// StepFC: 扩展接口
/// </summary>
typedef struct {
    // ROM 加载器读取信息
    sfc_ecode(*load_rom)(void*, sfc_rom_info_t*);
    // ROM 加载器卸载
    sfc_ecode(*free_rom)(void*, sfc_rom_info_t*);
    // 执行指令前
    void(*before_execute)(void*, sfc_famicom_t*);

} sfc_interface_t;


/// <summary>
/// StepFC: Mapper接口
/// </summary>
typedef struct {
    // Mapper 重置
    sfc_ecode(*reset)(sfc_famicom_t*);

} sfc_mapper_t;


/// <summary>
/// FC 模拟器主体
/// </summary>
struct sfc_famicom {
    // 参数
    void*               argument;
    // 扩展接口
    sfc_interface_t     interfaces;
    // Mapper接口
    sfc_mapper_t        mapper;
    // 寄存器
    sfc_cpu_register_t  registers;
    // PPU
    sfc_ppu_t           ppu;
    // ROM 信息
    sfc_rom_info_t      rom_info;
    // 程序内存仓库(Bank)/窗口(Window)
    uint8_t*            prg_banks[0x10000 >> 13];
    // 工作(work)/保存(save)内存
    uint8_t             save_memory[8 * 1024];
    // 显存
    uint8_t             video_memory[2 * 1024];
    // 4屏用额外显存
    uint8_t             video_memory_ex[2 * 1024];
    // 主内存
    uint8_t             main_memory[2 * 1024];
};


// 初始化
sfc_ecode sfc_famicom_init(
    sfc_famicom_t* famicom,
    void* argument,
    const sfc_interface_t* interfaces
);

// 重置
sfc_ecode sfc_famicom_reset(sfc_famicom_t* famicom);

// 反初始化
void sfc_famicom_uninit(sfc_famicom_t*);




// ----------------------- 内部函数

// 开始垂直空白标记
extern inline void sfc_vblank_flag_start(sfc_famicom_t* famicom);
// 结束垂直空白标记
extern inline void sfc_vblank_flag_end(sfc_famicom_t* famicom);

// 不可屏蔽中断 垂直空白
void sfc_do_vblank(sfc_famicom_t* famicom);
