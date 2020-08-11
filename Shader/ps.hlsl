struct PS_IN
{
    float4 sPostion : SV_Position;
};

float3 main(PS_IN pin) : SV_Target
{
    return float4(1, 0, 1, 1);
    
}