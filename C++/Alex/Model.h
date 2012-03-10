/* File: Model.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;             */

/*
   The Silicon Graphic Library model format is a light weight, simple and
   flexible format designed for the Open Graphics Library. The format allows
   static models to be streamed promptly within the Open Graphics Library
   framework. This is a high performance implementation and currently only
   supports the GL_V3F, GL_N3F_V3F, GL_T2F_N3F_V3F vertex formats!

   The binary file format is described:
      - The number of elements.
      - The format of the elements (for example GL_V3F), which determines the size of
        each element.
      - The mode of the elements (for example GL_TRIANGLES).
      - The array of data.

   This Implementation is Copyright (c) 2005 Simon Flannery.
 */

#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum {x, y, z, w};

#pragma pack(push)
#pragma pack(1)

struct texture
{
   float u, v;
};

struct vertex
{
// GL_V3F
   float m[3];

   float& operator[](int i)
   { 
      return m[i];
   }

   float operator[](int i) const
   { 
      return m[i];
   }
};

struct color_vertex
{
// GL_C4UB_V3F
   unsigned char r, g, b, a;
   vertex v;
};

struct normal_vertex
{
// GL_N3F_V3F
   vertex n;
   vertex v;
};

struct texture_vertex
{
// GL_T2F_N3F_V3F
   texture t;
   vertex n;
   vertex v;
};

struct object_vertex
{
   vertex  position;
   vertex  normal;
   texture texcoord;
};

#pragma pack(pop)

float my_max(float a, float b)
{
   return (a > b) ? a : b;
}

class Model
{
public:
   virtual bool Load(char* szFileName) = 0;
   virtual void GetDimensions(float& width, float& height, float& depth) = 0;
   virtual void GetCenter(float& cx, float& cy, float& cz) = 0;
   virtual void Unitize() = 0;
   virtual void Scale(float scale) = 0;
   virtual void Translate(float tx, float ty, float tz) = 0;

   virtual void Render() const = 0;

   virtual void Delete() = 0;
};

class Model_sgl : public Model
{
public:
   typedef void (Model_sgl::*p_Model_sgl_member)();
   typedef void (Model_sgl::*p_Model_sgl_member_1f)(float);
   typedef void (Model_sgl::*p_Model_sgl_member_3f)(float, float, float);
   typedef void (Model_sgl::*p_Model_sgl_member_3f_reference)(float&, float&, float&);
   
   virtual bool Load(char* szFileName);
   virtual void GetDimensions(float& width, float& height, float& depth);
   virtual void GetCenter(float& cx, float& cy, float& cz);
   virtual void Unitize();
   virtual void Scale(float scale);
   virtual void Translate(float tx, float ty, float tz);

   virtual void Render() const;

           bool Save(char* szFileName) const;
   virtual void Delete();

   void SetTexture(unsigned int t) {   id = t; return;   }
   unsigned int GetTexture() const {   return id;   }

private:

   void GL_V3F_Dimensions(float& width, float& height, float& depth);
   void GL_N3F_V3F_Dimensions(float& width, float& height, float& depth);
   void GL_T2F_N3F_V3F_Dimensions(float& width, float& height, float& depth);
   void GL_V3F_Center(float& cx, float& cy, float& cz);
   void GL_N3F_V3F_Center(float& cx, float& cy, float& cz);
   void GL_T2F_N3F_V3F_Center(float& cx, float& cy, float& cz);
   void GL_V3F_Unitize();
   void GL_N3F_V3F_Unitize();
   void GL_T2F_N3F_V3F_Unitize();
   void GL_V3F_Scale(float scale);
   void GL_N3F_V3F_Scale(float scale);
   void GL_T2F_N3F_V3F_Scale(float scale);
   void GL_V3F_Translate(float tx, float ty, float tz);
   void GL_N3F_V3F_Translate(float tx, float tt, float tz);
   void GL_T2F_N3F_V3F_Translate(float tx, float ty, float tz);

   void* data;
   int format, count, mode;
   unsigned int id;

   p_Model_sgl_member_3f_reference pDimensions;
   p_Model_sgl_member_3f_reference pCenter;
   p_Model_sgl_member pUnitize;
   p_Model_sgl_member_1f pScale;
   p_Model_sgl_member_3f pTranslate;
};

