//
//  Bridging-Header-cpp.mm
//  GbaDisplay
//
//  Created by Paul Joo on 6/17/25.
//

#import <Foundation/Foundation.h>

#import "Bridging-Header-cpp.h"
#import "cpu_runner.h"

CpuRunnerHandle CpuRunner_create() {
    return new CpuRunner::CpuRunner();
}

void CpuRunner_run(CpuRunnerHandle handle)
{
    static_cast<CpuRunner::CpuRunner*>(handle)->Run();
}

void CpuRunner_destroy(CpuRunnerHandle handle) {
    delete static_cast<CpuRunner::CpuRunner*>(handle);
}

int* CpuRunner_GetVramBuffer(CpuRunnerHandle handle) {
    return static_cast<CpuRunner::CpuRunner*>(handle)->VramBuffer;
}
