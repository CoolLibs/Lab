#include "RenderEffect_Edges.h"

std::string EdgesParameter(const RenderEffect_Edges& edges)
{
    return "const float edges_size = " + std::to_string(*edges.size) + ";\n\n";
};

std::string Edgesfct()
{
    return R"(
    vec3 edgeColor = vec3(1.0,0.5,0.0);

    float edges(vec3 ro, vec3 rd)
    {
        float EDGE_WIDTH = 0.2;
        float edge       = 1.0;
        float lastdS     = is0_default_sdf(ro);
        float dO         = 0.;
        float steps      = 0.0;
        for (int i = 0; i < MAX_STEPS; i++) {
            steps += 1;
            vec3  p  = ro + rd * dO;
            float dS = is0_default_sdf(p);
            dO += dS;
            if (dO > MAX_DIST || abs(dS) < SURF_DIST) {
                break;
            } 
            if (lastdS < EDGE_WIDTH && dS > lastdS + 0.001) {
                edge = 0.0;
            }
            lastdS = dS;
            
        }
        return edge;
    }

    float edges2(vec3 ro, vec3 rd) {

    float EDGE_WIDTH = 0.1;
    float edge       = 1.0;
    float lastd     = is0_default_sdf(ro);
    float t = 0.;
    for (int i = 0; i < MAX_STEPS; i++) {
    	vec3 pos = ro + rd * t;
        float d = is0_main_sdf(pos);
        t += d;
        // If we are very close to the object, consider it as a hit and exit this loop
        if( t > MAX_DIST || abs(d) < SURF_DIST*0.99) break;
        if (lastd < EDGE_WIDTH+0.04*sqrt(t) && d > lastd + 0.001) {
            edge = 0.0;
        }
        lastd = d;
    }
    return edge;
    }
    )";
};

std::string EdgesAdd()
{
    return R"(
    if(edges2(ro,rd) < 0.7){
        finalCol = edgeColor ;
    }
};