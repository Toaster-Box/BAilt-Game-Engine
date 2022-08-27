#include "LBuffer.h"

LBuffer::LBuffer(ConfigLoader* ConfigLoaderIn_ptr, GBuffer* GBufferIn_ptr)
{
    m_ConfigLoader_ptr = ConfigLoaderIn_ptr;

    m_GBuffer_ptr = GBufferIn_ptr;
    m_LBufferData.screenWidth = m_GBuffer_ptr->GetBufferWidth();
    m_LBufferData.screenHeight = m_GBuffer_ptr->GetBufferHeight();

    m_LBufferData.bufferId = rlLoadFramebuffer(m_LBufferData.screenWidth, m_LBufferData.screenHeight);   // Load an empty framebuffer

    if (m_LBufferData.bufferId > 0)
    {
        rlEnableFramebuffer(m_LBufferData.bufferId);
	   
        // Create HDR direct lighting tex 32bit RGBA 
        m_LBufferData.DirectLighting.id = rlLoadTexture(NULL, m_LBufferData.screenWidth, m_LBufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32, 1);
        m_LBufferData.DirectLighting.width = m_LBufferData.screenWidth;
        m_LBufferData.DirectLighting.height = m_LBufferData.screenHeight;
        m_LBufferData.DirectLighting.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
        m_LBufferData.DirectLighting.mipmaps = 1;

        // Create HDR indirect lighting tex 32bit RGBA
        m_LBufferData.IndirectLighting.id = rlLoadTexture(NULL, m_LBufferData.screenWidth, m_LBufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32, 1);
        m_LBufferData.IndirectLighting.width = m_LBufferData.screenWidth;
        m_LBufferData.IndirectLighting.height = m_LBufferData.screenHeight;
        m_LBufferData.IndirectLighting.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
        m_LBufferData.IndirectLighting.mipmaps = 1;

        // Create HDR final light tex 32bit RGBA
        m_LBufferData.FinalIlluminatedScene.id = rlLoadTexture(NULL, m_LBufferData.screenWidth, m_LBufferData.screenHeight, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32, 1);
        m_LBufferData.FinalIlluminatedScene.width = m_LBufferData.screenWidth;
        m_LBufferData.FinalIlluminatedScene.height = m_LBufferData.screenHeight;
        m_LBufferData.FinalIlluminatedScene.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
        m_LBufferData.FinalIlluminatedScene.mipmaps = 1;

        // Create Depth tex 32bit single channel
        m_LBufferData.Depth.id = m_GBuffer_ptr->GetDepthTexID();
        m_LBufferData.Depth.width = m_LBufferData.screenWidth;
        m_LBufferData.Depth.height = m_LBufferData.screenHeight;
        m_LBufferData.Depth.format = PIXELFORMAT_UNCOMPRESSED_R32;
        m_LBufferData.Depth.mipmaps = 1;

        // Attach all textures to FBO
        rlFramebufferAttach(m_LBufferData.bufferId, m_LBufferData.DirectLighting.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_LBufferData.bufferId, m_LBufferData.IndirectLighting.id, RL_ATTACHMENT_COLOR_CHANNEL1, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_LBufferData.bufferId, m_LBufferData.FinalIlluminatedScene.id, RL_ATTACHMENT_COLOR_CHANNEL2, RL_ATTACHMENT_TEXTURE2D, 0);

        rlFramebufferAttach(m_LBufferData.bufferId, m_LBufferData.Depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_DEPTH, 0);

        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(m_LBufferData.bufferId)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", m_LBufferData.bufferId);

        rlEnableFramebuffer(m_LBufferData.bufferId);

        rlActiveDrawBuffers(4);

        rlDisableFramebuffer();
    }
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    // Init shaders, mats, and models
    m_DirectionalLightShader.shader = LoadShader("D:/LBufferShader.vs", "D:/LBufferDirectionalShader.fs");
    m_DirectionalLightShader.shader.locs[SHADER_LOC_MAP_OCCLUSION] = rlGetLocationUniform(m_DirectionalLightShader.shader.id, "WorldPosTex"); //no default worldpos map so we use an empty non-cubemap
    m_DirectionalLightShader.shader.locs[SHADER_LOC_MAP_NORMAL] = rlGetLocationUniform(m_DirectionalLightShader.shader.id, "NormalTex");
    m_DirectionalLightShader.shader.locs[SHADER_LOC_MAP_ALBEDO] = rlGetLocationUniform(m_DirectionalLightShader.shader.id, "AlbedoTex");
    m_DirectionalLightShader.shader.locs[SHADER_LOC_MAP_METALNESS] = rlGetLocationUniform(m_DirectionalLightShader.shader.id, "MetalnessTex");
    m_DirectionalLightShader.shader.locs[SHADER_LOC_MAP_ROUGHNESS] = rlGetLocationUniform(m_DirectionalLightShader.shader.id, "RoughnessTex");
    //m_DirectionalLightShader.shader.locs[SHADER_LOC_MAP_EMISSION] = rlGetLocationUniform(m_DirectionalLightShader.shader.id, "EmissiveTex"); //Emissive light is added in the directional light pass
    m_DirectionalLightShader.shader.locs[SHADER_LOC_MAP_BRDF] = rlGetLocationUniform(m_DirectionalLightShader.shader.id, "DirectLightPassTex"); //once again using an empty tex map location
    m_DirectionalLightShader.cameraPositionLoc = GetShaderLocation(m_DirectionalLightShader.shader, "CameraWorldPos");
    m_DirectionalLightShader.colorLoc = GetShaderLocation(m_DirectionalLightShader.shader, "SampledDirectionalLight.Base.color");
    m_DirectionalLightShader.ambientIntensityLoc = GetShaderLocation(m_DirectionalLightShader.shader, "SampledDirectionalLight.Base.ambientIntensity");
    m_DirectionalLightShader.diffuseIntensityLoc = GetShaderLocation(m_DirectionalLightShader.shader, "SampledDirectionalLight.Base.diffuseIntensity");
    m_DirectionalLightShader.directionLoc = GetShaderLocation(m_DirectionalLightShader.shader, "SampledDirectionalLight.direction");

    m_PointLightShader.shader = LoadShader("D:/LBufferShader.vs", "D:/LBufferPointShader.fs");
    m_PointLightShader.shader.locs[SHADER_LOC_MAP_OCCLUSION] = GetShaderLocation(m_PointLightShader.shader, "WorldPosTex"); //no default worldpos map so we use an empty non-cubemap
    m_PointLightShader.shader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(m_PointLightShader.shader, "NormalTex");
    m_PointLightShader.shader.locs[SHADER_LOC_MAP_ALBEDO] = GetShaderLocation(m_PointLightShader.shader, "AlbedoTex");
    m_PointLightShader.shader.locs[SHADER_LOC_MAP_METALNESS] = GetShaderLocation(m_PointLightShader.shader, "MetalnessTex");
    m_PointLightShader.shader.locs[SHADER_LOC_MAP_ROUGHNESS] = GetShaderLocation(m_PointLightShader.shader, "RoughnessTex");
    m_PointLightShader.shader.locs[SHADER_LOC_MAP_BRDF] = GetShaderLocation(m_PointLightShader.shader, "DirectLightPassTex"); //once again using an empty tex map location
    m_PointLightShader.screenSizeLoc = GetShaderLocation(m_PointLightShader.shader, "ScreenSize");
    m_PointLightShader.cameraPositionLoc = GetShaderLocation(m_PointLightShader.shader, "CameraWorldPos");
    m_PointLightShader.colorLoc = GetShaderLocation(m_PointLightShader.shader, "SampledPointLight.Base.color");
    m_PointLightShader.ambientIntensityLoc = GetShaderLocation(m_PointLightShader.shader, "SampledPointLight.Base.ambientIntensity");
    m_PointLightShader.diffuseIntensityLoc = GetShaderLocation(m_PointLightShader.shader, "SampledPointLight.Base.diffuseIntensity");
    m_PointLightShader.positionLoc = GetShaderLocation(m_PointLightShader.shader, "SampledPointLight.position");
    m_PointLightShader.constantAttLoc = GetShaderLocation(m_PointLightShader.shader, "SampledPointLight.constantAtt");
    m_PointLightShader.linearAttLoc = GetShaderLocation(m_PointLightShader.shader, "SampledPointLight.linearAtt");
    m_PointLightShader.exponentialAttLoc = GetShaderLocation(m_PointLightShader.shader, "SampledPointLight.exponentialAtt");

    m_SpotLightShader.shader = LoadShader("D:/LBufferShader.vs", "D:/LBufferSpotShader.fs");
    m_SpotLightShader.shader.locs[SHADER_LOC_MAP_OCCLUSION] = GetShaderLocation(m_SpotLightShader.shader, "WorldPosTex"); //no default worldpos map so we use an empty non-cubemap
    m_SpotLightShader.shader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(m_SpotLightShader.shader, "NormalTex");
    m_SpotLightShader.shader.locs[SHADER_LOC_MAP_ALBEDO] = GetShaderLocation(m_SpotLightShader.shader, "AlbedoTex");
    m_SpotLightShader.shader.locs[SHADER_LOC_MAP_METALNESS] = GetShaderLocation(m_SpotLightShader.shader, "MetalnessTex");
    m_SpotLightShader.shader.locs[SHADER_LOC_MAP_ROUGHNESS] = GetShaderLocation(m_SpotLightShader.shader, "RoughnessTex");
    m_SpotLightShader.shader.locs[SHADER_LOC_MAP_BRDF] = GetShaderLocation(m_SpotLightShader.shader, "DirectLightPassTex"); //once again using an empty tex map location
    m_SpotLightShader.screenSizeLoc = GetShaderLocation(m_SpotLightShader.shader, "ScreenSize");
    m_SpotLightShader.cameraPositionLoc = GetShaderLocation(m_SpotLightShader.shader, "CameraWorldPos");
    m_SpotLightShader.colorLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.Base.Base.color");
    m_SpotLightShader.ambientIntensityLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.Base.Base.ambientIntensity");
    m_SpotLightShader.diffuseIntensityLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.Base.Base.diffuseIntensity");
    m_SpotLightShader.positionLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.Base.position");
    m_SpotLightShader.constantAttLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.Base.constantAtt");
    m_SpotLightShader.linearAttLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.Base.linearAtt");
    m_SpotLightShader.exponentialAttLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.Base.exponentialAtt");
    m_SpotLightShader.directionLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.direction");
    m_SpotLightShader.innerCutOffAngleLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.cutOff");
    m_SpotLightShader.outerCutOffAngleLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.outerCutOff");
    m_SpotLightShader.epsilonAngleLoc = GetShaderLocation(m_SpotLightShader.shader, "SampledSpotLight.epsilon");


    float screenRatio = float(m_LBufferData.screenWidth) / float(m_LBufferData.screenHeight);
    Mesh tempMesh = GenMeshPlane(screenRatio, 1.0f, 1, 1);
    m_planeModel = LoadModelFromMesh(tempMesh);

    m_planeModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = m_GBuffer_ptr->GetAlbedoTex();
    m_planeModel.materials[0].maps[MATERIAL_MAP_OCCLUSION].texture = m_GBuffer_ptr->GetWorldPosTex();
    m_planeModel.materials[0].maps[MATERIAL_MAP_NORMAL].texture = m_GBuffer_ptr->GetNormalTex();
    m_planeModel.materials[0].maps[MATERIAL_MAP_METALNESS].texture = m_GBuffer_ptr->GetMetalnessTex();
    m_planeModel.materials[0].maps[MATERIAL_MAP_ROUGHNESS].texture = m_GBuffer_ptr->GetRoughnessTex();
   //m_planeModel.materials[0].maps[MATERIAL_MAP_EMISSION].texture = m_GBuffer_ptr->GetEmissiveTex();
    m_planeModel.materials[0].shader = m_DirectionalLightShader.shader;

    Vector3 pos = {0.0f, 1.0f, 0.0f};
    Vector3 dir = {0.0f, -1.0f, 0.0f};
    Vector3 up = {0.0f, 0.0f, 1.0f};
    m_dirLightCam.SetProjection(CAMERA_ORTHOGRAPHIC);
    m_dirLightCam.SetFOV(1.0f);
    m_dirLightCam.SetCameraPosition(pos);
    m_dirLightCam.LookAt(dir, up);

    std::string icoshpereFilename = *m_ConfigLoader_ptr->GetBaseDirectory();
    std::string pFileNameAppend = "Icoshpereinvertnormals.obj";
    icoshpereFilename.append(pFileNameAppend);
    const char* pFileNameCast = const_cast<char*>(icoshpereFilename.c_str());
    m_icosphereModel = LoadModel(pFileNameCast);
    

    m_icosphereModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = m_GBuffer_ptr->GetAlbedoTex();
    m_icosphereModel.materials[0].maps[MATERIAL_MAP_OCCLUSION].texture = m_GBuffer_ptr->GetWorldPosTex();
    m_icosphereModel.materials[0].maps[MATERIAL_MAP_NORMAL].texture = m_GBuffer_ptr->GetNormalTex();
    m_icosphereModel.materials[0].maps[MATERIAL_MAP_METALNESS].texture = m_GBuffer_ptr->GetMetalnessTex();
    m_icosphereModel.materials[0].maps[MATERIAL_MAP_ROUGHNESS].texture = m_GBuffer_ptr->GetRoughnessTex();
    m_icosphereModel.materials[0].maps[MATERIAL_MAP_BRDF].texture = m_LBufferData.DirectLighting;
    m_icosphereModel.materials[0].shader = m_PointLightShader.shader;

    std::string coneFilename = *m_ConfigLoader_ptr->GetBaseDirectory();
    std::string sFileNameAppend = "Coneinvertnormals.obj";
    coneFilename.append(sFileNameAppend);
    const char* sFileNameCast = const_cast<char*>(coneFilename.c_str());
    m_coneModel = LoadModel(sFileNameCast);

    m_coneTransformationMatrix = MatrixIdentity();

    m_coneModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = m_GBuffer_ptr->GetAlbedoTex();
    m_coneModel.materials[0].maps[MATERIAL_MAP_OCCLUSION].texture = m_GBuffer_ptr->GetWorldPosTex();
    m_coneModel.materials[0].maps[MATERIAL_MAP_NORMAL].texture = m_GBuffer_ptr->GetNormalTex();
    m_coneModel.materials[0].maps[MATERIAL_MAP_METALNESS].texture = m_GBuffer_ptr->GetMetalnessTex();
    m_coneModel.materials[0].maps[MATERIAL_MAP_ROUGHNESS].texture = m_GBuffer_ptr->GetRoughnessTex();
    m_coneModel.materials[0].maps[MATERIAL_MAP_BRDF].texture = m_LBufferData.DirectLighting;
    m_coneModel.materials[0].shader = m_SpotLightShader.shader;
}     