bool Model_sgl::Load(char* szFileName)
{
   data = NULL; format = count = mode = 0; id = 0;
   pDimensions = NULL;
   pCenter     = NULL;
   pUnitize    = NULL;
   pScale      = NULL;
   pTranslate  = NULL;

   FILE* file = fopen(szFileName, "rb");

   if (file != NULL)
   {
      if (fread(&count, sizeof(int), 1, file) > 0 && count > 0)
      {
         if (fread(&format, sizeof(int), 1, file) > 0 && format > 0)
         {
            if (fread(&mode, sizeof(int), 1, file) > 0 && mode > 0)
            {
               size_t size = 0;

               switch (format) /* Setup member function pointers. */
               {
               case GL_V3F:
                  size = sizeof(vertex);
                  pDimensions = &Model_sgl::GL_V3F_Dimensions;
                  pCenter     = &Model_sgl::GL_V3F_Center;
                  pScale      = &Model_sgl::GL_V3F_Scale;
                  pUnitize    = &Model_sgl::GL_V3F_Unitize;
                  pTranslate  = &Model_sgl::GL_V3F_Translate;
                  break;

               case GL_N3F_V3F:
                  size = sizeof(normal_vertex);
                  pDimensions = &Model_sgl::GL_N3F_V3F_Dimensions;
                  pCenter     = &Model_sgl::GL_N3F_V3F_Center;
                  pScale      = &Model_sgl::GL_N3F_V3F_Scale;
                  pUnitize    = &Model_sgl::GL_N3F_V3F_Unitize;
                  pTranslate  = &Model_sgl::GL_N3F_V3F_Translate;
                  break;

               case GL_T2F_N3F_V3F:
                  size = sizeof(texture_vertex);
                  pDimensions = &Model_sgl::GL_T2F_N3F_V3F_Dimensions;
                  pCenter     = &Model_sgl::GL_T2F_N3F_V3F_Center;
                  pScale      = &Model_sgl::GL_T2F_N3F_V3F_Scale;
                  pUnitize    = &Model_sgl::GL_T2F_N3F_V3F_Unitize;
                  pTranslate  = &Model_sgl::GL_T2F_N3F_V3F_Translate;
                  break;
               }

               if (size != 0)
               {
                  if ((data = malloc(size * count)) != NULL)
                  {
                     if (fread(data, size, count, file) != count)
                     {
                        Delete();
                     }
                  }
               }
            }
         }
      }

      fclose(file);
   }

   return data != NULL;
}

void Model_sgl::GetDimensions(float& width, float& height, float& depth)
{
   if (pDimensions != NULL)
   {
      (this->*pDimensions)(width, height, depth);
   }

   return;
}

void Model_sgl::GetCenter(float& cx, float& cy, float& cz)
{
   if (pCenter != NULL)
   {
      (this->*pCenter)(cx, cy, cz);
   }

   return;
}

void Model_sgl::Unitize()
{
   if (pUnitize != NULL)
   {
      (this->*pUnitize)();
   }

   return;
}

void Model_sgl::Scale(float scale)
{
   if (pScale != NULL)
   {
      (this->*pScale)(scale);
   }

   return;
}

void Model_sgl::Translate(float tx, float ty, float tz)
{
   if (pTranslate != NULL)
   {
      (this->*pTranslate)(tx, ty, tz);
   }

   return;
}

void Model_sgl::Render() const
{
   if (data != NULL)
   {
      glInterleavedArrays(format, 0, data);
      glDrawArrays(mode, 0, count);
   }

   return;
}

bool Model_sgl::Save(char* szFileName) const
{
   if (data != NULL)
   {
      FILE* file = fopen(szFileName, "wb");

      if (file != NULL)
      {
         fwrite(&count,  sizeof(int), 1, file);
         fwrite(&format, sizeof(int), 1, file);
         fwrite(&mode,   sizeof(int), 1, file);

         size_t size = 0;

         switch (format)
         {
         case GL_V3F:
            size = sizeof(vertex);
            break;

         case GL_N3F_V3F:
            size = sizeof(normal_vertex);
            break;

         case GL_T2F_N3F_V3F:
            size = sizeof(texture_vertex);
            break;
         }

         fwrite(data, size, count, file);

         fclose(file);
      }
   }

   return true;
}

