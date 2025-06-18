//
//  Bridging-Header-cpp.mm
//  GbaDisplay
//
//  Created by Paul Joo on 6/17/25.
//

#import <Foundation/Foundation.h>

#import "Bridging-Header-cpp.h"
#import "cpu_runner.h"

CpuRunnerHandle CpuRunner_Create() {
    return new CpuRunner::CpuRunner();
}

void CpuRunner_Run(CpuRunnerHandle handle)
{
    static_cast<CpuRunner::CpuRunner*>(handle)->Run();
}

void CpuRunner_Destroy(CpuRunnerHandle handle) {
    delete static_cast<CpuRunner::CpuRunner*>(handle);
}

void* CpuRunner_GetMemory(CpuRunnerHandle handle) {
    return static_cast<CpuRunner::CpuRunner*>(handle)->Memory_;
}
