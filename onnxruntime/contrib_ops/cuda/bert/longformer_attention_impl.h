// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include "core/providers/cuda/shared_inc/cuda_utils.h"

namespace onnxruntime {
namespace contrib {
namespace cuda {

size_t GetPinnedBufferSize(
    int batch_size);

size_t GetLongformerAttentionWorkspaceSize(
    size_t element_size,
    int batch_size,
    int num_heads,
    int head_size,
    int sequence_length,
    int max_num_global,
    int window,
    bool disable_compact_memory);

bool LaunchLongformerAttentionKernel(
    const cudaDeviceProp& device_prop,  // Device Properties
    cublasHandle_t cublas,              // Cublas handle
    cudaStream_t stream,                // CUDA stream
    cudaEvent_t &is_copy_done,          // CUDA event to synchronize data copy
    const void* input,                  // Input tensor
    const void* attention_mask,         // Attention mask with shape (B, S)
    const void* global_input,           // Global attention input, or nullptr when max_num_global == 0.
    const int* global_attention,        // Global attention flags with shape (B, S)
    const int* global_index,            // Global index
    const int* batch_global_num,        // Number of global tokens per batch. It is in device memory.
    void* pinned_buffer,                // Pinned memory: copy of batch_global_num, and a buffer to copy to scratch2.
    void* workspace,                    // Temporary buffer
    void* output,                       // Output tensor
    int batch_size,                     // Batch size (B)
    int sequence_length,                // Sequence length (S)
    int num_heads,                      // Number of attention heads (N)
    int head_size,                      // Hidden layer size per head (H)
    int window,                         // One sided attention window (W)
    int max_num_global,                 // Maximum number of global tokens (G)
    const size_t element_size,          // Element size of input tensor,
    bool disable_compact_memory         // Disable compact memory kernel
);

}  // namespace cuda
}  // namespace contrib
}  // namespace onnxruntime
