
add_executable("euclid-test-runner") do
	configure do
		linkflags ["-lUnitTest", "-lEuclid"]
	end
	
	def sources(environment)
		FileList[root, "**/*.cpp"]
	end
end
