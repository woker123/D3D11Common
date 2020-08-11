struct VS_IN
{
    float3 vPostion : V_POSITION;
};

struct VS_OUT
{
    float4 sPostion : SV_Position;
};


VS_OUT main(VS_IN vin)
{
    VS_OUT vo;
    vo.sPostion = float4(vin.vPostion, 1);
    return vo;   
}