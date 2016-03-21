local qt = premake.extensions.qt

project "MediaViewer"

	-- project type
	kind "WindowedApp"
	language "C++"

	-- output folders
	objdir ( "../Output/obj/%{prj.name}/%{cfg.buildcfg}/%{cfg.platform}" )
	targetdir ( "../Output/bin/%{cfg.buildcfg}/%{cfg.platform}" )

	-- enable Qt for this project
	qt.enable()
	qtmodules { "core", "qml", "gui", "quick", "widgets" }

	-- files of the project
	files {
		"Sources/**",
		"Resources/**"
	}
	vpaths {
		["*"] = { "Sources" },
		["Generated/*"] = { "../Output/obj/MediaViewer" },
	}

	-- precompiled headers
	pchsource "Sources/MediaViewerPCH.cpp"
	pchheader "MediaViewerPCH.h"

	--
	-- uncomment to enable verbose QML import logs
	--
	-- filter ( "Debug" )			debugenvs { "QML_IMPORT_TRACE=1"}
