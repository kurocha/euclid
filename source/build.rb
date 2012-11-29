
add_library 'Euclid' do
	def sources(environment)
		Pathname.glob(root + 'Euclid/**/*.cpp')
	end
	
	def headers(environment)
		Pathname.glob(root + 'Euclid/**/*.{h,hpp}')
	end
	
	def files(environment)
		Pathname.glob(root + "bin/*")
	end
end
