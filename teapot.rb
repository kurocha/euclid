
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

required_version "0.5"

define_target "euclid" do |target|
	target.install do |environment|
		top = Teapot::Build.top(package.path)

		top.add_directory('source')
		top.add_directory('test')
		
		top.execute(:install, environment)
	end
	
	target.depends :platform
	target.depends "Language/C++11"
	target.depends "Library/unit-test"
	
	target.provides "Library/euclid" do
		append linkflags "-lEuclid"
	end
end
