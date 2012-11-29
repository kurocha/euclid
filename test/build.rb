
add_executable("euclid-test-runner") do
	configure do
		linkflags ["-lUnitTest", "-lEuclid"]
	end
	
	def sources(environment)
		Pathname.glob(root + "**/*.cpp")
	end
end
