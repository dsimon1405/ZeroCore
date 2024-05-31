#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

// out gl_PerVertex { vec4 gl_Position; };

void main ()
{
	for ( int i = 0; i < gl_in.length (); i++ )
	{
		gl_Position = gl_in [i].gl_Position;
		EmitVertex ();
	}
	
	EndPrimitive ();
}
