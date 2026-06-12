//------------------------------------------------------------------------------------------------
struct VertexInput
{
	float3 modelPosition : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
	float3 modelTangent : TANGENT;
	float3 modelBitangent : BITANGENT;
	float3 modelNormal : NORMAL;
};

//------------------------------------------------------------------------------------------------
struct PixelInput
{
	float4 clipPosition : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
	float4 worldTangent : TANGENT;
	float4 worldBitangent : BITANGENT;
	float4 worldNormal : NORMAL;
    int debugId : DEBUG_ID;
};

//------------------------------------------------------------------------------------------------
cbuffer PerFrameConstants : register(b1)
{
    float Time;
    int DebugID;
};

//------------------------------------------------------------------------------------------------
cbuffer CameraConstants : register(b2)
{
	float4x4 WorldToCameraTransform;	// View transform
	float4x4 CameraToRenderTransform;	// Non-standard transform from game to DirectX conventions
	float4x4 RenderToClipTransform;		// Projection transform
};

//------------------------------------------------------------------------------------------------
cbuffer ModelConstants : register(b3)
{
	float4x4 ModelToWorldTransform;		// Model transform
	float4 ModelColor;
};

//------------------------------------------------------------------------------------------------
cbuffer LightConstants : register(b8)
{
    float3 SunDirection;
    float SunIntensity;
    float AmbientIntensity;
};

//------------------------------------------------------------------------------------------------
Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);

//------------------------------------------------------------------------------------------------
SamplerState samplerState : register(s0);
SamplerState normalState : register(s1);

//------------------------------------------------------------------------------------------------
PixelInput VertexMain(VertexInput input)
{
	float4 modelPosition = float4(input.modelPosition, 1);
	float4 worldPosition = mul(ModelToWorldTransform, modelPosition);
	float4 cameraPosition = mul(WorldToCameraTransform, worldPosition);
	float4 renderPosition = mul(CameraToRenderTransform, cameraPosition);
	float4 clipPosition = mul(RenderToClipTransform, renderPosition);

	float4 worldTangent = mul(ModelToWorldTransform, float4(input.modelTangent, 0.0f));
	float4 worldBitangent = mul(ModelToWorldTransform, float4(input.modelBitangent, 0.0f));
	float4 worldNormal = mul(ModelToWorldTransform, float4(input.modelNormal, 0.0f));

	PixelInput vsOutput;
	vsOutput.clipPosition = clipPosition;
	vsOutput.color = input.color;
	vsOutput.uv = input.uv;
	vsOutput.worldTangent = worldTangent;
	vsOutput.worldBitangent = worldBitangent;
	vsOutput.worldNormal = worldNormal;
    vsOutput.debugId = DebugID;
	return vsOutput;
}

//------------------------------------------------------------------------------------------------
float4 PixelMain(PixelInput input) : SV_Target0
{
	float ambience = AmbientIntensity;
	float diffuseTexel = SunIntensity * saturate(dot(normalize(input.worldNormal.xyz), -SunDirection));
	float4 lightColor = float4((ambience + diffuseTexel).xxx, 1);
	float4 textureColor = diffuseTexture.Sample(samplerState, input.uv);
	
	float4 vertexColor = input.color;
	float4 modelColor = ModelColor;
	float4 color = lightColor * textureColor * vertexColor * modelColor;
	clip(color.a - 0.01f);
	
	if( input.debugId == 1 )
    {
        color.rgb = input.worldNormal.xyz;
        color.rgb += 1.f;
        color.rgb *= 0.5f;
    }
	else if( input.debugId == 2 )
    {
        color.rgb = input.worldTangent.xyz;
        color.rgb += 1.f;
        color.rgb *= 0.5f;
    }
	else if( input.debugId == 3 )
    {
        color.rgb = input.worldBitangent.xyz;
        color.rgb += 1.f;
        color.rgb *= 0.5f;
    }
	
	//float3 tbnMicroNormal = normalize( normalTexel.rgb * 2.0) - 1
    float3 tangentSpaceNormal = (textureColor.rgb * 2.0) - 1;
	// Transform
    //float3 worldTangent = input.worldTangent;
    //float3 worldBitangent = input.worldBitangent;
    //float3 worldNormal = input.worldNormal;
    //float3x3 tbnToWorld = float3x3(worldTangent, worldBitangent, worldNormal);
	//float3 worldMicroNormal = mul( tbnToWorld, )
	
    float3 worldSpaceNormal = tangentSpaceNormal;
	// Lighting only
    float4 finalColor = float4(diffuseTexel, diffuseTexel, diffuseTexel, color.a);

	
    return color;
}
