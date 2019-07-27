--
dofile("config.lua");

BUILD_DIR = path.getabsolute("./build")

solution "0xed solution"
	location(BUILD_DIR)
	
	configurations {
		"Debug",
		"Release"
	}

	platforms {
		"x64"
	}
	
	language "C++"
	
	configuration {"Debug"}
		targetsuffix "_debug"
		flags {
			"Symbols"
		}
		defines {
			"DEBUG",
			"CONF_DEBUG"
		}
	
	configuration {"Release"}
		targetsuffix "_release"
		flags {
			"Optimize"
		}
		defines {
			"NDEBUG",
			"CONF_RELEASE"
		}
	
	configuration {}
	
	targetdir("../build")
	
	includedirs {
		"src",
		SDL2_include,
	}

	libdirs {
		SDL2_libdir
	}
	
	links {
		"user32",
		"shell32",
		"winmm",
		"ole32",
		"oleaut32",
		"imm32",
		"version",
		"ws2_32",
		"advapi32",
        "comdlg32", -- GetOpenFileName 
		"gdi32",
		"glu32",
		"opengl32",
		"SDL2",
	}
	
	flags {
		"NoExceptions",
		"NoRTTI",
		"EnableSSE",
		"EnableSSE2",
		"EnableAVX",
		"EnableAVX2",
		"Cpp14"
	}
	
	defines {
		--"NK_INCLUDE_FIXED_TYPES",
		--"NK_INCLUDE_STANDARD_IO",
		--"NK_INCLUDE_STANDARD_VARARGS",
		--"NK_INCLUDE_DEFAULT_ALLOCATOR",
		--"NK_INCLUDE_VERTEX_BUFFER_OUTPUT",
		--"NK_INCLUDE_FONT_BAKING",
		--"NK_INCLUDE_DEFAULT_FONT"
		--"EASY_PROFILER_VERSION_MAJOR=1",
		--"EASY_PROFILER_VERSION_MINOR=3",
		--"EASY_PROFILER_VERSION_PATCH=0",
		--"EASY_DEFAULT_PORT=28077",
		--"BUILD_WITH_EASY_PROFILER=1"
	}
	
	-- disable exception related warnings
	buildoptions{ "/wd4577", "/wd4530" }
	

project "0xed"
	kind "WindowedApp"
	--kind "ConsoleApp"
	
	configuration {}
	
	files {
		"src/**.h",
		"src/**.c",
		"src/**.cpp",
        "src/0xed.rc",
	}