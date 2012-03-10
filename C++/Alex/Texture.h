/* File: Texture.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;          */

#ifndef TEXTURE_H
#define TEXTURE_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>   /* Must Include before GL/gl.h. */
#include <GL/gl.h>     /* OpenGL. */
#include <GL/glu.h>    /* OpenGL utilities. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BITMAP_ID 0x4D42

#pragma pack(push)
#pragma pack(1)

struct TGAINFOHEADER
{
   BYTE idLength;
   BYTE colorMapType;
   BYTE imageType;
   BYTE colorMapSpec[5];
   WORD xOrigin;
   WORD yOrigin;
   WORD width;
   WORD height;
   BYTE bpp;
   BYTE imageDesc;
};

#pragma pack(pop)

class Texture2d
{
public:
   void Load(const char* szFileName, bool bAlpha);
   void LoadWithBump(const char* szFileName, const char* szBumpName);
   void Delete();

   void Enable();
   void Disable();
   void Bind();

   unsigned int GetId() {   return id;   }

protected:
   static bool IsExt(const char* source, const char* target);

   static bool CheckSize(int w, int h);

   inline static int GetClosestPowerOfTwo(int x)
   {
      int k = x;
      int i = -1;

      while (k != 0)
      {
         k >>= 1;
         i++;
      }

      return 1 << (i + ((x >> (i - 1)) & 1));
   }

private:

   static BYTE* LoadTGAIntoRGB(const char* szFileName, TGAINFOHEADER* pHeader);
   static BYTE* LoadTGAIntoRGBA(const char* szFileName, TGAINFOHEADER* pHeader);

   static BYTE* LoadBMPIntoRGB(const char* szFileName, BITMAPINFOHEADER* pHeader);
   static BYTE* LoadBMPIntoRGBA(const char* szFileName, BITMAPINFOHEADER* pHeader);

   unsigned int id;
};

/* Load a bmp or tga file and log the openGL texture ID. */
void Texture2d::Load(const char* szFileName, bool bAlpha)
{
   if (IsExt(szFileName, "tga") != false) /* TGA Support. */
   {
      BYTE* tgaData = NULL;
      TGAINFOHEADER tgaHeader = {0};

      if (bAlpha == false)
      {
         if ((tgaData = LoadTGAIntoRGB(szFileName, &tgaHeader)) != NULL)
         {
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, tgaHeader.width, tgaHeader.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tgaData);

            free(tgaData);
         }
      }
      else
      {
         if ((tgaData = LoadTGAIntoRGBA(szFileName, &tgaHeader)) != NULL)
         {
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 4, tgaHeader.width, tgaHeader.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tgaData);

            free(tgaData);
         }
      }
   }
   else if (IsExt(szFileName, "bmp") != false) /* BMP Support. */
   {
      BYTE* bmpData = NULL;
      BITMAPINFOHEADER bmpHeader = {0};

      if (bAlpha == false)
      {
         if ((bmpData = LoadBMPIntoRGB(szFileName, &bmpHeader)) != NULL)
         {
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, bmpHeader.biWidth, bmpHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bmpData);

            free(bmpData);
         }
      }
      else
      {
         if ((bmpData = LoadBMPIntoRGBA(szFileName, &bmpHeader)) != NULL)
         {
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 4, bmpHeader.biWidth, bmpHeader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmpData);

            free(bmpData);
         }
      }
   }

   return;
}

/* Load Base Texture and Bump for ONE pass rendering. Log the openGL Texture ID. */
/* ONLY THE BMP FORMAT IS SUPPORTED. */
void Texture2d::LoadWithBump(const char* szFileName, const char* szBumpName)
{
   BYTE* bmpData = NULL;
   BYTE* bupData = NULL;
   BITMAPINFOHEADER bmpHeader = {0};
   BITMAPINFOHEADER bupHeader = {0};

   if (IsExt(szFileName, "bmp") != false)
   {
      if ((bmpData = LoadBMPIntoRGB(szFileName, &bmpHeader)) != NULL)
      {
         if ((bupData = LoadBMPIntoRGB(szBumpName, &bupHeader)) != NULL)
         {
            if (bmpHeader.biWidth == bupHeader.biWidth && bmpHeader.biHeight == bupHeader.biHeight)
            {
               BYTE* p = (BYTE*) malloc(4 * (bmpHeader.biSizeImage / 3));

               for (size_t i = 0, j = 0; i < bmpHeader.biSizeImage; i += 3, j += 4)
               {
                  p[j + 0] = (BYTE) (bmpData[i + 0]);
                  p[j + 1] = (BYTE) (bmpData[i + 1]);
                  p[j + 2] = (BYTE) (bmpData[i + 2]);	
                  p[j + 3] = (BYTE) ((bupData[i + 0] + bupData[i + 1] + bupData[i + 2]) / 6.0f);
               }

               glGenTextures(1, &id);
               glBindTexture(GL_TEXTURE_2D, id);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
               glTexImage2D(GL_TEXTURE_2D, 0, 4, bmpHeader.biWidth, bmpHeader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, p);
            }

            free(bupData);
         }

         free(bmpData);
      }
   }

   return;
}

