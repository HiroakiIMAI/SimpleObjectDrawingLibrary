#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma comment( lib, "assimp.lib" )



#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

namespace asmpGlUtil
{
	void color4_to_float4(const aiColor4D *c, float f[4]);
	void set_float4(float f[4], float a, float b, float c, float d);
	void apply_material(const aiMaterial *mtl);
	void recursive_render(const aiScene *sc, const aiNode* nd);
};


	