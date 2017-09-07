
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "1.0.0"

# Project Metadata

define_project "euclid" do |project|
	project.title = "Euclid"
	project.summary = 'A computational geometry and math library.'
	
	project.license = 'MIT License'
	
	project.add_author 'Samuel Williams', email: 'samuel.williams@oriontransfer.co.nz'
	
	project.version = '1.0.0'
end

# Build Targets

define_target 'euclid-library' do |target|
	target.build do
		source_root = target.package.path + 'source'
		
		copy headers: source_root.glob('Euclid/**/*.{h,hpp}')
		
		build static_library: "Euclid", source_files: source_root.glob('Euclid/**/*.cpp')
	end
	
	target.depends "Build/Files"
	target.depends "Build/Clang"
	
	target.depends :platform
	target.depends "Language/C++11", private: true
	
	target.provides "Library/Euclid" do
		append linkflags [
			->{install_prefix + 'lib/libEuclid.a'},
		]
	end
end

define_target "euclid-tests" do |target|
	target.build do |*arguments|
		test_root = target.package.path + 'test'
		
		run tests: 'Euclid', source_files: test_root.glob('Euclid/**/*.cpp'), arguments: arguments
	end
	
	target.depends "Build/Clang"
	
	target.depends :platform
	target.depends "Language/C++11", private: true
	target.depends "Library/UnitTest"
	target.depends "Library/Euclid"
	
	target.provides "Test/Euclid"
end

# Configurations

define_configuration 'development' do |configuration|
	configuration[:source] = "https://github.com/kurocha"
	
	configuration.import 'euclid'
	
	# Provides all the build related infrastructure:
	configuration.require "platforms"
	configuration.require "build-files"
	
	# Provides unit testing infrastructure and generators:
	configuration.require "unit-test"
	
	# Provides some useful C++ generators:
	configuration.require 'generate-cpp-class'
	
	configuration.require "generate-project"
	configuration.require "generate-travis"
end

define_configuration "euclid" do |configuration|
	configuration.public!
end
