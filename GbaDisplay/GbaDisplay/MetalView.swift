//
//  MetalView.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

import SwiftUI
import MetalKit

struct MetalView: NSViewRepresentable {
    func makeCoordinator() -> TileRenderer {
        TileRenderer(mtkView: mtkView)
    }

    let mtkView = MTKView()

    func makeNSView(context: Context) -> MTKView {
        mtkView.enableSetNeedsDisplay = true
        mtkView.isPaused = false
        return mtkView
    }

    func updateNSView(_ nsView: MTKView, context: Context) {}
}
