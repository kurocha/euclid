
require 'teapot/build'

ENV['BUILD_PREFIX'] ||= File.expand_path("../build", __FILE__)
ENV['INSTALL_PREFIX'] ||= File.expand_path("../install", __FILE__)

environment = Teapot::Environment.new do
	buildflags {Teapot::Commands.split(ENV['BUILDFLAGS'])}

	ccflags {[
		buildflags,
		Teapot::Commands.split(ENV['CFLAGS'])
	]}

	cxxflags {[
		buildflags,
		Teapot::Commands.split(ENV['CXXFLAGS'])
	]}
	
	default cxx (ENV['CXX'] || "clang++")
	default cc (ENV['CC'] || "clang")
	
	default build_prefix (ENV['BUILD_PREFIX'] || "build")
	default install_prefix (ENV['INSTALL_PREFIX'] || "install")
end

top = Teapot::Build.top(Pathname.new(__FILE__).dirname)

top.add_directory('source')
top.add_directory('test')

task :build do
	top.execute(:build, environment)
end

task :install do
	top.execute(:install, environment)
end

task :default => :build