void Model_sgl::Delete()
{
   free(data); data = NULL; format = count = mode = 0; id = 0;

   pDimensions = NULL;
   pCenter     = NULL;
   pUnitize    = NULL;
   pScale      = NULL;
   pTranslate  = NULL;

   return;
}

void Model_sgl::GL_V3F_Dimensions(float& width, float& height, float& depth)
{
   width = height = depth = 0.0f;
   float maxx = 0.0f, minx = 0.0f, maxy = 0.0f, miny = 0.0f, maxz = 0.0f, minz = 0.0f;

   if (data != NULL)
   {
   /* Get the maximum and minimum. */
      vertex* v = (vertex*) data;

      maxx = minx = v[0][x];
      maxy = miny = v[0][y];
      maxz = minz = v[0][z];
      
      for (int i = 0; i < count; ++i)
      {
         if (maxx < v[i][x]) {   maxx = v[i][x];   }
         if (minx > v[i][x]) {   minx = v[i][x];   }
         if (maxy < v[i][y]) {   maxy = v[i][y];   }
         if (miny > v[i][y]) {   miny = v[i][y];   }
         if (maxz < v[i][z]) {   maxz = v[i][z];   }
         if (minz > v[i][z]) {   minz = v[i][z];   }
      }

   /* Calculate model width, height, and depth. */
      width  = fabs(maxx) + fabs(minx);
      height = fabs(maxy) + fabs(miny);
      depth  = fabs(maxz) + fabs(minz);
   }

   return;
}

void Model_sgl::GL_N3F_V3F_Dimensions(float& width, float& height, float& depth)
{
   width = height = depth = 0.0f;

   float maxx = 0.0f, minx = 0.0f, maxy = 0.0f, miny = 0.0f, maxz = 0.0f, minz = 0.0f;

   if (data != NULL)
   {
   /* Get the maximum and minimum. */
      normal_vertex* v = (normal_vertex*) data;
      
      maxx = minx = v[0].v[x];
      maxy = miny = v[0].v[y];
      maxz = minz = v[0].v[z];
      
      for (int i = 0; i < count; ++i)
      {
         if (maxx < v[i].v[x]) {   maxx = v[i].v[x];   }
         if (minx > v[i].v[x]) {   minx = v[i].v[x];   }
         if (maxy < v[i].v[y]) {   maxy = v[i].v[y];   }
         if (miny > v[i].v[y]) {   miny = v[i].v[y];   }
         if (maxz < v[i].v[z]) {   maxz = v[i].v[z];   }
         if (minz > v[i].v[z]) {   minz = v[i].v[z];   }
      }

   /* Calculate model width, height, and depth. */
      width  = fabs(maxx) + fabs(minx);
      height = fabs(maxy) + fabs(miny);
      depth  = fabs(maxz) + fabs(minz);
   }

   return;
}

void Model_sgl::GL_T2F_N3F_V3F_Dimensions(float& width, float& height, float& depth)
{
   width = height = depth = 0.0f;

   float maxx = 0.0f, minx = 0.0f, maxy = 0.0f, miny = 0.0f, maxz = 0.0f, minz = 0.0f;

   if (data != NULL)
   {
   /* Get the maximum and minimum. */
      texture_vertex* v = (texture_vertex*) data;

      maxx = minx = v[0].v[x];
      maxy = miny = v[0].v[y];
      maxz = minz = v[0].v[z];

      for (int i = 0; i < count; ++i)
      {
         if (maxx < v[i].v[x]) {   maxx = v[i].v[x];   }
         if (minx > v[i].v[x]) {   minx = v[i].v[x];   }
         if (maxy < v[i].v[y]) {   maxy = v[i].v[y];   }
         if (miny > v[i].v[y]) {   miny = v[i].v[y];   }
         if (maxz < v[i].v[z]) {   maxz = v[i].v[z];   }
         if (minz > v[i].v[z]) {   minz = v[i].v[z];   }
      }

   /* Calculate model width, height, and depth. */
      width  = fabs(maxx) + fabs(minx);
      height = fabs(maxy) + fabs(miny);
      depth  = fabs(maxz) + fabs(minz);
   }

   return;
}

