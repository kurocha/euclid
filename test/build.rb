
compile_executable("euclid-test-runner") do
	def source_files(environment)
		FileList[root, "**/*.cpp"]
	end
end
