
add_library 'Euclid' do
	def sources
		Pathname.glob(root + 'Euclid/**/*.cpp')
	end
	
	def headers
		Pathname.glob(root + 'Euclid/**/*.h')
	end
	
	def files
		Pathname.glob(root + "bin/*")
	end
end
