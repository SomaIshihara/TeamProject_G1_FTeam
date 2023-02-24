xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 112;
 -11.88800;16.53592;-1.00000;,
 -10.88800;16.53592;-1.00000;,
 -10.88800;-0.69986;-1.00000;,
 -11.88800;-0.69986;-1.00000;,
 -9.88800;16.53592;-1.00000;,
 -9.88800;-0.69986;-1.00000;,
 -9.88800;16.53592;-1.00000;,
 -9.88800;16.53592;0.00000;,
 -9.88800;-0.69986;0.00000;,
 -9.88800;-0.69986;-1.00000;,
 -9.88800;16.53592;1.00000;,
 -9.88800;-0.69986;1.00000;,
 -9.88800;16.53592;1.00000;,
 -10.88800;16.53592;1.00000;,
 -10.88800;-0.69986;1.00000;,
 -9.88800;-0.69986;1.00000;,
 -11.88800;16.53592;1.00000;,
 -11.88800;-0.69986;1.00000;,
 -11.88800;16.53592;1.00000;,
 -11.88800;16.53592;0.00000;,
 -11.88800;-0.69986;0.00000;,
 -11.88800;-0.69986;1.00000;,
 -11.88800;16.53592;-1.00000;,
 -11.88800;-0.69986;-1.00000;,
 -11.88800;-0.69986;0.00000;,
 -10.88800;-0.69986;0.00000;,
 -9.88800;-0.69986;0.00000;,
 -11.88800;-0.69986;-1.00000;,
 -10.88800;-0.69986;-1.00000;,
 -9.88800;-0.69986;-1.00000;,
 4.74500;16.53592;-1.00000;,
 5.74500;16.53592;-1.00000;,
 5.74500;-0.69986;-1.00000;,
 4.74500;-0.69986;-1.00000;,
 6.74500;16.53592;-1.00000;,
 6.74500;-0.69986;-1.00000;,
 6.74500;16.53592;-1.00000;,
 6.74500;16.53592;0.00000;,
 6.74500;-0.69986;-0.00000;,
 6.74500;-0.69986;-1.00000;,
 6.74500;16.53592;1.00000;,
 6.74500;-0.69986;1.00000;,
 6.74500;16.53592;1.00000;,
 5.74500;16.53592;1.00000;,
 5.74500;-0.69986;1.00000;,
 6.74500;-0.69986;1.00000;,
 4.74500;16.53592;1.00000;,
 4.74500;-0.69986;1.00000;,
 4.74500;16.53592;1.00000;,
 4.74500;16.53592;0.00000;,
 4.74500;-0.69986;-0.00000;,
 4.74500;-0.69986;1.00000;,
 4.74500;16.53592;-1.00000;,
 4.74500;-0.69986;-1.00000;,
 4.74500;-0.69986;-0.00000;,
 5.74500;-0.69986;-0.00000;,
 6.74500;-0.69986;-0.00000;,
 4.74500;-0.69986;-1.00000;,
 5.74500;-0.69986;-1.00000;,
 6.74500;-0.69986;-1.00000;,
 -10.88800;19.30014;0.00000;,
 -10.88800;16.53592;-1.00000;,
 -11.88800;16.53592;-1.00000;,
 -11.88800;16.53592;0.00000;,
 -9.88800;16.53592;0.00000;,
 -9.88800;16.53592;-1.00000;,
 5.74500;19.30014;0.00000;,
 6.74500;16.53592;0.00000;,
 6.74500;16.53592;-1.00000;,
 5.74500;16.53592;-1.00000;,
 4.74500;16.53592;-1.00000;,
 4.74500;16.53592;0.00000;,
 -10.72346;12.15191;-1.00000;,
 5.27654;12.15191;-1.00000;,
 5.27654;10.15191;-1.00000;,
 -10.72346;10.15191;-1.00000;,
 5.27654;12.15191;-1.00000;,
 5.27654;12.15191;1.00000;,
 5.27654;10.15191;1.00000;,
 5.27654;10.15191;-1.00000;,
 5.27654;12.15191;1.00000;,
 -10.72346;12.15191;1.00000;,
 -10.72346;10.15191;1.00000;,
 5.27654;10.15191;1.00000;,
 -10.72346;12.15191;1.00000;,
 -10.72346;12.15191;-1.00000;,
 -10.72346;10.15191;-1.00000;,
 -10.72346;10.15191;1.00000;,
 5.27654;12.15191;-1.00000;,
 -10.72346;12.15191;-1.00000;,
 -10.72346;10.15191;-1.00000;,
 5.27654;10.15191;-1.00000;,
 -10.72346;4.67047;-1.00000;,
 5.27654;4.67047;-1.00000;,
 5.27654;2.67046;-1.00000;,
 -10.72346;2.67046;-1.00000;,
 5.27654;4.67047;-1.00000;,
 5.27654;4.67047;1.00000;,
 5.27654;2.67046;1.00000;,
 5.27654;2.67046;-1.00000;,
 5.27654;4.67047;1.00000;,
 -10.72346;4.67047;1.00000;,
 -10.72346;2.67046;1.00000;,
 5.27654;2.67046;1.00000;,
 -10.72346;4.67047;1.00000;,
 -10.72346;4.67047;-1.00000;,
 -10.72346;2.67046;-1.00000;,
 -10.72346;2.67046;1.00000;,
 5.27654;4.67047;-1.00000;,
 -10.72346;4.67047;-1.00000;,
 -10.72346;2.67046;-1.00000;,
 5.27654;2.67046;-1.00000;;
 
 52;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;6,7,8,9;,
 4;7,10,11,8;,
 4;12,13,14,15;,
 4;13,16,17,14;,
 4;18,19,20,21;,
 4;19,22,23,20;,
 4;24,25,14,21;,
 4;25,26,11,14;,
 4;27,28,25,24;,
 4;28,29,26,25;,
 4;30,31,32,33;,
 4;31,34,35,32;,
 4;36,37,38,39;,
 4;37,40,41,38;,
 4;42,43,44,45;,
 4;43,46,47,44;,
 4;48,49,50,51;,
 4;49,52,53,50;,
 4;54,55,44,51;,
 4;55,56,41,44;,
 4;57,58,55,54;,
 4;58,59,56,55;,
 3;60,61,62;,
 3;60,62,63;,
 3;60,64,65;,
 3;60,65,61;,
 3;60,13,10;,
 3;60,10,64;,
 3;60,63,18;,
 3;60,18,13;,
 3;66,43,40;,
 3;66,40,67;,
 3;66,67,68;,
 3;66,68,69;,
 3;66,69,70;,
 3;66,70,71;,
 3;66,71,48;,
 3;66,48,43;,
 4;72,73,74,75;,
 4;76,77,78,79;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;84,77,88,89;,
 4;90,91,78,87;,
 4;92,93,94,95;,
 4;96,97,98,99;,
 4;100,101,102,103;,
 4;104,105,106,107;,
 4;104,97,108,109;,
 4;110,111,98,107;;
 
 MeshMaterialList {
  2;
  52;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  0,
  1,
  0,
  0,
  0,
  1,
  0,
  1,
  0,
  0;;
  Material {
   0.787451;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.492157;0.500000;0.500000;;
   TextureFilename {
    "data\\TEXTURE\\tex_fence.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  25;
  0.000000;0.172689;-0.984976;,
  0.000000;0.172689;-0.984976;,
  0.000000;0.000000;-1.000000;,
  0.984976;0.172689;0.000000;,
  1.000000;0.000000;0.000000;,
  0.984976;0.172689;0.000000;,
  0.000000;0.172689;0.984976;,
  0.000000;0.000000;1.000000;,
  0.000000;0.172689;0.984976;,
  -0.984976;0.172689;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.340189;-0.940357;,
  0.000000;-1.000000;-0.000000;,
  0.984976;0.172689;0.000000;,
  0.000000;0.340189;0.940357;,
  -0.984976;0.172689;0.000000;,
  -0.940357;0.340189;0.000000;,
  0.940357;0.340189;0.000000;,
  0.940357;0.340189;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  52;
  4;0,1,2,2;,
  4;1,0,2,2;,
  4;5,3,4,4;,
  4;3,5,4,4;,
  4;8,6,7,7;,
  4;6,8,7,7;,
  4;15,9,10,10;,
  4;9,15,10,10;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  4;0,1,2,2;,
  4;1,0,2,2;,
  4;13,13,4,4;,
  4;13,13,4,4;,
  4;8,6,7,7;,
  4;6,8,7,7;,
  4;15,9,10,10;,
  4;9,15,10,10;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  3;11,1,0;,
  3;16,15,9;,
  3;17,3,5;,
  3;11,0,1;,
  3;14,6,8;,
  3;17,5,3;,
  3;16,9,15;,
  3;14,8,6;,
  3;14,6,8;,
  3;18,13,13;,
  3;18,13,13;,
  3;11,0,1;,
  3;11,1,0;,
  3;16,15,9;,
  3;16,9,15;,
  3;14,8,6;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;23,23,23,23;,
  4;24,24,24,24;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;23,23,23,23;,
  4;24,24,24,24;;
 }
 MeshTextureCoords {
  112;
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.500000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
