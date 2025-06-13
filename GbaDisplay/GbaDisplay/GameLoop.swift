//
//  GameLoop.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

import Foundation

let VBLANK_TIME_MS : UInt32 = 5;

class GameLoop {
    
    private var timer: Timer?
    
    var scanLine : uint = 0
    var triangle_pos : uint = 0
    
    func run()
    {
        timer = Timer.scheduledTimer(withTimeInterval: 1.0 / 60.0, repeats: true) { [weak self] _ in
            self?.dispatch()
        }
    }
    
    func dispatch()
    {
        // Set scanline to indicate to CPU that VRAM can be updated.
        scanLine = 160;
        
        // Wait for time for CPU to update.
        usleep(VBLANK_TIME_MS * 1000);
        
        // Copy over data/ Do action
        triangle_pos = (triangle_pos + 1) % 100;
    }
    
}