void Model_sgl::GL_V3F_Center(float& cx, float& cy, float& cz)
{
   float maxx = 0.0f, minx = 0.0f, maxy = 0.0f, miny = 0.0f, maxz = 0.0f, minz = 0.0f;

   if (data != NULL)
   {
   /* Get the maximum and minimum. */
      vertex* v = (vertex*) data;

      maxx = minx = v[0][x];
      maxy = miny = v[0][y];
      maxz = minz = v[0][z];
      
      for (int i = 0; i < count; ++i)
      {
         if (maxx < v[i][x]) {   maxx = v[i][x];   }
         if (minx > v[i][x]) {   minx = v[i][x];   }
         if (maxy < v[i][y]) {   maxy = v[i][y];   }
         if (miny > v[i][y]) {   miny = v[i][y];   }
         if (maxz < v[i][z]) {   maxz = v[i][z];   }
         if (minz > v[i][z]) {   minz = v[i][z];   }
      }

   /* Calculate model width, height, and depth. */
      float width  = fabs(maxx) + fabs(minx);
      float height = fabs(maxy) + fabs(miny);
      float depth  = fabs(maxz) + fabs(minz);
       
   /* Calculate center of the model. */
      cx = (maxx + minx) / 2.0f;
      cy = (maxy + miny) / 2.0f;
      cz = (maxz + minz) / 2.0f;
   }

   return;
}

void Model_sgl::GL_N3F_V3F_Center(float& cx, float& cy, float& cz)
{
   float maxx = 0.0f, minx = 0.0f, maxy = 0.0f, miny = 0.0f, maxz = 0.0f, minz = 0.0f;

   if (data != NULL)
   {
   /* Get the maximum and minimum. */
      normal_vertex* v = (normal_vertex*) data;

      maxx = minx = v[0].v[x];
      maxy = miny = v[0].v[y];
      maxz = minz = v[0].v[z];
      
      for (int i = 0; i < count; ++i)
      {
         if (maxx < v[i].v[x]) {   maxx = v[i].v[x];   }
         if (minx > v[i].v[x]) {   minx = v[i].v[x];   }
         if (maxy < v[i].v[y]) {   maxy = v[i].v[y];   }
         if (miny > v[i].v[y]) {   miny = v[i].v[y];   }
         if (maxz < v[i].v[z]) {   maxz = v[i].v[z];   }
         if (minz > v[i].v[z]) {   minz = v[i].v[z];   }
      }

   /* Calculate model width, height, and depth. */
      float width  = fabs(maxx) + fabs(minx);
      float height = fabs(maxy) + fabs(miny);
      float depth  = fabs(maxz) + fabs(minz);
       
   /* Calculate center of the model. */
      cx = (maxx + minx) / 2.0f;
      cy = (maxy + miny) / 2.0f;
      cz = (maxz + minz) / 2.0f;
   }

   return;
}

void Model_sgl::GL_T2F_N3F_V3F_Center(float& cx, float& cy, float& cz)
{
   float maxx = 0.0f, minx = 0.0f, maxy = 0.0f, miny = 0.0f, maxz = 0.0f, minz = 0.0f;

   if (data != NULL)
   {
   /* Get the maximum and minimum. */
      texture_vertex* v = (texture_vertex*) data;

      maxx = minx = v[0].v[x];
      maxy = miny = v[0].v[y];
      maxz = minz = v[0].v[z];
      
      for (int i = 0; i < count; ++i)
      {
         if (maxx < v[i].v[x]) {   maxx = v[i].v[x];   }
         if (minx > v[i].v[x]) {   minx = v[i].v[x];   }
         if (maxy < v[i].v[y]) {   maxy = v[i].v[y];   }
         if (miny > v[i].v[y]) {   miny = v[i].v[y];   }
         if (maxz < v[i].v[z]) {   maxz = v[i].v[z];   }
         if (minz > v[i].v[z]) {   minz = v[i].v[z];   }
      }

   /* Calculate model width, height, and depth. */
      float width  = fabs(maxx) + fabs(minx);
      float height = fabs(maxy) + fabs(miny);
      float depth  = fabs(maxz) + fabs(minz);
       
   /* Calculate center of the model. */
      cx = (maxx + minx) / 2.0f;
      cy = (maxy + miny) / 2.0f;
      cz = (maxz + minz) / 2.0f;
   }

   return;
}

