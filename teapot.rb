
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "0.8.0"

define_target "euclid" do |target|
	target.build do |environment|
		build_directory(package.path, 'source', environment)
	end
	
	target.depends :platform
	target.depends "Language/C++11"
	
	target.provides "Library/Euclid" do
		append linkflags "-lEuclid"
	end
end

define_target "euclid-tests" do |target|
	target.build do |environment|
		build_directory(package.path, 'test', environment)
	end
	
	target.depends :platform
	target.depends "Language/C++11"
	target.depends "Library/UnitTest"
	target.depends "Library/Euclid"
	
	target.provides "Test/Euclid"
end

define_configuration "euclid" do |configuration|
	configuration[:source] = "https://github.com/dream-framework"
	
	configuration.import! "platforms"
	
	configuration.require "unit-test"
end

define_configuration "travis" do |configuration|
	configuration[:source] = "https://github.com/dream-framework"
	
	configuration.require "platform-linux"
	configuration.require "variants"
	
	configuration.require "unit-test"
end

define_configuration "local" do |configuration|
	configuration[:source] = "../"
	configuration.import! "local"
end