void LBuffer::UpdateLBuffer(Camera3D& cameraIn)
{
    BindForWriting();

    rlSetUniformSampler(SHADER_LOC_MAP_OCCLUSION, m_GBuffer_ptr->GetWorldPosTexID());
    rlSetUniformSampler(SHADER_LOC_MAP_NORMAL, m_GBuffer_ptr->GetNormalTexID());
    rlSetUniformSampler(SHADER_LOC_MAP_ALBEDO, m_GBuffer_ptr->GetAlbedoTexID());
    rlSetUniformSampler(SHADER_LOC_MAP_METALNESS, m_GBuffer_ptr->GetMetalnessTexID());
    rlSetUniformSampler(SHADER_LOC_MAP_ROUGHNESS, m_GBuffer_ptr->GetRoughnessTexID());
    rlSetUniformSampler(SHADER_LOC_MAP_EMISSION, m_GBuffer_ptr->GetEmissiveTexID());
    rlSetUniformSampler(SHADER_LOC_MAP_BRDF, m_LBufferData.DirectLighting.id);

    CalcDirectLighting(cameraIn);
    CalcIndirectLighting(cameraIn);

    BindForReading();
}


void LBuffer::CalcDirectLighting(Camera3D& cameraIn)
{
    float camPos[3] = { -cameraIn.position.x, cameraIn.position.y, cameraIn.position.z };
    float screenSize[2] = { m_LBufferData.screenWidth, m_LBufferData.screenHeight };

    SetShaderValue(m_DirectionalLightShader.shader, m_DirectionalLightShader.cameraPositionLoc, camPos, RL_SHADER_UNIFORM_VEC3);

    SetShaderValue(m_PointLightShader.shader, m_PointLightShader.cameraPositionLoc, camPos, RL_SHADER_UNIFORM_VEC3);
    SetShaderValue(m_PointLightShader.shader, m_PointLightShader.screenSizeLoc, screenSize, RL_SHADER_UNIFORM_VEC2);

    SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.cameraPositionLoc, camPos, RL_SHADER_UNIFORM_VEC3);
    SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.screenSizeLoc, screenSize, RL_SHADER_UNIFORM_VEC2);

    //SetShaderValueV(m_SpotLightShader.shader, m_SpotLightShader.cameraPositionLoc, &cameraIn.position, RL_SHADER_UNIFORM_VEC3, 3);

    // Begin directional lights
    BeginMode3D(*m_dirLightCam.GetCameraPTR());
    //rlDisableDepthTest();
    rlDisableBackfaceCulling();
    Vector3 zeros = {0.0f, 0.0f, 0.0f};
    Vector3 planeScale = {-1.0f, 1.0f, 1.0f};
    for (unsigned int i = 0; i < m_DirectionalLightContainer.size(); i++)
    {
        Vector2 screenSize = { m_LBufferData.screenWidth, m_LBufferData.screenHeight };

        float color[3] = { m_DirectionalLightContainer[i].color.x, m_DirectionalLightContainer[i].color.y, m_DirectionalLightContainer[i].color.z };
        m_DirectionalLightContainer[i].direction = Vector3Normalize(m_DirectionalLightContainer[i].direction);
        float direction[3] = { m_DirectionalLightContainer[i].direction.x, m_DirectionalLightContainer[i].direction.y, m_DirectionalLightContainer[i].direction.z };
        float ambient[1] = { m_DirectionalLightContainer[i].ambientIntensity };
        float diffuse[1] = { m_DirectionalLightContainer[i].diffuseIntensity };

        SetShaderValue(m_DirectionalLightShader.shader, m_DirectionalLightShader.colorLoc, color, RL_SHADER_UNIFORM_VEC3);
        SetShaderValue(m_DirectionalLightShader.shader, m_DirectionalLightShader.ambientIntensityLoc, ambient, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_DirectionalLightShader.shader, m_DirectionalLightShader.diffuseIntensityLoc, diffuse, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_DirectionalLightShader.shader, m_DirectionalLightShader.directionLoc, direction, RL_SHADER_UNIFORM_VEC3);

        DrawModelEx(m_planeModel, zeros, zeros, 0.0f, planeScale, WHITE);
    }
    rlEnableBackfaceCulling();
    EndMode3D();
    // End directional lights

    // Begin point and spot lights
    BeginMode3D(cameraIn);
    rlDisableDepthTest();
    for (unsigned i = 0; i < m_PointLightContainer.size(); i++)
    {
        float pConstant[1] = { m_PointLightContainer[i].constantAtt };
        float pLinear[1] = { m_PointLightContainer[i].linearAtt };
        float pExponential[1] = { m_PointLightContainer[i].exponentioalAtt };
        float pDiffuseIntensity[1] = { m_PointLightContainer[i].diffuseIntensity };
        
        float pColor[3] = { m_PointLightContainer[i].color.x, m_PointLightContainer[i].color.y, m_PointLightContainer[i].color.z };
        float pPosition[3] = { m_PointLightContainer[i].position.x, m_PointLightContainer[i].position.y, m_PointLightContainer[i].position.z };
        float pAmbient[1] = { m_PointLightContainer[i].ambientIntensity };

        SetShaderValue(m_PointLightShader.shader, m_PointLightShader.colorLoc, pColor, RL_SHADER_UNIFORM_VEC3);
        SetShaderValue(m_PointLightShader.shader, m_PointLightShader.ambientIntensityLoc, pAmbient, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_PointLightShader.shader, m_PointLightShader.diffuseIntensityLoc, pDiffuseIntensity, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_PointLightShader.shader, m_PointLightShader.positionLoc, pPosition, RL_SHADER_UNIFORM_VEC3);
        
        SetShaderValue(m_PointLightShader.shader, m_PointLightShader.constantAttLoc, pConstant, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_PointLightShader.shader, m_PointLightShader.linearAttLoc, pLinear, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_PointLightShader.shader, m_PointLightShader.exponentialAttLoc, pExponential, RL_SHADER_UNIFORM_FLOAT);

        float pLightRadius = (-pLinear[0] + std::sqrtf(pLinear[0] * pLinear[0] - 4.0f * pExponential[0] * (pConstant[0] - (256.0f / 5.0f) * pDiffuseIntensity[0]))) / (2 * pExponential[0]);
        Vector3 pCorrectedPos = { -m_PointLightContainer[i].position.x, m_PointLightContainer[i].position.y, m_PointLightContainer[i].position.z};

        DrawModel(m_icosphereModel, pCorrectedPos, pLightRadius, WHITE);
    }

    for (unsigned int i = 0; i < m_SpotLightContainer.size(); i++)
    {
        float sConstant[1] = { m_SpotLightContainer[i].constantAtt };
        float sLinear[1] = { m_SpotLightContainer[i].linearAtt };
        float sExponential[1] = { m_SpotLightContainer[i].exponentioalAtt };
        float sDiffuseIntensity[1] = { m_SpotLightContainer[i].diffuseIntensity };
        float sCutOffAngle[1] = { m_SpotLightContainer[i].innerCutOffAngle };
        float sOuterCutOffAngle[1] = { cosf(m_SpotLightContainer[i].outerCutOffAngle) }; //only the cos is used so its precalced
        float sEpsilon[1] = { cosf(m_SpotLightContainer[i].innerCutOffAngle) - cosf(m_SpotLightContainer[i].outerCutOffAngle) }; // precomputing this since its the same for every fs instance
        
        float sColor[3] = { m_SpotLightContainer[i].color.x, m_SpotLightContainer[i].color.y, m_SpotLightContainer[i].color.z };
        float sPosition[3] = { m_SpotLightContainer[i].position.x, m_SpotLightContainer[i].position.y, m_SpotLightContainer[i].position.z };
        float sDirection[3] = { m_SpotLightContainer[i].direction.x, m_SpotLightContainer[i].direction.y, m_SpotLightContainer[i].direction.z };
        float sAmbient[1] = { m_SpotLightContainer[i].ambientIntensity };

        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.colorLoc, sColor, RL_SHADER_UNIFORM_VEC3);
        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.ambientIntensityLoc, sAmbient, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.diffuseIntensityLoc, sDiffuseIntensity, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.innerCutOffAngleLoc, sCutOffAngle, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.outerCutOffAngleLoc, sOuterCutOffAngle, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.epsilonAngleLoc, sEpsilon, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.positionLoc, sPosition, RL_SHADER_UNIFORM_VEC3);
        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.directionLoc, sDirection, RL_SHADER_UNIFORM_VEC3);

        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.constantAttLoc, sConstant, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.linearAttLoc, sLinear, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_SpotLightShader.shader, m_SpotLightShader.exponentialAttLoc, sExponential, RL_SHADER_UNIFORM_FLOAT);
        
        float sLightDistance = (-sLinear[0] + std::sqrtf(sLinear[0] * sLinear[0] - 4.0f * sExponential[0] * (sConstant[0] - (256.0f / 5.0f) * sDiffuseIntensity[0]))) / (2 * sExponential[0]);
        float sLightRadius = tanf(m_SpotLightContainer[i].outerCutOffAngle) * sLightDistance;
        Vector3 sScale = { sLightRadius, sLightDistance, sLightRadius };
        m_coneTransformation.SetPosition(m_SpotLightContainer[i].position);
        m_coneTransformation.SetScale(sScale);
        m_coneTransformation.LookAt(m_SpotLightContainer[i].direction, {0.0f, 0.0f, 1.0f});
        m_coneTransformation.UpdateTransformation(&m_coneTransformationMatrix);
        m_coneModel.transform = m_coneTransformationMatrix;

        DrawModel(m_coneModel, zeros, 1.0f, WHITE);
    }
    rlEnableDepthTest();
    EndMode3D();
    // End point and spot lights
}

