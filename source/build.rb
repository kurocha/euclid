
compile_library 'Euclid' do
	def source_files(environment)
		FileList[root, 'Euclid/**/*.cpp']
	end
end

copy_headers do
	def source_files(environment)
		FileList[root, 'Euclid/**/*.{h,hpp}']
	end
end