/* Texture Cleanup. */
void Texture2d::Delete()
{
   glDeleteTextures(1, &id);

   return;
}

/* Enable GL_TEXTURE_2D. */
void Texture2d::Enable()
{
   glEnable(GL_TEXTURE_2D);

   return;
}

/* Disable GL_TEXTURE_2D. */
void Texture2d::Disable()
{
   glDisable(GL_TEXTURE_2D);

   return;
}

/* Bind this texture id to GL_TEXTURE_2D. */
void Texture2d::Bind()
{
   glBindTexture(GL_TEXTURE_2D, id);

   return;
}

/* Returns true if the source file name ends with target, else returns false. */
bool Texture2d::IsExt(const char* source, const char* target)
{
   if (source != NULL)
   {
      const char* str = source;
      int len = (int) strlen(target);
   
      while ((str = strstr(str, target)) != NULL)
      {
         str += len;
      
         if (*str == ' ' || *str == '\0')
         {
            return true;
         }
      }
   }

   return false;
}

/* Checks if the image has dimensions that are a POWER of TWO. */
bool Texture2d::CheckSize(int w, int h)
{
   float max_texture_size = 0.0f;
   bool bResult = false;

   glGetFloatv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

   if (w <= max_texture_size && h <= max_texture_size)
   {
      if (GetClosestPowerOfTwo(w) == w && GetClosestPowerOfTwo(h) == h)
      {
         bResult = true;
      }
   }

   return bResult;
}

/* Returns a pointer to the tga image data of the tga specified
   by filename. Also returns the tga header information. */
BYTE* Texture2d::LoadTGAIntoRGB(const char* szFileName, TGAINFOHEADER* pHeader)
{
   BYTE* tgaImage = NULL;
   FILE* file = fopen(szFileName, "rb");

   if (file != NULL)
   {
      fread(pHeader, 1, sizeof(TGAINFOHEADER), file);

      if (pHeader->imageType == 2) /* RGB data. */
      {
         if (CheckSize(pHeader->width, pHeader->height) != false)
         {
            size_t colorMode = pHeader->bpp / 8;
            size_t imageSize = pHeader->width * pHeader->height * colorMode;

            tgaImage = new BYTE[imageSize];

            fread(tgaImage, 1, imageSize, file);

         /* Swap the R and B values to get RGB since the color format is stored in BGR. */
            for (size_t i = 0; i < imageSize; i += 3)
            {
               BYTE tBlue      = tgaImage[i + 0];
               tgaImage[i + 0] = tgaImage[i + 2];
               tgaImage[i + 2] = tBlue;
            }
         }
      }

      fclose(file);
   }

   return tgaImage;
}

/* Returns a pointer to the tga image data that has an alpha channel
   of the tga specified by filename. Also returns the tga header
   information. */
BYTE* Texture2d::LoadTGAIntoRGBA(const char* szFileName, TGAINFOHEADER* pHeader)
{
   BYTE* tgaImage = NULL;
   FILE* file = fopen(szFileName, "rb");

   if (file != NULL)
   {
      fread(pHeader, 1, sizeof(TGAINFOHEADER), file);

      if (pHeader->imageType == 2) /* RGB data. */
      {
         if (CheckSize(pHeader->width, pHeader->height) != false)
         {
            size_t colorMode = pHeader->bpp / 8;
            size_t imageSize = pHeader->width * pHeader->height * colorMode;

            tgaImage = new BYTE[imageSize];

            fread(tgaImage, 1, imageSize, file);

         /* Swap the R and B values to get RGB since the color format is stored in BGR. */
            for (size_t i = 0; i < imageSize; i += 4)
            {
               BYTE tBlue      = tgaImage[i + 0];
               tgaImage[i + 0] = tgaImage[i + 2];
               tgaImage[i + 2] = tBlue;
            }
         }
      }

      fclose(file);
   }

   return tgaImage;
}

