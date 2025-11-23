#include <emscripten/html5_webgpu.h>
#include <stdio.h>
#include <webgpu/webgpu.h>

#define SHADER_SOURCE(...) #__VA_ARGS__

static const char *wgsl_triangle = SHADER_SOURCE(
    // what goes into our shader
    struct VertexIn {
      @location(0) aPos : vec2<f32>, @location(1) aCol : vec3<f32>,
    };

    @vertex fn vs_main(@builtin(vertex_index) vertexIndex : u32)
        ->@builtin(position) vec4f {
          let pos = array(vec2f(0.0, 0.5),   // top center
                          vec2f(-0.5, -0.5), // bottom left
                          vec2f(0.5, -0.5)   // bottom right
          );

          return vec4f(pos[vertexIndex], 0.0, 1.0);
        }

    @fragment fn fs()
        ->@location(0) vec4f { return vec4f(1.0, 0.0, 0.0, 1.0); }

);

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

  WGPUShaderModuleWGSLDescriptor wgsl = {
      .chain.sType = WGPUSType_ShaderModuleWGSLDescriptor, .code = ""};

  wgpuDeviceCreateShaderModule(device,
                               &(WGPUShaderModuleDescriptor){
                                   .nextInChain = (WGPUChainedStruct *)(&wgsl),
                                   .label = "Hardcoded red triangle pipeline"});

  // render
  wgpuDeviceCreateCommandEncoder(
      device, &(WGPUCommandEncoderDescriptor){.label = "Command Encoder"});
  // WGPURenderPassEncoder
  // submit encoder?

  printf("Frame rendered!\n");

  return 0;
}
