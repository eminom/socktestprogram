
require "fileutils"
require_relative "exconf"

module ProtoSync
	def self.init
		@origin_path = ConfigEx.origin_path
		if not File.directory? @origin_path then
			print "Not expected for #{@origin_path}\n"
			exit
		end
	end

	def self.process
		Dir.glob("#{@origin_path}/**/*.proto") do |path|
			next if File.directory?(path)
			lpath = File.basename(path)
			print "Processing #{lpath} ... "
			lpath = lpath.chomp(File.extname(lpath)) + ".pb"
			target_path = "#{ConfigEx.current_path}/#{lpath}"
			cmd = "protoc.exe --proto_path=\"#{@origin_path}\" -o #{target_path} #{path}"
			if not system cmd then
				print "Error compiling protocols:#{$?}\n"
				print "Command line: #{cmd}\n"
				exit
			end
			print "\n"
		end
	end

	def self.test
		now_path = ConfigEx.current_path
		print "Testing under #{now_path}\n"
		Dir.glob("#{now_path}/**/*.proto") do |path|
			print "Inspecting #{path} ..."
			cmd = "protoc.exe --proto_path=\"#{now_path}\" -o #{path}.pb #{path}"
			res = system cmd
			print "executing result: #{res} : #{$?}\n"
		end
	end
end	

# ProtoSync.test
ProtoSync.init
ProtoSync.process
print "Finished\n"


