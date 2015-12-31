Pod::Spec.new do |s|

  s.name         = "cslim"
  s.version      = "1.1.1"
  s.summary      = "An implementation of FitNesse SliM for C and Objective-C"
  s.homepage     = "https://github.com/dougbradbury/cslim"
  s.license      = { :type => 'EPL', :file => 'LICENSE' }
  s.authors      = "Robert Martin", "James Grenning", "Doug Bradbury", "Eric Myer" 
  s.source       = { :git => "https://github.com/paulstringer/cslim.git", :tag  => "#{s.version}" }
  s.source_files = 'include/Com/*.h', 'include/CSlim/*.h', 'include/ExecutorObjectiveC/*.h', 'src/Com/*', 'src/CSlim/*', 'src/ExecutorObjectiveC/*', 'fixtures/Main.c'
  s.private_header_files = "**/*.h"
  
  main_file_osx = 'src/ExecutorObjectiveC/main.m'
  main_file_ios = 'fixtures/Main.c'
  s.ios.exclude_files= main_file_osx
  s.osx.exclude_files = main_file_ios

  main_files_all = main_file_osx, main_file_ios
  s.tvos.exclude_files = main_files_all
  s.watchos.exclude_files = main_files_all

end
