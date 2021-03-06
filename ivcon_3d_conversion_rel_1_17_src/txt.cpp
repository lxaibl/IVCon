/******************************************************************************/
// Author:  John Burkardt
// Change: 02 Mar 2007 Tomasz Lis adjusted to new IVCONV class, minor fixes

// Purpose:
//   Support of writing all available 3D data into text file, for easy investigation.

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - No read support
// Format support note - writing:
// - Full 32-bit vertices support
// - Full 32-bit faces support, unlimited face order
// - Full meshes support
// - Full materials support
// - Full textures support
// - Texture UV supported
// - Default view supported
// - Environment supported, no lights support
// - Shapes supported

//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.

/******************************************************************************/

#include "ivconv.h"

bool IVCONV::txt_write ( FILE *fileout )

/******************************************************************************/
/*
Purpose:

  TXT_WRITE writes the graphics data to a text file.
  
	Author: John Burkardt
	Modified: 25 June 1998
*/
{
	stats.text_num = 0;
	
	fprintf ( fileout, "%s created by IVCON.\n", fileout_name );
	fprintf ( fileout, "Original data in %s.\n", filein_name );
	fprintf ( fileout, "View origin at %f %f %f.\n",
        view.origin[0], view.origin[1], view.origin[2] );
	fprintf ( fileout, "View pivot at %f %f %f.\n",
        view.pivot[0], view.pivot[1], view.pivot[2] );
	stats.text_num +=5;

	fprintf ( fileout, "Scene background color: %f %f %f\n",
        env.rgb[0], env.rgb[1], env.rgb[2] );
	stats.text_num++;
    if (env.texture>=0)
    {
        fprintf ( fileout, "Background texture number: %d\n",
            env.texture );
        stats.text_num++;
    }

	//TRANSFORMATION MATRIX.
	fprintf ( fileout, "\n" );
	fprintf ( fileout, "Transformation matrix:\n" );
	fprintf ( fileout, "\n" );
	stats.text_num += 3;
	
	unsigned int i;
	for ( i = 0; i < 4; ++i )
    {
		fprintf ( fileout, "  %9f %9f %9f %9f\n", transform_matrix[i][0],
			transform_matrix[i][1], transform_matrix[i][2], transform_matrix[i][3] );
		stats.text_num++;
	}

	//VERTICES
	fprintf ( fileout, "\n" );
	fprintf ( fileout, "  Vertices: %lu\n", vertex_num );
	stats.text_num = stats.text_num + 2;
    if (vertex_num>0)
    {
        fprintf ( fileout, "%7s %9s %9s %9s %9s %9s %9s\n",
                  "idx","coordX","coordY","coordZ","colorR","colorG","colorB");
        stats.text_num++;
        for ( unsigned long ivert = 0; ivert < vertex_num; ivert++ )
        {
            Vertex *vert=&vertex[ivert];
            fprintf ( fileout, "%7lu %9f %9f %9f %9f %9f %9f\n",ivert,
                vert->cor3[0],vert->cor3[1],vert->cor3[2],vert->rgb[0],vert->rgb[1],vert->rgb[2]);
            stats.text_num++;
        }
        fprintf ( fileout, "\n" );
        fprintf ( fileout, "  Additional per-vertex data\n" );
        stats.text_num+=2;
        fprintf ( fileout, "%7s %6s %9s %9s %9s %9s %9s\n",
                  "idx","mater.","textureU","textureV","normalX","normalY","normalZ");
        stats.text_num++;
        for ( unsigned long ivert = 0; ivert < vertex_num; ivert++ )
        {
            Vertex *vert=&vertex[ivert];
            fprintf ( fileout, "%7lu %6u %9f %9f %9f %9f %9f\n",ivert,
                vert->material,vert->tex_uv[0],vert->tex_uv[1],vert->normal[0],vert->normal[1],vert->normal[2]);
            stats.text_num++;
        }
    }

    //FACES
	fprintf ( fileout, "\n" );
	fprintf ( fileout, "  Faces: %lu\n", face_num );
	stats.text_num = stats.text_num + 2;
    if (face_num>0)
    {
        fprintf ( fileout, "%7s %6s %6s %6s %8s\n",
                  "idx","mesh","flags","smooth","area");
        stats.text_num++;
        for ( unsigned long iface = 0; iface < face_num; iface++ )
        {
            Face *fac=&face[iface];
            fprintf ( fileout, "%7lu %6u %6u %6u %9f\n",iface,
                fac->mesh,fac->flags,fac->smooth,fac->area);
            stats.text_num++;
        }
        fprintf ( fileout, "\n" );
        fprintf ( fileout, "  Faces data\n" );
        stats.text_num+=2;
        fprintf ( fileout, "%7s %6s %9s %9s %9s %9s\n",
                  "idx","order","normalX","normalY","normalZ","vertices");
        stats.text_num++;
        for ( unsigned long iface = 0; iface < face_num; iface++ )
        {
            Face *fac=&face[iface];
            fprintf ( fileout, "%7lu %6u %9f %9f %9f",iface,
                fac->order,fac->normal[0],fac->normal[1],fac->normal[2]);
            for (unsigned int ivert=0;ivert<fac->order;ivert++)
                fprintf ( fileout, " %4u",fac->vertices[ivert]);
            fprintf ( fileout, "\n" );
            stats.text_num++;
        }
    }
    //MESHES
	fprintf ( fileout, "\n" );
	fprintf ( fileout, "  Meshes: %lu\n", mesh_num );
	stats.text_num = stats.text_num + 2;
    if (mesh_num>0)
    {
        fprintf ( fileout, "%6s  %s\n","idx","name");
        stats.text_num++;
        for ( unsigned long imesh = 0; imesh < mesh_num; imesh++ )
        {
            fprintf ( fileout, "%6lu \"%s\"\n",imesh,mesh[imesh].name);
            stats.text_num++;
        }
    }

	//LINES.
	fprintf ( fileout, "\n" );
	fprintf ( fileout, "  Shapes(lines): %d\n", shape_num );
	stats.text_num = stats.text_num + 2;
	if ( shape_num > 0 )
    {
		fprintf ( fileout, "\n" );
		fprintf ( fileout, "  Line coordinates data:\n" );
		fprintf ( fileout, "  #line X Y Z\n" );
		stats.text_num = stats.text_num + 3;
		
		unsigned int nitem = 0;
		unsigned long iline;
		for ( iline = 0; iline < shape_num; iline++ )
          for (unsigned long ptidx=0;ptidx<shape[iline].order;ptidx++)
          {
              fprintf ( fileout, "%3lu% 10f% 10f% 10f\n",ptidx,shape[iline].cor3[0][ptidx],
                      shape[iline].cor3[1][ptidx],shape[iline].cor3[2][ptidx] );
              stats.text_num++;
	      }
		
		fprintf ( fileout, "\n" );
		fprintf ( fileout, "  Line colors and order:\n" );
		fprintf ( fileout, "\n" );
		stats.text_num+=3;
		
		nitem = 0;
		for ( iline = 0; iline < shape_num; iline++ )
        {
			fprintf ( fileout, "%03lu %f %f %f %lu\n",iline,shape[iline].rgb[0],
                    shape[iline].rgb[1],shape[iline].rgb[2],shape[iline].order);
		    stats.text_num++;
		}
	}

	//MATERIALS.
	fprintf ( fileout, "\n" );
	fprintf ( fileout, "  Materials: %lu\n", material_num );
	stats.text_num +=2;
    if (material_num>0)
    {
        fprintf ( fileout, "%7s %6s %6s %6s %7s %s\n",
          "idx","textr.","texMsk","texBump","alpha","  name");
        stats.text_num++;
        for ( unsigned long imat = 0; imat < material_num; imat++ )
        {
            Material *matr=&material[imat];
            fprintf ( fileout, "%7lu %6d %6d %6d %9f \"%s\"\n",imat,
                matr->texture,matr->mask_texture,matr->bump_texture,
                matr->alpha,matr->name);
            stats.text_num++;
        }
        fprintf ( fileout, "\n" );
        fprintf ( fileout, "  Material colors\n");
        stats.text_num +=2;
        fprintf ( fileout, "%7s %9s %9s %9s %9s %9s %9s %9s %9s %9s\n",
          "idx","realR","realG","realB","ambientR","ambientG","ambientB","speculR","speculG","speculB");
        stats.text_num++;
        for ( unsigned long imat = 0; imat < material_num; imat++ )
        {
            Material *matr=&material[imat];
            fprintf ( fileout, "%7lu %9f %9f %9f %9f %9f %9f %9f %9f %9f\n",imat,
                matr->rgb[0],matr->rgb[1],matr->rgb[2],
                matr->ambient[0],matr->ambient[1],matr->ambient[2],
                matr->specular[0],matr->specular[1],matr->specular[2]);
            stats.text_num++;
        }
    }

	//TEXTURES.
	fprintf ( fileout, "\n" );
	fprintf ( fileout, "  Textures: %lu\n", texture_num );
	stats.text_num = stats.text_num + 2;
	if (texture_num > 0)
    {
        fprintf ( fileout, "%7s %9s %s\n",
          "idx","intense"," filename");
        stats.text_num++;
        for ( unsigned long itxtr = 0; itxtr < texture_num; itxtr++ )
        {
            Texture *textr=&texture[itxtr];
            fprintf ( fileout, "%7lu %9f \"%s\"\n",itxtr,
                textr->intense,textr->name);
            stats.text_num++;
        }
	}

	//Report.
	printf ( "\n" );
	printf ( "TXT_WRITE - Wrote %d text lines.\n", stats.text_num );
	
	return true;
}
/**********************************************************************/

