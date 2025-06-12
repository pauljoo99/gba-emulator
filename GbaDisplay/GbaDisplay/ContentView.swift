//
//  ContentView.swift
//  GbaDisplay
//
//  Created by Paul Joo on 6/12/25.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        MetalView()
            .frame(minWidth: 400, minHeight: 300)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
