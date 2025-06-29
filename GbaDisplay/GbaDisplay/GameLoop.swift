//
//  GameLoop.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

import Foundation
import AppKit
import UniformTypeIdentifiers


let VBLANK_TIME_MS : UInt32 = 5
let kMaxRawBytes : Int = 1024 * 1024;

class GameLoop {
    
    public func initCpuRunner()
    {
        let openPanel = NSOpenPanel()
        openPanel.allowedContentTypes = [UTType(filenameExtension: "gba")!, UTType(filenameExtension: "bin")!]
        openPanel.allowsMultipleSelection = true
        openPanel.canChooseDirectories = false
        openPanel.canChooseFiles = true
        if openPanel.runModal() == .OK
        {
            let urls = openPanel.urls
            let argv = [
                "GbaDisplay",
                urls[0].path,
                urls[1].path
            ]
            let cArgv: [UnsafeMutablePointer<CChar>?] = argv.map { strdup($0) }
            cArgv.withUnsafeBufferPointer { buffer in
                let ptr = UnsafeMutablePointer(mutating: buffer.baseAddress)
                CpuRunner_Init(CpuRunnerHandle, Int32(cArgv.count), ptr)
            }
        } else {
            print("No files selected")
        }
    }
    
    public func start()
    {
        timer = Timer.scheduledTimer(withTimeInterval: 60.0, repeats: true) { [weak self] _ in
            self?.dispatch()
        }
        DispatchQueue.global(qos: .background).async {
            CpuRunner_Run(self.CpuRunnerHandle)
        }
    }

    private var timer: Timer?
        
    // From CPU
    private var in_DISPCNT : UInt16 = 0b1 << 6
    private var in_background_registers_0 : Background = Background(ctr: 0, x: 0, y: 0)
    private var in_background_registers_1 : Background = Background(ctr: 0, x: 0, y: 0)
    private var in_background_registers_2 : Background = Background(ctr: 0, x: 0, y: 0)
    private var in_background_registers_3 : Background = Background(ctr: 0, x: 0, y: 0)
    
    public let CpuRunnerHandle : CpuRunnerHandle = CpuRunner_Create()
   
    private func SignalUpdateBuffersReady()
    {
        let IO_MEMORY = GetHalfWordMemoryPtr(memory_ptr: CpuRunner_GetMemory(CpuRunnerHandle)!, address: 0x04000000)
        let VCOUNT = IO_MEMORY.advanced(by: 0x6 / 2)
        VCOUNT.pointee = 159
    }

    private func SignalDoNotUpdateBuffers()
    {
        let IO_MEMORY = GetHalfWordMemoryPtr(memory_ptr: CpuRunner_GetMemory(CpuRunnerHandle)!, address: 0x04000000)
        let VCOUNT = IO_MEMORY.advanced(by: 0x6 / 2)
        VCOUNT.pointee = 0
    }
    
    private func dispatch()
    {
        // Set scanline to indicate to CPU that VRAM can be updated.
        SignalUpdateBuffersReady()
        
        // Wait for time for CPU to update buffers.
        usleep(VBLANK_TIME_MS * 1000);
        
        // Set scanline to indicate to CPU to not update buffers until the next dispatch.
        SignalDoNotUpdateBuffers()
    }
    
}
