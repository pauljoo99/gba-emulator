//
//  MemoryAccessor.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/17/25.
//

import Foundation

let BIOS_SIZE = 0x4000
let WRAM_ONBOARD_SIZE = 0x40000
let WRAM_ONCHIP_SIZE = 0x8000
let IO_REGISTERS_SIZE = 0x1FF
let PALETTE_RAM_SIZE = 0x400
let VRAM_SIZE = 0x18000
let OAM_SIZE = 0x400
let GAMEPAK_WS_SIZE = 0x2000000
let GAMEPAK_SRAM_SIZE = 0x10000
let MYSTERY_ADDR_SIZE = 1

let TOTAL_MEMORY_SIZE = BIOS_SIZE + WRAM_ONBOARD_SIZE + WRAM_ONCHIP_SIZE + IO_REGISTERS_SIZE + PALETTE_RAM_SIZE + VRAM_SIZE + OAM_SIZE + 3 * GAMEPAK_WS_SIZE + GAMEPAK_SRAM_SIZE

func GetHalfWordMemoryPtr(handle: CpuRunnerHandle, address: UInt32) -> UnsafeMutablePointer<UInt16> {
    return UnsafeMutableRawPointer(GetByteMemoryPtr(handle: handle, address: address)).bindMemory(to: UInt16.self, capacity: TOTAL_MEMORY_SIZE / 2)
}

func GetWordMemoryPtr(handle: CpuRunnerHandle, address: UInt32) -> UnsafeMutablePointer<UInt32> {
    return UnsafeMutableRawPointer(GetByteMemoryPtr(handle: handle, address: address)).bindMemory(to: UInt32.self, capacity: TOTAL_MEMORY_SIZE / 4)
}

func GetByteMemoryPtr(handle: CpuRunnerHandle, address: UInt32) -> UnsafeMutablePointer<UInt8> {
    guard let basePtr = CpuRunner_GetMemory(handle) else {
        fatalError("Bad Access")
    }

    let totalMemorySize = TOTAL_MEMORY_SIZE
    let memPtr = basePtr.bindMemory(to: UInt8.self, capacity: totalMemorySize) // Bind as UInt8 pointer

    switch address {
    case 0x00000000...0x00003FFF:
        // BIOS
        return memPtr.advanced(by: Int(address - 0x00000000))

    case 0x02000000...0x0203FFFF:
        // WRAM OnBoard
        return memPtr.advanced(by: BIOS_SIZE + Int(address - 0x02000000))

    case 0x03000000...0x03007FFF:
        // WRAM OnChip
        return memPtr.advanced(by: BIOS_SIZE + WRAM_ONBOARD_SIZE + Int(address - 0x03000000))

    case 0x04000000...0x040001FE:
        // IO Registers
        return memPtr.advanced(by: BIOS_SIZE + WRAM_ONBOARD_SIZE + WRAM_ONCHIP_SIZE + Int(address - 0x04000000))

    case 0x05000000...0x050001FF:
        // Palette RAM
        return memPtr.advanced(by: BIOS_SIZE + WRAM_ONBOARD_SIZE + WRAM_ONCHIP_SIZE + IO_REGISTERS_SIZE + Int(address - 0x05000000))

    case 0x06000000...0x0600BFFF:
        // VRAM
        return memPtr.advanced(by: BIOS_SIZE + WRAM_ONBOARD_SIZE + WRAM_ONCHIP_SIZE + IO_REGISTERS_SIZE + PALETTE_RAM_SIZE + Int(address - 0x06000000))

    case 0x07000000...0x070001FF:
        // OAM
        return memPtr.advanced(by: BIOS_SIZE + WRAM_ONBOARD_SIZE + WRAM_ONCHIP_SIZE + IO_REGISTERS_SIZE + PALETTE_RAM_SIZE + VRAM_SIZE + Int(address - 0x07000000))

    case 0x08000000...0x09FFFFFF:
        // GamePak WS0
        return memPtr.advanced(by: BIOS_SIZE + WRAM_ONBOARD_SIZE + WRAM_ONCHIP_SIZE + IO_REGISTERS_SIZE + PALETTE_RAM_SIZE + VRAM_SIZE + OAM_SIZE + Int(address - 0x08000000))

    case 0x0A000000...0x0BFFFFFF:
        // GamePak WS1
        return memPtr.advanced(by: BIOS_SIZE + WRAM_ONBOARD_SIZE + WRAM_ONCHIP_SIZE + IO_REGISTERS_SIZE + PALETTE_RAM_SIZE + VRAM_SIZE + OAM_SIZE + GAMEPAK_WS_SIZE + Int(address - 0x0A000000))

    case 0x0C000000...0x0DFFFFFF:
        // GamePak WS2
        return memPtr.advanced(by: BIOS_SIZE + WRAM_ONBOARD_SIZE + WRAM_ONCHIP_SIZE + IO_REGISTERS_SIZE + PALETTE_RAM_SIZE + VRAM_SIZE + OAM_SIZE + 2 * GAMEPAK_WS_SIZE + Int(address - 0x0C000000))

    case 0x0E000000...0x0E00FFFF:
        // GamePak SRAM
        return memPtr.advanced(by: BIOS_SIZE + WRAM_ONBOARD_SIZE + WRAM_ONCHIP_SIZE + IO_REGISTERS_SIZE + PALETTE_RAM_SIZE + VRAM_SIZE + OAM_SIZE + 3 * GAMEPAK_WS_SIZE + Int(address - 0x0E000000))

    case 0x04000410:
        // Mystery address maps after all above
        return memPtr.advanced(by: BIOS_SIZE + WRAM_ONBOARD_SIZE + WRAM_ONCHIP_SIZE + IO_REGISTERS_SIZE + PALETTE_RAM_SIZE + VRAM_SIZE + OAM_SIZE + 3 * GAMEPAK_WS_SIZE + GAMEPAK_SRAM_SIZE)

    default:
        fatalError("Bad Access")
    }
}