void Model_sgl::GL_V3F_Unitize()
{
   float maxx = 0.0f, minx = 0.0f, maxy = 0.0f, miny = 0.0f, maxz = 0.0f, minz = 0.0f;

   if (data != NULL)
   {
   /* Get the maximum and minimum. */
      vertex* v = (vertex*) data;

      maxx = minx = v[0][x];
      maxy = miny = v[0][y];
      maxz = minz = v[0][z];
      
      for (int i = 0; i < count; ++i)
      {
         if (maxx < v[i][x]) {   maxx = v[i][x];   }
         if (minx > v[i][x]) {   minx = v[i][x];   }
         if (maxy < v[i][y]) {   maxy = v[i][y];   }
         if (miny > v[i][y]) {   miny = v[i][y];   }
         if (maxz < v[i][z]) {   maxz = v[i][z];   }
         if (minz > v[i][z]) {   minz = v[i][z];   }
      }

   /* Calculate model width, height, and depth. */
      float width  = fabs(maxx) + fabs(minx);
      float height = fabs(maxy) + fabs(miny);
      float depth  = fabs(maxz) + fabs(minz);
       
   /* Calculate center of the model. */
      float cx = (maxx + minx) / 2.0f;
      float cy = (maxy + miny) / 2.0f;
      float cz = (maxz + minz) / 2.0f;

   /* Calculate unitizing scale factor. */
      float scale = 2.0f / my_max(my_max(width, height), depth);

   /* Translate around center then scale. */
      v = (vertex*) data;
      
      for (int i = 0; i < count; ++i)
      {
         v[i][x] -= cx;
         v[i][y] -= cy;
         v[i][z] -= cz;
         v[i][x] *= scale;
         v[i][y] *= scale;
         v[i][z] *= scale;
      }
   }

   return;
}

void Model_sgl::GL_N3F_V3F_Unitize()
{
   float maxx = 0.0f, minx = 0.0f, maxy = 0.0f, miny = 0.0f, maxz = 0.0f, minz = 0.0f;

   if (data != NULL)
   {
   /* Get the maximum and minimum. */
      normal_vertex* v = (normal_vertex*) data;
      
      maxx = minx = v[0].v[x];
      maxy = miny = v[0].v[y];
      maxz = minz = v[0].v[z];
      
      for (int i = 0; i < count; ++i)
      {
         if (maxx < v[i].v[x]) {   maxx = v[i].v[x];   }
         if (minx > v[i].v[x]) {   minx = v[i].v[x];   }
         if (maxy < v[i].v[y]) {   maxy = v[i].v[y];   }
         if (miny > v[i].v[y]) {   miny = v[i].v[y];   }
         if (maxz < v[i].v[z]) {   maxz = v[i].v[z];   }
         if (minz > v[i].v[z]) {   minz = v[i].v[z];   }
      }

   /* Calculate model width, height, and depth. */
      float width  = fabs(maxx) + fabs(minx);
      float height = fabs(maxy) + fabs(miny);
      float depth  = fabs(maxz) + fabs(minz);
       
   /* Calculate center of the model. */
      float cx = (maxx + minx) / 2.0f;
      float cy = (maxy + miny) / 2.0f;
      float cz = (maxz + minz) / 2.0f;

   /* Calculate unitizing scale factor. */
      float scale = 2.0f / my_max(my_max(width, height), depth);

   /* Translate around center then scale. */
      v = (normal_vertex*) data;
      
      for (int i = 0; i < count; ++i)
      {
         v[i].v[x] -= cx;
         v[i].v[y] -= cy;
         v[i].v[z] -= cz;
         v[i].v[x] *= scale;
         v[i].v[y] *= scale;
         v[i].v[z] *= scale;
      }
   }

   return;
}