/* Returns a pointer to the bitmap image data of the bitmap specified
   by filename. Also returns the bitmap header information.
   No support for 8-bit bitmaps. */
BYTE* Texture2d::LoadBMPIntoRGB(const char* szFileName, BITMAPINFOHEADER* pHeader)
{
   BITMAPFILEHEADER bmpHeader = {0};  /* Bitmap file header. */
   BYTE* bmpImage = NULL;             /* Bitmap image data. */

   FILE* file = fopen(szFileName, "rb");

   if (file != NULL)
   {
   /* Read the bitmap file header. */
      fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);

   /* Verify that this is a bitmap by checking for the universal bitmap id. */
      if (bmpHeader.bfType == BITMAP_ID)
      {
      /* Read the bitmap information header. */
         fread(pHeader, sizeof(BITMAPINFOHEADER), 1, file);

         if (CheckSize(pHeader->biWidth, pHeader->biHeight) != false && pHeader->biBitCount == 24 && pHeader->biCompression == BI_RGB)
         {
            pHeader->biSizeImage = pHeader->biWidth * pHeader->biHeight * 3; /* FLANNERY, quick bug fix! Some bitmaps have a zeroed image size. */
            bmpImage = (BYTE*) malloc(pHeader->biSizeImage);

            if (bmpImage != NULL)
            {
               fseek(file, bmpHeader.bfOffBits, SEEK_SET);
               fread(bmpImage, sizeof(BYTE), pHeader->biSizeImage, file);

            /* Swap the R and B values to get RGB since the bitmap color format is in BGR. */
               for (size_t i = 0; i < pHeader->biSizeImage; i += 3)
               {
                  BYTE tBlue      = bmpImage[i + 0];
                  bmpImage[i + 0] = bmpImage[i + 2];
                  bmpImage[i + 2] = tBlue;
               }
            }
         }
      }

      fclose(file);
   }

   return bmpImage;
}

/* Returns a pointer to the bitmap image data of the bitmap specified
   by filename with the addition of an alpha channel. Also returns
   the bitmap header information. No support for 8-bit bitmaps. */
BYTE* Texture2d::LoadBMPIntoRGBA(const char* szFileName, BITMAPINFOHEADER* pHeader)
{
   BITMAPFILEHEADER bmpHeader = {0};  /* Bitmap file header. */
   BYTE* bmpImage = NULL, * p = NULL; /* Bitmap image data. */

   FILE* file = fopen(szFileName, "rb");

   if (file != NULL)
   {
   /* Read the bitmap file header. */
      fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);

   /* Verify that this is a bitmap by checking for the universal bitmap id. */
      if (bmpHeader.bfType == BITMAP_ID)
      {
      /* Read the bitmap information header. */
         fread(pHeader, sizeof(BITMAPINFOHEADER), 1, file);

         if (CheckSize(pHeader->biWidth, pHeader->biHeight) != false && pHeader->biBitCount == 24 && pHeader->biCompression == BI_RGB)
         {
            pHeader->biSizeImage = pHeader->biWidth * pHeader->biHeight * 3; /* FLANNERY, quick bug fix! Some bitmaps have a zeroed image size. */
            bmpImage = (BYTE*) malloc(pHeader->biSizeImage);
            p        = (BYTE*) malloc((pHeader->biSizeImage / 3) * 4);

            if (bmpImage != NULL)
            {
               fseek(file, bmpHeader.bfOffBits, SEEK_SET);
               fread(bmpImage, sizeof(BYTE), pHeader->biSizeImage, file);

            /* Swap the R and B values to get RGB since the bitmap color format is in BGR. */
               for (size_t i = 0, a = 0; i < pHeader->biSizeImage; i += 3, a += 4)
               {
                  p[a + 0] = bmpImage[i + 2];
                  p[a + 1] = bmpImage[i + 1];
                  p[a + 2] = bmpImage[i + 0];

                  if (bmpImage[i + 0] + bmpImage[i + 1] + bmpImage[i + 2] < 3)
                  {
                     p[a + 3] = 0;
                  }
                  else
                  {
                     p[a + 3] = 255;
                  }
               }

               free(bmpImage);
            }
         }
      }

      fclose(file);
   }

   return p;
}

#endif
