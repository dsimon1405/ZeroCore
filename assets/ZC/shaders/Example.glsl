

                    //  SSBO draw GL_POINTS

struct Particle    //  std 430 don't use mat and vec types!
{
    float secs_to_start;

    float pos_start[3];
    float pos_cur[3];
    
    float life_secs_total;
    float life_secs_cur;

    float dir_move_normalized[3];
};
layout (std430, binding = 0) readonly buffer InParticle
{
        //  shared block
    float x1[3];
    float x2[2];
    float x3[3];
        //  individual block
    Particle particles[];
} inParticle;

        //  VERTEX SHADER
    //  out
layout (location = 0) out OutV
{
    float x1[3];
    float x2[2];
    float x3[3];
    Particle p;
} outV;
void main()
{
    for (int i; i < 3; ++i) outV.x1[i] = inParticle.x1[i];
    for (int i; i < 2; ++i) outV.x2[i] = inParticle.x2[i];
    for (int i; i < 3; ++i) outV.x3[i] = inParticle.x3[i];
    outV.p = inParticle.particles[gl_VertexID];    //  think that same result will be for lines and triangles if struct like Particle will consist vertex data
}

        //  GEOMETRY SHADER

layout (location = 0) in InG    //  ALL VARIABLES MUST BE INVOLVED INTO THE SOME FUCNTION OR GONNA BE PROBLEM WITH ALIGNMENT (I THOUGHT THAT WITH ALIGNMENT. DATA AFTER NOT INVOLVED FIELD IS NOT CORRECT)
{
    float x1[3];
    float x2[2];
    float x3[3];
    Particle p;
} inG[];
    //  out
layout (location = 0) out OutG
{
    float life_time_alpha;
    vec2 tex_coords;
    float x1[3];
    float x2[2];
    float x3[3];
    Particle p;
} outG;
void main()
{
    for (int i; i < 3; ++i) outG.x1[i] = inG[0].x1[i];
    for (int i; i < 2; ++i) outG.x2[i] = inG[0].x2[i];
    for (int i; i < 3; ++i) outG.x3[i] = inG[0].x3[i];
    
        //  gl_PrimitiveIDIn is primitive, not vertex id. Such way will be work if drawing GL_POINTS and ssbo array store data of each point.
    outG.p = inParticle.particles[gl_PrimitiveIDIn];    //  get from ssbo
        //  or
    outG.p = inG[0].p;      //  get from in

    //  TO DRAWING PRIMITIVE WITH MORE VERTICES THEN JUST A POINT, NEED TO CHECK HOW IT WILL BE WORK, AND WHAT YOU ACTUALY STORE IN SSBO, AND WHAT YOU NEED TO DO IN GEOMETRY SHADER
}

//  FRAEGMENT SHADER, i think that from ssbo may be need only shared block (look Particle), everything else must be transfered at out/in block at the previous stages