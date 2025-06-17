//
//  RegisterDecoders.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/16/25.
//

import Foundation

/// Mirror of the GBA BGxCNT register
struct BGControl {
    private var raw: UInt16

    init(raw: UInt16 = 0) {
        self.raw = raw
    }

    /// Bits 0–1: Priority (0 = highest … 3 = lowest)
    var priority: UInt8 {
        get { UInt8(raw & 0b11) }
        set {
            raw = (raw & ~0b11) | UInt16(newValue & 0b11)
        }
    }

    /// Bits 2–3: Character block index (0–3)
    var charBlock: UInt8 {
        get { UInt8((raw >> 2) & 0b11) }
        set {
            raw = (raw & ~(0b11 << 2)) | (UInt16(newValue & 0b11) << 2)
        }
    }

    /// Bit 4: Mosaic on/off
    var mosaic: Bool {
        get { (raw & (1 << 4)) != 0 }
        set {
            if newValue { raw |=  (1 << 4) }
            else        { raw &= ~(1 << 4) }
        }
    }

    /// Bit 5: Color mode (0 = 4bpp, 1 = 8bpp)
    var is8bpp: Bool {
        get { (raw & (1 << 5)) != 0 }
        set {
            if newValue { raw |=  (1 << 5) }
            else        { raw &= ~(1 << 5) }
        }
    }

    /// Bits 8–12: Screen block index (0–31)
    var screenBlock: UInt8 {
        get { UInt8((raw >> 8) & 0b1_1111) }
        set {
            raw = (raw & ~(0b1_1111 << 8)) | (UInt16(newValue & 0b1_1111) << 8)
        }
    }

    /// Bit 13: Wraparound for affine backgrounds
    var wrap: Bool {
        get { (raw & (1 << 13)) != 0 }
        set {
            if newValue { raw |=  (1 << 13) }
            else        { raw &= ~(1 << 13) }
        }
    }

    /// Bits 14–15: BG size code (0…3)
    ///
    /// For text BGs: 0=256×256, 1=512×256, 2=256×512, 3=512×512
    /// For affine BGs: 0=128×128, 1=256×256, 2=512×512, 3=1024×1024
    var size: UInt8 {
        get { UInt8((raw >> 14) & 0b11) }
        set {
            raw = (raw & ~(0b11 << 14)) | (UInt16(newValue & 0b11) << 14)
        }
    }

    /// The underlying 16‑bit register value
    var value: UInt16 {
        get { raw }
        set { raw = newValue }
    }
}

/// Mirror of the GBA BGxHOFS (horizontal offset) register
struct BGOffsetX {
    private var raw: UInt16

    init(raw: UInt16 = 0) {
        self.raw = raw
    }

    /// Bits 0–8: Horizontal offset (0–511)
    var offset: UInt16 {
        get { raw & 0x01FF }
        set {
            raw = (raw & ~0x01FF) | (newValue & 0x01FF)
        }
    }

    /// Underlying raw register value
    var value: UInt16 {
        get { raw }
        set { raw = newValue }
    }
}

/// Mirror of the GBA BGxVOFS (vertical offset) register
struct BGOffsetY {
    private var raw: UInt16

    init(raw: UInt16 = 0) {
        self.raw = raw
    }

    /// Bits 0–8: Vertical offset (0–511)
    var offset: UInt16 {
        get { raw & 0x01FF }
        set {
            raw = (raw & ~0x01FF) | (newValue & 0x01FF)
        }
    }

    /// Underlying raw register value
    var value: UInt16 {
        get { raw }
        set { raw = newValue }
    }
}

struct Background {
    var in_REG_BGxCNT : BGControl = BGControl(raw: 0)
    var in_REG_BGxHOFS  : BGOffsetX = BGOffsetX(raw: 0)
    var in_REG_BGxVOFS  : BGOffsetY = BGOffsetY(raw: 0)
}
