#ifndef GB_REGISTERS_H
#define GB_REGISTERS_H

// PAGES ARE DEFINED WITH REFERENCE TO THE GAME BOY PROGRAMMING MANUAL

// DMG -> monochrome gameboy
// CGB -> color game boy

#define MEMORY_SIZE 0x10000

//==============================================================================
// General Memory Map
//==============================================================================
#define ROM_DATA_AREA         0x0100
#define PROGRAM_START_ADDRESS 0x0150

#define EXTERNAL_WORKING_RAM 0xA000
#define INTERNAL_WORKING_RAM 0xC000

#define CGB_SWITCHABLE_BANK 0xD000

#define HIGH_RAM_START 0xFF00
#define STACK_RAM 0xFF80

//==============================================================================
// Port/Mode registers
//==============================================================================
#define P1 0xFF00
#define SB 0xFF01
#define SC 0xFF02

#define DIV  0xFF04
#define TIMA 0xFF05
#define TMA  0xFF06
#define TAC  0xFF07

// CGB only
#define KEY1 0xFF4D
#define RP   0xFF56

//==============================================================================
// Bank control registers (CGB only)
//==============================================================================
#define  VBK 0xFF4F
#define SVBK 0xFF70

//==============================================================================
// Interrupt flags
//==============================================================================
#define IF 0xFF0F
#define IE 0xFFFF

//==============================================================================
// Display Ram definitions
//==============================================================================
#define DISPLAY_RAM_START 0x8000
#define DISPLAY_RAM_STOP 0x9FFF
#define DISPLAY_RAM_BANK_SIZE DISPLAY_RAM_STOP - DISPLAY_RAM_START + 1
#define DISPLAY_RAM DISPLAY_RAM_START

#define OBJ_CHR_DATA      0x8000
#define BG_CHR_DATA       0x9000
#define BG_DISPLAY_1_DATA 0x9800
#define BG_DISPLAY_2_DATA 0x9C00

// Offsets from the start of the display ram to the respective data
#define OBJ_CHR_DATA_OS 0x0000
#define BG_CHR_DATA_OS  0x1000
#define BG_DISPLAY_1_DATA_OS 0x1800
#define BG_DISPLAY_2_DATA_OS 0x1C00

// (?) Object Access Memory
#define OAM 0xFE00
#define OAM_STOP 0xFE9F
#define OAM_SIZE OAM - OAM_STOP + 1

//==============================================================================
// LCD control registers (Page 18)
//==============================================================================
#define VIDEO_CONTROL_REGISTERS_START 0xFF40
#define LCDC 0xFF40
#define STAT 0xFF41
#define SCY  0xFF42
#define SCX  0xFF43
#define LY   0xFF44
#define LYC  0xFF45
#define DMA  0xFF46
#define BGP  0xFF47
#define OBP0 0xFF48
#define OBP1 0xFF49
#define WY   0xFF4A
#define WX   0xFF4B

// CGB LCD only
#define HDMA1 0xFF51
#define HDMA2 0xFF52
#define HDMA3 0xFF53
#define HDMA4 0xFF54
#define HDMA5 0xFF55

#define BCPS  0xFF68
#define BCPD  0xFF69
#define OCPS  0xFF6A
#define OCPD  0xFF6B

//==============================================================================
// Sound Registers
//==============================================================================
#define NR_XX        0xFF10
#define WAVEFORM_RAM 0xFF30

#endif //GB_REGISTERS_H