func CreateOam(
    x : UInt16,
    y : UInt16,
    shape : UInt16,
    size : UInt16,
    tile_id : UInt16) -> UInt64
{
    let attr0 : UInt64 = UInt64(((shape & 0b11) << 14) | (y & (0xFF)))
    let attr1 : UInt64 = UInt64(((size & 0b11) << 14) | (x & (0x1FF)))
    let attr2 : UInt64 = UInt64(tile_id & (0x3FF))
    return (attr2 << 32) | (attr1 << 16) | (attr0)
}

extension Oam {
    var x: UInt16 {
        return attr1 & 0x1FF
    }

    var y: UInt16 {
        return attr0 & 0xFF
    }

    var shape: UInt16 {
        return attr0 >> 14
    }

    var size: UInt16 {
        return attr1 >> 14
    }

    var tid: UInt16 {
        return attr2 & 0x3FF
    }

    var mode: UInt16 {
        return (attr0 >> 8) & 0b11
    }

    var widthPx: UInt16 {
        switch (shape, size) {
        case (0b00, 0b00): return 8
        case (0b01, 0b00): return 16
        case (0b10, 0b00): return 8
        case (0b00, 0b01): return 16
        case (0b01, 0b01): return 32
        case (0b10, 0b01): return 8
        case (0b00, 0b10): return 32
        case (0b01, 0b10): return 32
        case (0b10, 0b10): return 16
        case (0b00, 0b11): return 64
        case (0b01, 0b11): return 64
        case (0b10, 0b11): return 32
        default: return 0
        }
    }

    var heightPx: UInt16 {
        switch (shape, size) {
        case (0b00, 0b00): return 8
        case (0b01, 0b00): return 8
        case (0b10, 0b00): return 16
        case (0b00, 0b01): return 16
        case (0b01, 0b01): return 8
        case (0b10, 0b01): return 32
        case (0b00, 0b10): return 32
        case (0b01, 0b10): return 16
        case (0b10, 0b10): return 32
        case (0b00, 0b11): return 64
        case (0b01, 0b11): return 32
        case (0b10, 0b11): return 64
        default: return 0
        }
    }
}
