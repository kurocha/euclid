
add_library 'Euclid' do
	def sources(environment)
		FileList[root, 'Euclid/**/*.cpp']
	end
	
	def headers(environment)
		FileList[root, 'Euclid/**/*.{h,hpp}']
	end
end
