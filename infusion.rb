
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

required_version "0.2"
require 'teapot/build'

define_package "unit-test-0.1.0" do |package|
	package.build(:all) do |platform, environment|
		# Need to set build_prefix and install_prefix appropriately:
		top = Teapot::Build.top(Pathname.new(__FILE__).dirname)

		top.add_directory('source')
		top.add_directory('test')
		
		local_build = environment.merge do
			build_prefix "build-#{platform.name}-#{variant}"
			install_prefix platform.prefix
		end
		
		top.execute(:install, local_build)
	end
end
