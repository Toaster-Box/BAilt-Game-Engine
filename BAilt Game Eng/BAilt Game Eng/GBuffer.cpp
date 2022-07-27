#include "GBuffer.h"

GBuffer::GBuffer(ConfigLoader* cfgLoaderIn_ptr)
{ 
	m_ConfigLoader_ptr = cfgLoaderIn_ptr; 

    m_bufferData.screenWidth = *m_ConfigLoader_ptr->GetConfigScreenWidth();
    m_bufferData.screenHeight = *m_ConfigLoader_ptr->GetConfigScreenHeight();


    m_bufferData.bufferId = rlLoadFramebuffer(m_bufferData.screenWidth, m_bufferData.screenHeight);   // Load an empty framebuffer

    if (m_bufferData.bufferId > 0)
    {
        rlEnableFramebuffer(m_bufferData.bufferId);

        // Create WorldPos tex 32bit RGBA
        m_bufferData.WorldPos.id = rlLoadTexture(NULL, m_bufferData.screenWidth, m_bufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32, 1);
        m_bufferData.WorldPos.width = m_bufferData.screenWidth;
        m_bufferData.WorldPos.height = m_bufferData.screenHeight;
        m_bufferData.WorldPos.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
        m_bufferData.WorldPos.mipmaps = 1;

        // Create Depth tex 32bit single channel
        m_bufferData.Depth.id = rlLoadTextureDepth(m_bufferData.screenWidth, m_bufferData.screenHeight, false);
        m_bufferData.Depth.width = m_bufferData.screenWidth;
        m_bufferData.Depth.height = m_bufferData.screenHeight;
        m_bufferData.Depth.format = PIXELFORMAT_UNCOMPRESSED_R32;
        m_bufferData.Depth.mipmaps = 1;

        // Create Normal tex 8bit RGBA
        m_bufferData.Normal.id = rlLoadTexture(NULL, m_bufferData.screenWidth, m_bufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_bufferData.Normal.width = m_bufferData.screenWidth;
        m_bufferData.Normal.height = m_bufferData.screenHeight;
        m_bufferData.Normal.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_bufferData.Normal.mipmaps = 1;

        // Create Albedo tex 8bit RGBA
        m_bufferData.Albedo.id = rlLoadTexture(NULL, m_bufferData.screenWidth, m_bufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_bufferData.Albedo.width = m_bufferData.screenWidth;
        m_bufferData.Albedo.height = m_bufferData.screenHeight;
        m_bufferData.Albedo.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_bufferData.Albedo.mipmaps = 1;

        // Create Metalness tex 8bit RGBA
        m_bufferData.Metalness.id = rlLoadTexture(NULL, m_bufferData.screenWidth, m_bufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_bufferData.Metalness.width = m_bufferData.screenWidth;
        m_bufferData.Metalness.height = m_bufferData.screenHeight;
        m_bufferData.Metalness.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_bufferData.Metalness.mipmaps = 1;

        // Create Roughness tex 8bit RGBA
        m_bufferData.Roughness.id = rlLoadTexture(NULL, m_bufferData.screenWidth, m_bufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_bufferData.Roughness.width = m_bufferData.screenWidth;
        m_bufferData.Roughness.height = m_bufferData.screenHeight;
        m_bufferData.Roughness.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_bufferData.Roughness.mipmaps = 1;

        // Create Emissive tex 8bit RGBA
        m_bufferData.Emissive.id = rlLoadTexture(NULL, m_bufferData.screenWidth, m_bufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_bufferData.Emissive.width = m_bufferData.screenWidth;
        m_bufferData.Emissive.height = m_bufferData.screenHeight;
        m_bufferData.Emissive.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_bufferData.Emissive.mipmaps = 1;


        // Attach all textures to FBO
        rlFramebufferAttach(m_bufferData.bufferId, m_bufferData.WorldPos.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_bufferData.bufferId, m_bufferData.Normal.id, RL_ATTACHMENT_COLOR_CHANNEL1, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_bufferData.bufferId, m_bufferData.Albedo.id, RL_ATTACHMENT_COLOR_CHANNEL2, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_bufferData.bufferId, m_bufferData.Metalness.id, RL_ATTACHMENT_COLOR_CHANNEL3, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_bufferData.bufferId, m_bufferData.Roughness.id, RL_ATTACHMENT_COLOR_CHANNEL4, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_bufferData.bufferId, m_bufferData.Emissive.id, RL_ATTACHMENT_COLOR_CHANNEL5, RL_ATTACHMENT_TEXTURE2D, 0);

        rlFramebufferAttach(m_bufferData.bufferId, m_bufferData.Depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_DEPTH, 0);

        
        
        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(m_bufferData.bufferId)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", m_bufferData.bufferId);

        rlEnableFramebuffer(m_bufferData.bufferId);

        rlActiveDrawBuffers(6);

        rlDisableFramebuffer();
    }
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");
}

void GBuffer::BindForWriting()
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlEnableFramebuffer(m_bufferData.bufferId); // Enable render target
  
    rlActiveDrawBuffers(6);

    // Set viewport and RLGL internal framebuffer size
    rlViewport(0, 0, m_bufferData.screenWidth, m_bufferData.screenHeight);

    rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
    rlLoadIdentity();               // Reset current matrix (projection)

    // Set orthographic projection to current framebuffer size
    // NOTE: Configured top-left corner as (0, 0)
    rlOrtho(0, m_bufferData.screenWidth, m_bufferData.screenHeight, 0, 0.0f, 1000.0f);

    rlMatrixMode(RL_MODELVIEW);     // Switch back to modelview matrix
    rlLoadIdentity();               // Reset current matrix (modelview)

	ClearBackground(LIGHTGRAY);
}


void GBuffer::BindForReading()
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlDisableFramebuffer();         // Disable render target (fbo)
}
