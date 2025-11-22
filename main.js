var Module = {
  preinitializedWebGPUDevice: null,
  print: (...args) => console.log(...args),
  printErr: (...args) => console.error(...args),
};

async function initAndRun() {
  if (!('gpu' in navigator)) {
    console.error("WebGPU not supported in this browser (navigator.gpu is undefined)");
    return;
  }

  const adapter = await navigator.gpu.requestAdapter();
  if (!adapter) {
    console.error("WebGPU adapter is null (GPU blocked or unsupported).");
    return;
  }

  const device = await adapter.requestDevice();
  if (!device) {
    console.error("WebGPU device creation failed.");
    return;
  }

  // This is what emscripten_webgpu_get_device() asserts on.
  Module.preinitializedWebGPUDevice = device;

  const script = document.createElement('script');
  script.src = "renderer.js";
  document.body.appendChild(script);
}

initAndRun();
