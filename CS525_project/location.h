#pragma once
namespace UniformLoc
{
	// need to change later
	const int M_loc = 0;
	const int V_loc = 4;
	const int P_loc = 8;
	const int PVM_loc = 12;
	const int texture_loc[] = { 16,17,18,19,20,21 };
	const int height_index_loc = 22;
	const int cubemap_loc = 23;
	const int pass_loc = 24;
	const int plane_loc = 25;

	const int time_loc = 30;
	const int waveSpeed_loc = 31;
	const int cameraPos_loc = 32;



	//water
	const int waterColor_loc = 38;
	const int waterColorMix_loc = 39;
	const int waterTexture_loc[] = { 26,27,28 ,33 };
	const int wave_strength_loc = 29;
	const int waveLength_loc = 48;
	const int waveAmplitude_loc = 49;
	const int clipDistance_loc = 50;
	const int edgeSoftness_loc = 51;
	const int minblue_loc = 52;
	const int maxblue_loc = 53;
	const int murkyDepth_loc = 54;

	// fog
	const int density_loc = 40;
	const int gradient_loc = 41;
	const int fogColor_loc = 42;

	//pick objects
	const int meshid_loc = 43;
	const int currentid_loc = 44;


	//flat shading
	const int shadingMode_loc = 45;

	//depth
	const int test_loc = 46;
	const int depthTexture_loc = 47;
	const int depthView_loc = 55;

	//gui
	const int guiwh_loc = 56;
	const int guipos_loc = 57;
	const int windowWH_loc = 58;

	// sun

	const int sunPos_loc = 59;
	const int sunScale_loc = 60;
	const int flaretexture_loc = 61;
	const int strength_loc = 62;
	const int fadefactor_loc = 63;
	const int suntexture_loc = 83;

	//point light
	const int lightPos_loc = 34;
	const int shiness_loc = 37;
	const int pka_loc = 64;
	const int pLa_loc = 65;
	const int pkd_loc = 66;
	const int pLd_loc = 67;
	const int pks_loc = 35;
	const int pLs_loc = 36;

	//dir light
	const int dka_loc = 68;
	const int dLa_loc = 69;
	const int dkd_loc = 70;
	const int dLd_loc = 71;
	const int dir_loc = 72;

	//terrain 
	const int peakcolor_loc =73;
	const int highpartcolor_loc = 74;
	const int highheight_loc = 75;
	const int middlepartcolor_loc = 76;
	const int middleheight_loc = 77;
	const int underwatercolor_loc = 78;
	const int terrainheight_loc = 79;
	const int peakheight_loc = 80;
	const int underheight_loc = 81;

	//tesselation
	const int level_loc = 82;


	//picking objects
	const int terrainpostexture_loc = 85;
	const int submeshcolor_loc = 84;

	const int meshhighlight_loc = 85;
	//cubemap 
	//const int cubemapV_loc[6] = { 64,68,72,76,80,84 };
	//const int dcubemap_loc = 88;


};

namespace AttribLoc
{
	const int Pos_loc = 0;
	const int TexCoord_loc = 1;
	const int Normal_loc = 2;
};

// texture 0~5 : used in terrain

//texture 6, 7 : used in water; 8 dudv map ; 9 water normal

//texture 10 :  depth texture

//texture 11 : lensfla

//texture 12: dynamic cube map

//texture 13: sun texture

