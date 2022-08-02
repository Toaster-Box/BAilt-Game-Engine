#include "GBuffer.h"

GBuffer::GBuffer(int screenWidth, int screenHeight)
{ 
    m_GBufferData.screenWidth = screenWidth;
    m_GBufferData.screenHeight = screenHeight;


    m_GBufferData.bufferId = rlLoadFramebuffer(m_GBufferData.screenWidth, m_GBufferData.screenHeight);   // Load an empty framebuffer

    if (m_GBufferData.bufferId > 0)
    {
        rlEnableFramebuffer(m_GBufferData.bufferId);

        // Create WorldPos tex 32bit RGBA PIXELFORMAT_UNCOMPRESSED_R32G32B32A32
        m_GBufferData.WorldPos.id = rlLoadTexture(NULL, m_GBufferData.screenWidth, m_GBufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32, 1);
        m_GBufferData.WorldPos.width = m_GBufferData.screenWidth;
        m_GBufferData.WorldPos.height = m_GBufferData.screenHeight;
        m_GBufferData.WorldPos.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
        m_GBufferData.WorldPos.mipmaps = 1;

        // Create Depth tex 32bit single channel
        m_GBufferData.Depth.id = rlLoadTextureDepth(m_GBufferData.screenWidth, m_GBufferData.screenHeight, false);
        m_GBufferData.Depth.width = m_GBufferData.screenWidth;
        m_GBufferData.Depth.height = m_GBufferData.screenHeight;
        m_GBufferData.Depth.format = PIXELFORMAT_UNCOMPRESSED_R32;
        m_GBufferData.Depth.mipmaps = 1;

        // Create Normal tex 8bit RGBA
        m_GBufferData.Normal.id = rlLoadTexture(NULL, m_GBufferData.screenWidth, m_GBufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_GBufferData.Normal.width = m_GBufferData.screenWidth;
        m_GBufferData.Normal.height = m_GBufferData.screenHeight;
        m_GBufferData.Normal.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_GBufferData.Normal.mipmaps = 1;

        // Create Albedo tex 8bit RGBA
        m_GBufferData.Albedo.id = rlLoadTexture(NULL, m_GBufferData.screenWidth, m_GBufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_GBufferData.Albedo.width = m_GBufferData.screenWidth;
        m_GBufferData.Albedo.height = m_GBufferData.screenHeight;
        m_GBufferData.Albedo.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_GBufferData.Albedo.mipmaps = 1;

        // Create Metalness tex 8bit RGBA
        m_GBufferData.Metalness.id = rlLoadTexture(NULL, m_GBufferData.screenWidth, m_GBufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_GBufferData.Metalness.width = m_GBufferData.screenWidth;
        m_GBufferData.Metalness.height = m_GBufferData.screenHeight;
        m_GBufferData.Metalness.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_GBufferData.Metalness.mipmaps = 1;

        // Create Roughness tex 8bit RGBA
        m_GBufferData.Roughness.id = rlLoadTexture(NULL, m_GBufferData.screenWidth, m_GBufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_GBufferData.Roughness.width = m_GBufferData.screenWidth;
        m_GBufferData.Roughness.height = m_GBufferData.screenHeight;
        m_GBufferData.Roughness.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_GBufferData.Roughness.mipmaps = 1;

        // Create Emissive tex 8bit RGBA
        m_GBufferData.Emissive.id = rlLoadTexture(NULL, m_GBufferData.screenWidth, m_GBufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_GBufferData.Emissive.width = m_GBufferData.screenWidth;
        m_GBufferData.Emissive.height = m_GBufferData.screenHeight;
        m_GBufferData.Emissive.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_GBufferData.Emissive.mipmaps = 1;


        // Attach all textures to FBO
        rlFramebufferAttach(m_GBufferData.bufferId, m_GBufferData.WorldPos.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_GBufferData.bufferId, m_GBufferData.Normal.id, RL_ATTACHMENT_COLOR_CHANNEL1, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_GBufferData.bufferId, m_GBufferData.Albedo.id, RL_ATTACHMENT_COLOR_CHANNEL2, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_GBufferData.bufferId, m_GBufferData.Metalness.id, RL_ATTACHMENT_COLOR_CHANNEL3, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_GBufferData.bufferId, m_GBufferData.Roughness.id, RL_ATTACHMENT_COLOR_CHANNEL4, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_GBufferData.bufferId, m_GBufferData.Emissive.id, RL_ATTACHMENT_COLOR_CHANNEL5, RL_ATTACHMENT_TEXTURE2D, 0);

        rlFramebufferAttach(m_GBufferData.bufferId, m_GBufferData.Depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_DEPTH, 0);

        
        
        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(m_GBufferData.bufferId)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", m_GBufferData.bufferId);

        rlEnableFramebuffer(m_GBufferData.bufferId);

        rlActiveDrawBuffers(6);

        rlDisableFramebuffer();
    }
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");
}

void GBuffer::BindForWriting()
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlEnableFramebuffer(m_GBufferData.bufferId); // Enable render target
  
    rlActiveDrawBuffers(6);

    // Set viewport and RLGL internal framebuffer size
    rlViewport(0, 0, m_GBufferData.screenWidth, m_GBufferData.screenHeight);

    rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
    rlLoadIdentity();               // Reset current matrix (projection)

    // Set orthographic projection to current framebuffer size
    // NOTE: Configured top-left corner as (0, 0)
    rlOrtho(0, m_GBufferData.screenWidth, m_GBufferData.screenHeight, 0, 0.0f, 1000.0f);

    rlMatrixMode(RL_MODELVIEW);     // Switch back to modelview matrix
    rlLoadIdentity();               // Reset current matrix (modelview)

	ClearBackground(BLACK);
}


void GBuffer::BindForReading()
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlDisableFramebuffer();         // Disable render target (fbo)
}