void LBuffer::CalcIndirectLighting(Camera3D& cameraIn)
{
}

void LBuffer::BindForWriting()
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlEnableFramebuffer(m_LBufferData.bufferId); // Enable render target

    rlActiveDrawBuffers(4);

    // Set viewport and RLGL internal framebuffer size
    rlViewport(0, 0, m_LBufferData.screenWidth, m_LBufferData.screenHeight);

    rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
    rlLoadIdentity();               // Reset current matrix (projection)

    // Set orthographic projection to current framebuffer size
    // NOTE: Configured top-left corner as (0, 0)
    rlOrtho(0, m_LBufferData.screenWidth, m_LBufferData.screenHeight, 0, 0.0f, 1000.0f);

    rlMatrixMode(RL_MODELVIEW);     // Switch back to modelview matrix
    rlLoadIdentity();               // Reset current matrix (modelview)

    ClearBackground(BLACK);
}

void LBuffer::BindForReading()
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlDisableFramebuffer();         // Disable render target (fbo)
}

unsigned int LBuffer::CreateDirectionalLight()
{
    DirectionalLight newDirLight; 
    m_DirectionalLightContainer.push_back(newDirLight); 
    return m_DirectionalLightContainer.size() - 1;
}


unsigned int LBuffer::CreatePointLight()
{
    PointLight newPointLight;
    m_PointLightContainer.push_back(newPointLight);
    return m_PointLightContainer.size() - 1;
}


unsigned int LBuffer::CreateSpotLight()
{
    SpotLight newSpotLight;
    m_SpotLightContainer.push_back(newSpotLight);
    return m_SpotLightContainer.size() - 1;
}


bool LBuffer::CheckDirectionalLightIndex(unsigned int index)
{
    if (index > m_DirectionalLightContainer.size() + 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}


bool LBuffer::CheckPointLightIndex(unsigned int index)
{
    if (index > m_PointLightContainer.size() + 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}


bool LBuffer::CheckSpotLightIndex(unsigned int index)
{
    if (index > m_SpotLightContainer.size() + 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}
