
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "1.0.0"

define_target "euclid" do |target|
	target.build do
		source_root = target.package.path + 'source'
		
		copy headers: source_root.glob('Euclid/**/*.{h,hpp}')
		
		build static_library: "Euclid",
			source_files: source_root.glob('Euclid/**/*.cpp')
	end
	
	target.depends "Build/Files"
	target.depends "Build/Clang"
	
	target.depends :platform
	target.depends "Language/C++11"
	
	target.provides "Library/Euclid" do
		append linkflags "-lEuclid"
	end
end

define_target "euclid-tests" do |target|
	target.build do
		test_root = target.package.path + 'test'
		
		build executable: "Euclid-tests", 
			source_files: test_root.glob('Euclid/**/*.cpp')
		
		run executable: "Euclid-tests"
	end
	
	target.depends "Build/Clang"
	
	target.depends :platform
	target.depends "Language/C++11"
	target.depends "Library/UnitTest"
	target.depends "Library/Euclid"
	
	target.provides "Test/Euclid"
end

define_configuration "travis" do |configuration|
	configuration[:source] = "https://github.com/dream-framework"
	
	configuration.require "platforms"
	configuration.require "build-files"
	
	configuration.require "unit-test"
end

define_configuration "local" do |configuration|
	configuration[:source] = "../"
	
	configuration.require "local"
end
