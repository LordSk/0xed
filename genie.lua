--
local BUILD_DIR = path.getabsolute("./build")
local SDL2_include = path.getabsolute("./external/SDL2-2.0.20/include")
local SDL2_libdir = path.getabsolute("./external/SDL2-2.0.20/lib/x64")

solution "0xed solution"
	location(BUILD_DIR)
	targetdir(BUILD_DIR)
	
	configurations {
		"Debug",
		"Release"
	}

	platforms {
		"x64"
	}
	
	language "C++"
	
	configuration {"Debug"}
		targetsuffix "_dbg"
		flags {
			"Symbols"
		}
		defines {
			"DEBUG",
			"CONF_DEBUG"
		}
	
	configuration {"Release"}
		flags {
			"Optimize"
		}
		defines {
			"NDEBUG",
			"CONF_RELEASE"
		}
	
	configuration { "qbs" }
		targetdir("../build")

	configuration {}
	
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