PROJECT_NAME = path.getname(os.getcwd())

minko.project.library("minko-plugin-" .. PROJECT_NAME)

	files {
		"lib/**.hpp",
		"lib/**.h",
		"lib/**.cpp",
		"lib/**.c",
		"include/**.hpp",
		"src/**.cpp"
	}

	includedirs {
		"include"
	}

	if os.getenv('EMSCRIPTEN_WGET_HEADERS') then
		configuration { "html5" }
			defines { "EMSCRIPTEN_WGET_HEADERS" }
	end

	minko.plugin.enable("http-worker")
