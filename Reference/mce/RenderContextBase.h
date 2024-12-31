
class mce::RenderContextBase { /* Size=0x1b8 */
  /* 0x0000 */ mce::RenderContextStateBase currentState;
  /* 0x0110 */ mce::VertexFormatCacheKey lastVertexFormat;
  /* 0x0138 */ std::array<mce::ShaderProgram *,3> lastShaderPrograms;
  /* 0x0150 */ mce::FrameBufferObject* mFrameBufferObject;
  /* 0x0158 */ mce::SwapChain mSwapChain;
  /* 0x0178 */ mce::RenderDevice* mRenderDevice;
  /* 0x0180 */ mce::RenderTargetState mRenderTargetState;
  
  RenderContextBase();
  ~RenderContextBase();
  void trim();
  mce::RenderDevice* getDevice() const;
  mce::FrameBufferObject* getFrameBufferObject();
  void createWindowSizeDependentResources();
  void clearContextState();
  void lostContext();
  void draw(const mce::PrimitiveMode, const uint32_t, const uint32_t);
  void drawIndexed(const mce::PrimitiveMode, const uint32_t, const uint32_t);
  void drawInstanced(const mce::PrimitiveMode, const uint32_t, const uint32_t, const uint32_t);
  void drawIndexedInstanced(const mce::PrimitiveMode, const uint32_t, const uint32_t, const uint32_t);
  void setViewport(const mce::ViewportInfo&);
  void setScissorRect(const int32_t, const int32_t, const uint32_t, const uint32_t);
  void clearColorBuffer(const mce::Color&, const mce::ViewportInfo*) const;
  void clearDepthBuffer(const float, const mce::ViewportInfo*) const;
  void clearStencilBuffer(const unsigned char, const mce::ViewportInfo*) const;
  void clearDepthStencilBuffer(const float, const unsigned char, const mce::ViewportInfo*) const;
  bool isThreadedSubmitMode() const;
  void beginFrame();
  void endFrame();
  void flush();
  void swapBuffers();
  void discardBackbuffer();
  void unbindAllTexturesAndShaders();
  void setRenderTarget(mce::CommandList&);
  void setRenderTarget();
  mce::SwapChain& getSwapChain();
  glm::tvec2<float,0> getRenderTargetSize() const;
  bool captureScreenAsRGB(std::vector<unsigned char,std::allocator<unsigned char> >&, int32_t&, int32_t&);
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};