//
//  Bridging-Header-cpp.h
//  GbaDisplay
//
//  Created by Paul Joo on 6/17/25.
//

#ifndef Bridging_Header_cpp_h
#define Bridging_Header_cpp_h

#ifdef __cplusplus
extern "C" {
#endif

typedef void* CpuRunnerHandle;

CpuRunnerHandle CpuRunner_create();
void CpuRunner_run(CpuRunnerHandle handle);
void CpuRunner_destroy(CpuRunnerHandle handle);
int* CpuRunner_GetVramBuffer(CpuRunnerHandle handle);

#ifdef __cplusplus
}
#endif

#endif /* Bridging_Header_cpp_h */
