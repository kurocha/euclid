
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

required_version "0.5"

define_package "euclid" do |package|
	package.install do |environment|
		top = Teapot::Build.top(package.path)

		top.add_directory('source')
		top.add_directory('test')
		
		top.execute(:install, environment)
	end
	
	package.depends :system
	package.depends "Language/C++11"
	package.depends "Library/unit-test"
	
	package.provides "Library/euclid" do
		append linkflags "-lEuclid"
	end
end
