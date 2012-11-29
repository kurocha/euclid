
add_library 'Euclid' do
	def sources(environment)
		FileList[root, 'Euclid/**/*.cpp']
	end
	
	def headers(environment)
		FileList[root, 'Euclid/**/*.{h,hpp}']
	end
	
	def files(environment)
		FileList[root, "bin/*"]
	end
end
