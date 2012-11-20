
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

required_version "0.2"
require 'teapot/build'

define_package "euclid-0.1.0" do |package|
	package.build(:all) do |platform, environment|
		# Need to set build_prefix and install_prefix appropriately:
		top = Teapot::Build.top(Pathname.new(__FILE__).dirname)

		top.add_directory('source')
		top.add_directory('test')
		
		top.execute(:install, environment.flatten)
	end
end
