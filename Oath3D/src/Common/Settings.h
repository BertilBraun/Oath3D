#pragma once

#include <string>

namespace Oath {
	namespace Settings {

		static std::string TEXTURE_PATH		= "res/Textures/";
		static std::string MODEL_PATH		= "res/Models/";
		static std::string AUDIO_PATH		= "res/Audios/";
		static std::string SHADER_PATH		= "res/Shaders/";

		static std::string SHADER_VERT_EXT	= "Vert.glsl";
		static std::string SHADER_GEOM_EXT	= "Geom.glsl";
		static std::string SHADER_FRAG_EXT	= "Frag.glsl";
		static std::string SHADER_COMP_EXT	= "Comp.glsl";

		static std::string LOG_FILE = "Log.txt";
		
		static float FRAME_TIME = 1 / 60.0f;
		static bool CONTINUE_WITH_LOST_FOCUS = false;
		static bool DEBUG_RENDER_BULLET = false;
		
		static bool USING_IMGUI = true;
		
		static float FOV	= 90.0f;
		static float ZNEAR	= 0.1f;
		static float ZFAR	= 1000.0f;

		static float WIDTH	= 1600;
		static float HEIGHT	= 900;
		
		static float GRAVITY = 9.81f;

		static const int NUM_CASCADES = 1;

		static float CASCADES[NUM_CASCADES + 1] { ZNEAR, 250/*, 90, 250*/ };
		static float SHADOW_OFFSET = 20;
		
		static GLsizei SHADOW_MAP_SIZE = 4096;
	};
}