Shaders
=======

GLSL shaders. For demo check out demo_shadercomp or demo_uniform

esShaderLoad
------------

Create a shader program from two shader files

 * *param 1* **esShader*** *shader*  
self

 * *param 2* **const char*** *vert_file*  
Vertex shader file path

 * *param 3* **const char*** *frag_file*  
Fragment shader file path

**return** *int* 0 for success

esShaderUse
-----------

Activate a shader to use

 * *param 1* **esShader*** *shader*  
self

esShaderUnload
--------------

Free a shader

 * *param 1* **esShader*** *shader*  
self

esShaderUniformRegister
-----------------------

Register an uniform constant. You register an uniform to a register specified in the parameter **reg**

 * *param 1* **esShader*** *shader*  
self

 * *param 2* **esUniform** *reg*  
Register to put the uniform location in (Max 10 registered constants

 * *param 3* **const char*** *name*  
Name of the uniform in the GLSL shader

**return** *int* 0 for success

esShaderUniformGl
-----------------

Getting the OpenGL location of a registered uniform

Geometry buffers
================

OpenGL vertex arrays. For a demo of geometry check out demo_red or demo_stride

esGeoBufCreate
--------------

Generate a vertex buffer

 * *param 1* **esGeoBuf** *buf*  
self

esGeoBufCopy
------------

Copy vertex data into buffer

 * *param 1* **esGeoBuf** *buf*  
self

 * *param 2* **const void*** *data*  
Pointer to vertex data

 * *param 3* **size_t** *size*  
Total size of vertex data

 * *param 4* **enum esGeoBufType** *type*  
Type of persistance of buffer data. Read about glBufferData to get a betteer understanding

esGeoBufDelete
--------------

Free a buffer

 * *param 1* **esGeoBuf** *buf*  
self

Geometry
========

Check out demo_red or demo_stride

esGeoReset
----------

Resets a geometry structure

 * *param 1* **esGeo** *geo*  
self

 * *param 2* **int** *bufcount*  
Amount of vertex pointer count

esGeoPoint
----------

Wrapper for glVertexPointer. Settings for a GLSL attribute

 * *param 1* **esGeo** *geo*  
self

 * *param 2* **int** *id*  
Attribute buffer id

 * *param 3* **esGeoBuf*** *geobuf*  
Buffer structure which the pointer will be using

 * *param 4* **enum esGeoDataType** *datatype*  
Datatype of data

 * *param 5* **int** *elements*  
Amount of data elements

 * *param 6* **size_t** *offset*  
Offset to data if it's in a unpacked datastructure

 * *param 7* **size_t** *stride*  
Size of unpacket datastructure

 * *param 8* **enum esBool** *normalized*  
Will the data be within -1.0 to 1.0 in GLSL?

esGeoRender
-----------

Render geometry

 * *param 1* **esGeo** *geo*  
self

 * *param 2* **int** *vertices*  
Amount of vertices to render

Projection
==========

Tools for generating opengl compatible 4x4 float matrices. Demo in demo_cam and demo_mainloop

esProjOrtho
-----------

Procude a 2D orthogonal matrix

 * *param 1* **float[16]** *mat*  
Destination to put matrix

 * *param 2* **float** *x0*  
Right border

 * *param 3* **float** *y0*  
Top border

 * *param 4* **float** *x1*  
Left border

 * *param 5* **float** *y1*  
Bottom border

esProjPerspective
-----------------

Procude a 3D perspective matrix

 * *param 1* **float[16]** *mat*  
Destination to put matrix

 * *param 2* **float** *fov*  
Field of view (radianses)

 * *param 3* **float** *screenratio*  
Ratio of screen (width / height)

 * *param 4* **float** *near*  
Near field

 * *param 5* **float** *far*  
Far field

 * *param 6* **esVec3** *eye*  
Position of camera

 * *param 7* **esVec3** *at*  
Target of camera

 * *param 8* **esVec3** *up*  
Up vector

Texture
=======

Texture loading. Demo in demo_image

esTextureLoad
-------------

Load a texture from file

 * *param 1* **esTexture*** *tex*  
self

 * *param 2* **const char*** *file_name*  
Path to image file

 * *param 3* **enum esTextureMipmap** *min*  
Minification filter

 * *param 4* **enum esTextureMipmap** *mag*  
Magnification filter

**return** *int* 0 for success

esTextureUse
------------

Bind a texture

 * *param 1* **esTexture*** *tex*  
self

esTextureUnload
---------------

Delete a texture

 * *param 1* **esTexture*** *tex*  
self

