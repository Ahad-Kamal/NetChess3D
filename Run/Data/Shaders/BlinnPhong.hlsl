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
	float3 worldTangent : TANGENT;
	float3 worldBitangent : BITANGENT;
	float3 worldNormal : NORMAL;
};

//------------------------------------------------------------------------------------------------
cbuffer PerFrameConstants : register( b1 )
{
    float Time;
    int DebugID;
    float2 PER_FRAME_EMPTY_PADDING;
};

//------------------------------------------------------------------------------------------------
cbuffer CameraConstants : register( b2 )
{
	float4x4 WorldToCameraTransform;	// View transform
	float4x4 CameraToRenderTransform;	// Non-standard transform from game to DirectX conventions
	float4x4 RenderToClipTransform;		// Projection transform
};

//------------------------------------------------------------------------------------------------
cbuffer ModelConstants : register( b3 )
{
	float4x4 ModelToWorldTransform;		// Model transform
	float4 ModelColor;
};

//------------------------------------------------------------------------------------------------
cbuffer LightConstants : register( b8 )
{
    float3 SunDirection;
    float SunIntensity;
    float AmbientIntensity;
	float3 LIGHT_EMPTY_PADDING;
};

//------------------------------------------------------------------------------------------------
Texture2D diffuseTexture : register( t0 );
Texture2D normalTexture : register( t1 );

//------------------------------------------------------------------------------------------------
SamplerState diffuseSampler : register( s0 );
SamplerState normalSampler : register( s1 );

//-----------------------------------------------------------------------------------------------
float3 EncodeXYZtoRGB( float3 xyzInput )
{
	return ( xyzInput + 1.f ) * 0.5f;
}

//-----------------------------------------------------------------------------------------------
float3 DecodeRGBtoXYZ( float3 rgbInput )
{
	return ( rgbInput * 2.f ) - 1.f;
}

//------------------------------------------------------------------------------------------------
PixelInput VertexMain( VertexInput input )
{
	float4 modelPosition = float4( input.modelPosition, 1 );
	float4 worldPosition = mul( ModelToWorldTransform, modelPosition );
	float4 cameraPosition = mul( WorldToCameraTransform, worldPosition );
	float4 renderPosition = mul( CameraToRenderTransform, cameraPosition );
	float4 clipPosition = mul( RenderToClipTransform, renderPosition );

	float3 worldTangent = mul( ModelToWorldTransform, float4( input.modelTangent, 0.0f ) ).xyz;
	float3 worldBitangent = mul( ModelToWorldTransform, float4( input.modelBitangent, 0.0f ) ).xyz;
	float3 worldNormal = mul( ModelToWorldTransform, float4( input.modelNormal, 0.0f ) ).xyz;

	PixelInput vsOutput;
	vsOutput.clipPosition = clipPosition;
	vsOutput.color = input.color;
	vsOutput.uv = input.uv;
	vsOutput.worldTangent = worldTangent;
	vsOutput.worldBitangent = worldBitangent;
	vsOutput.worldNormal = worldNormal;
	return vsOutput;
}

//------------------------------------------------------------------------------------------------
float4 PixelMain( PixelInput input ) : SV_Target0
{
	// Get samples
	float4 diffuseTexel = diffuseTexture.Sample( diffuseSampler, input.uv );
    float4 normalTexel = normalTexture.Sample( normalSampler, input.uv );
	
	// Diffuse color
	float4 surfaceColor = input.color;
	float4 modelColor = ModelColor;
	float4 diffuseColor = diffuseTexel * surfaceColor * modelColor;

	// TBN to World
    float3 worldTangent = normalize( input.worldTangent );
    float3 worldBitangent = normalize( input.worldBitangent );
    float3 worldNormal = normalize( input.worldNormal );
    float3x3 tbnToWorld = float3x3( worldTangent, worldBitangent, worldNormal );

	// Micro normals	
	float3 tbnMicroNormal = normalize( DecodeRGBtoXYZ( normalTexel.rgb ) );
    float3 worldMicroNormal = mul( tbnMicroNormal, tbnToWorld );	
	
	// Lighting
    float ambientLight = AmbientIntensity;
    float directionalLight = SunIntensity * saturate( dot( worldMicroNormal, -SunDirection ) );
    float4 lightColor = float4( ( ambientLight + directionalLight ).xxx, 1 );
	
    // Final Color
    float4 finalColor = diffuseColor * lightColor;
	
	// Debug modes
	if( DebugID == 1 )
    {
		finalColor = diffuseTexel;
    }
    else if( DebugID == 2 )
    {
		finalColor = normalTexel;
    }
    else if( DebugID == 3 )
    {
		finalColor.rgb = float3( input.uv.x, input.uv.y, 0 );
    }
    else if( DebugID == 4 )
    {
		finalColor.rgb = EncodeXYZtoRGB( input.worldTangent.xyz );
    }
    else if( DebugID == 5 )
    {
		finalColor.rgb = EncodeXYZtoRGB( input.worldBitangent.xyz );
    }
    else if( DebugID == 6 )
    {
        finalColor.rgb = EncodeXYZtoRGB( input.worldNormal.xyz );
    }
    else if( DebugID == 7 )
    {
        finalColor.rgb = EncodeXYZtoRGB( tbnMicroNormal.xyz );
    }
    else if( DebugID == 8 )
    {
        float directionalLight = SunIntensity * saturate( dot( input.worldNormal, -SunDirection ) );
        float4 lightColor = float4( ( ambientLight + directionalLight ).xxx, 1 );
        finalColor.rgb = EncodeXYZtoRGB( lightColor.xyz );
    }
    else if( DebugID == 9 )
    {
        finalColor.rgb = EncodeXYZtoRGB( lightColor.xyz );
    }
	
	clip( finalColor.a - 0.01f );
    return finalColor;
}