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

typedef void *CpuRunnerHandle;

CpuRunnerHandle CpuRunner_Create();
int CpuRunner_Init(CpuRunnerHandle handle, int argc, char *argv[]);
void CpuRunner_Run(CpuRunnerHandle handle);
void CpuRunner_Destroy(CpuRunnerHandle handle);
void *CpuRunner_GetMemory(CpuRunnerHandle handle);

#ifdef __cplusplus
}
#endif

#endif /* Bridging_Header_cpp_h */