void Model_sgl::GL_T2F_N3F_V3F_Unitize()
{
   float maxx = 0.0f, minx = 0.0f, maxy = 0.0f, miny = 0.0f, maxz = 0.0f, minz = 0.0f;

   if (data != NULL)
   {
   /* Get the maximum and minimum. */
      texture_vertex* v = (texture_vertex*) data;

      maxx = minx = v[0].v[x];
      maxy = miny = v[0].v[y];
      maxz = minz = v[0].v[z];

      for (int i = 0; i < count; ++i)
      {
         if (maxx < v[i].v[x]) {   maxx = v[i].v[x];   }
         if (minx > v[i].v[x]) {   minx = v[i].v[x];   }
         if (maxy < v[i].v[y]) {   maxy = v[i].v[y];   }
         if (miny > v[i].v[y]) {   miny = v[i].v[y];   }
         if (maxz < v[i].v[z]) {   maxz = v[i].v[z];   }
         if (minz > v[i].v[z]) {   minz = v[i].v[z];   }
      }

   /* Calculate model width, height, and depth. */
      float width  = fabs(maxx) + fabs(minx);
      float height = fabs(maxy) + fabs(miny);
      float depth  = fabs(maxz) + fabs(minz);
       
   /* Calculate center of the model. */
      float cx = (maxx + minx) / 2.0f;
      float cy = (maxy + miny) / 2.0f;
      float cz = (maxz + minz) / 2.0f;

   /* Calculate unitizing scale factor. */
      float scale = 2.0f / my_max(my_max(width, height), depth);

   /* Translate around center then scale. */
      v = (texture_vertex*) data;
      
      for (int i = 0; i < count; ++i)
      {
         v[i].v[x] -= cx;
         v[i].v[y] -= cy;
         v[i].v[z] -= cz;
         v[i].v[x] *= scale;
         v[i].v[y] *= scale;
         v[i].v[z] *= scale;
      }
   }

   return;
}

void Model_sgl::GL_V3F_Scale(float scale)
{
   if (data != NULL)
   {
      vertex* v = (vertex*) data;
      
      for (int i = 0; i < count; ++i)
      {
         v[i][x] *= scale;
         v[i][y] *= scale;
         v[i][z] *= scale;
      }
   }

   return;
}

void Model_sgl::GL_N3F_V3F_Scale(float scale)
{
   if (data != NULL)
   {
      normal_vertex* v = (normal_vertex*) data;
      
      for (int i = 0; i < count; ++i)
      {
         v[i].v[x] *= scale;
         v[i].v[y] *= scale;
         v[i].v[z] *= scale;
      }
   }

   return;
}

void Model_sgl::GL_T2F_N3F_V3F_Scale(float scale)
{
   if (data != NULL)
   {
      texture_vertex* v = (texture_vertex*) data;
      
      for (int i = 0; i < count; ++i)
      {
         v[i].v[x] *= scale;
         v[i].v[y] *= scale;
         v[i].v[z] *= scale;
      }
   }

   return;
}

void Model_sgl::GL_V3F_Translate(float tx, float ty, float tz)
{
   if (data != NULL)
   {
      vertex* v = (vertex*) data;
      
      for (int i = 0; i < count; ++i)
      {
         v[i][x] += tx;
         v[i][y] += ty;
         v[i][z] += tz;
      }
   }

   return;
}

void Model_sgl::GL_N3F_V3F_Translate(float tx, float ty, float tz)
{
   if (data != NULL)
   {
      normal_vertex* v = (normal_vertex*) data;

      for (int i = 0; i < count; ++i)
      {
         v[i].v[x] += tx;
         v[i].v[y] += ty;
         v[i].v[z] += tz;
      }
   }

   return;
}

void Model_sgl::GL_T2F_N3F_V3F_Translate(float tx, float ty, float tz)
{
   if (data != NULL)
   {
      texture_vertex* v = (texture_vertex*) data;

      for (int i = 0; i < count; ++i)
      {
         v[i].v[x] += tx;
         v[i].v[y] += ty;
         v[i].v[z] += tz;
      }
   }

   return;
}

#endif
