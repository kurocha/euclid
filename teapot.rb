
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

required_version "0.5"

define_target "euclid" do |target|
	target.install do |environment|
		Teapot::Build.install_directory(package.path, 'source', environment)
	end
	
	target.depends :platform
	target.depends "Language/C++11"
	
	target.provides "Library/Euclid" do
		append linkflags "-lEuclid"
	end
end

define_target "euclid-tests" do |target|
	target.install do |environment|
		Teapot::Build.install_directory(package.path, 'test', environment)
	end
	
	target.depends :platform
	target.depends "Language/C++11"
	target.depends "Library/UnitTest"
	target.depends "Library/Euclid"
	
	target.provides "Test/Euclid"
end
