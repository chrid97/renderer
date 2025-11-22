#include <emscripten/html5_webgpu.h>
#include <stdio.h>
#include <webgpu/webgpu.h>

int main() {
  printf("WebGPU headers loaded successfully!\n");

  WGPUInstance instance = wgpuCreateInstance(NULL);
  WGPUDevice device = emscripten_webgpu_get_device();
  WGPUQueue queue = wgpuDeviceGetQueue(device);

  WGPUSurfaceDescriptorFromCanvasHTMLSelector canvas_desc = {
      .chain = {.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector},
      .selector = "canvas",
  };

  WGPUSurfaceDescriptor desc = {
      .nextInChain = &canvas_desc.chain,
      // .label = {.data = "Main window", .length = WGPU_STRLEN},
  };

  WGPUSurface surface = wgpuInstanceCreateSurface(instance, &desc);
  WGPUSwapChainDescriptor scDesc = {.usage = WGPUTextureUsage_RenderAttachment,
                                    .format = WGPUTextureFormat_BGRA8Unorm,
                                    .width = 800,
                                    .height = 800,
                                    .presentMode = WGPUPresentMode_Fifo};
  WGPUSwapChain swapchain = wgpuDeviceCreateSwapChain(device, surface, &scDesc);
  WGPUTextureView targetView = wgpuSwapChainGetCurrentTextureView(swapchain);

  WGPURenderPassColorAttachment colorAttachment = {
      .view = targetView,
      .clearValue = (WGPUColor){0.1, 0.2, 0.3, 1.0},
      .loadOp = WGPULoadOp_Clear,
      .storeOp = WGPUStoreOp_Store,
  };

  WGPURenderPassDescriptor renderPassDesc = {
      .colorAttachmentCount = 1,
      .colorAttachments = &colorAttachment,
  };

  WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, NULL);

  WGPURenderPassEncoder pass =
      wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDesc);
  wgpuRenderPassEncoderEnd(pass);

  WGPUCommandBuffer cmdBuf = wgpuCommandEncoderFinish(encoder, NULL);

  wgpuQueueSubmit(queue, 1, &cmdBuf);

  wgpuSwapChainPresent(swapchain);

  printf("Frame rendered!\n");

  return 0;
}
